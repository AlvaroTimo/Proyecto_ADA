#include <bits/stdc++.h>
using namespace std;

//Variables Globales-----------------------------------------------------------------------
unsigned long long numConfChecks;
int verbose;

//Estructura grafo-------------------------------------------------------------------------
struct Graph {
	int n = 0;
	int m = 0;
	int maxDeg = 0;
	vector<vector<int>> AList;
	vector<int> deg;
};

//Estructura utilizada junto con ordenar por degree--------------------------------------
struct degItem {
	int deg;
	int vertex;
};
struct maxDeg {
	bool operator() (const degItem& lhs, const degItem& rhs) const {
		//Compara dos degItems por grado y luego por vértice
		numConfChecks += 2;
		if (lhs.deg > rhs.deg) return true;
		numConfChecks += 2;
		if (lhs.deg < rhs.deg) return false;
		//if we are here we know that lhs.deg == rhs.deg. Our choice can be arbitrary
		if (lhs.vertex > rhs.vertex) return true;
		else return false;
	}
};

//Estructura utilizada junto con la cola de prioridad DSatur-------------------------------
struct satItem {
	int sat;
	int deg;
	int vertex;
};
struct maxSat {
	bool operator() (const satItem& lhs, const satItem& rhs) const {
		//Compara dos satItems sat deg, luego por degree y por vértice
		numConfChecks += 2;
		if (lhs.sat > rhs.sat) return true;
		numConfChecks += 2;
		if (lhs.sat < rhs.sat) return false;
		numConfChecks += 2;
		if (lhs.deg > rhs.deg) return true;
		numConfChecks += 2;
		if (lhs.deg < rhs.deg) return false;

		if (lhs.vertex > rhs.vertex) return true;

		else return false;
	}
};

//Procedimientos utilizados por todos los algoritmos------------------------------------------------------
void logAndExit(string s) {
	//Escribe mensajes en la pantalla y en el archivo de registro y luego sale del programa
	ofstream resultsLog("resultsLog.log", ios::app);
	resultsLog << s;
	cout << s;
	resultsLog.close();
}
Graph readInputFile(char fname[]) {
	//Lee un archivo de formato DIMACS y devuelve la estructura Graph correspondiente
	ifstream inStream;
	inStream.open(fname);
	if (inStream.fail()) logAndExit("Error. No reconocimiento de argument/filename <" + string(fname) + "> en command.\n");
	char c, str[1000];
	int line = 0, u, v, edgeCnt = 0;
	vector<vector<bool>> A;
	Graph G;
	try {
		while (!inStream.eof()) {
			line++;
			inStream.get(c);
			if (inStream.eof()) break;
			switch (c) {
			case 'c':
				//ignorar una línea de comentario
				inStream.putback('c');
				inStream.get(str, 999, '\n');
				break;
			case 'p':
				//lee la línea de parámetros del archivo y configure la matriz de adyacencia
				inStream.get(c);
				inStream.getline(str, 999, ' ');
				if (G.n != 0 || G.m != 0) logAndExit("Archivo de entrada no valido. Línea que comienza con 'p' en la línea " + to_string(line) + " definido más de una vez.\n");
				if (strcmp(str, "edge")) logAndExit("Archivo de entrada no valido. problema en la linea " + to_string(line) + ". Palabra 'edge' no encontrada.\n");
				inStream >> G.n >> G.m;
				A.clear();
				A.resize(G.n, vector<bool>(G.n, false));
				for (u = 0; u < G.n; u++) A[u][u] = true;
				break;
			case 'e':
				//Leer un edge
				inStream >> u >> v;
				if (u < 1 || u > G.n || v < 1 || v > G.n || u == v) logAndExit("Archivo de entrada no valido. problema en la linea " + to_string(line) + ". Invalida edge.\n");
				if (!A[u - 1][v - 1]) edgeCnt++;
				else logAndExit("Archivo de entrada no valido. problema en la linea " + to_string(line) + ". Edge definido previamente.\n");
				A[u - 1][v - 1] = true;
				A[v - 1][u - 1] = true;
				break;
			default:
				logAndExit("Archivo de entrada no valido. problema en la linea " + to_string(line) + "\n");
			}
			inStream.get();
		}
		//Terminado de leer el archivo
		inStream.close();
		if (edgeCnt != G.m) logAndExit("Archivo de entrada no valido. El número de bordes de lectura no es igual al número especificado en la parte superior del archivo.\n");
	}
	catch (...) {
		logAndExit("Archivo de entrada no valido. Error no identificado cerca de la línea " + to_string(line) + ".\n");
	}
	inStream.close();
	//Verifique que no haya bordes. Si es así, salga de inmediato
	if (G.m <= 0) {
		logAndExit("El grafico no tiene bordes. La solucion optima obviamente es usar un color.\n");
	}
	//Ahora usa la matriz de adyacencia para construir el gráfico G
	G.deg.clear();
	G.AList.clear();
	G.deg.resize(G.n, 0);
	G.AList.resize(G.n, vector<int>());
	for (u = 0; u < G.n; u++) {
		for (v = 0; v < G.n; v++) {
			if (A[u][v] && u != v) {
				G.AList[u].push_back(v);
				G.deg[u]++;
			}
		}
	}
	G.maxDeg = *max_element(G.deg.begin(), G.deg.end());
	return(G);
}
void prettyPrintSolution(vector<vector<int>>& S) {
	int i, count = 0, col;
	cout << "\n\n";
	for (col = 0; col < S.size(); col++) {
		cout << "C-" << col << "\t= {";
		if (S[col].size() == 0) cout << "empty}\n";
		else {
			for (i = 0; i < S[col].size() - 1; i++) cout << S[col][i] +1 << ", ";
			cout << S[col][S[col].size() - 1] +1 << "}\n";
			count = count + S[col].size();
		}
	}
	cout << "Numero total de nodos = " << count << endl;
}

//Funciones para el algoritmo Greedy y DSatur----------------------------------------------
int getFirstFeasCol(Graph& G, int v, vector<int>& c, vector<bool>& used) {
	int i;
	for (int u : G.AList[v]) {
		if (c[u] != -1) used[c[u]] = true;
	}
	for (i = 0; i < used.size(); i++) {
		if (used[i] == false) break;
	}
	for (int u : G.AList[v]) {
		if (c[u] != -1) used[c[u]] = false;
	}
	numConfChecks += G.deg[v] + G.deg[v];
	return i;
}
vector<int> greedycol(Graph& G, bool sortByDegree) {
	int i;
	vector<bool> used(G.maxDeg + 1, false);
	vector<int> c(G.n, -1), perm;
	if (sortByDegree) {
		//Ordenar vértices por grado en tiempo O(n lg n)
		set<degItem, maxDeg> L;
		numConfChecks += G.n;
		for (i = 0; i < G.n; i++) L.insert({ G.deg[i], i });
		for (degItem el : L) perm.push_back(el.vertex);
	}
	else {
		//Baraja los vértices en tiempo O(n)
		for (i = 0; i < G.n; i++) perm.push_back(i);
		random_shuffle(perm.begin(), perm.end());
	}

	for (int v : perm) {
		i = getFirstFeasCol(G, v, c, used);
		c[v] = i;
	}
	return c;
}
vector<int> DSatur(Graph& G) {
	int u, i;
	vector<bool> used(G.maxDeg + 1, false);
	vector<int> c(G.n), d(G.n);
	vector<set<int>> adjCols(G.n);
	set<satItem, maxSat> Q;
	set<satItem, maxSat>::iterator maxPtr;
	//Inicialice las estructuras de datos. Estos son una cola de prioridad (montón binario), un conjunto de colores adyacentes a
    //cada vértice sin colorear (inicialmente vacío)
	//y el degree d(v) de cada vértice no coloreado en el gráfico inducido por vértices no coloreados
	numConfChecks += G.n;
	for (u = 0; u < G.n; u++) {
		c[u] = -1;
		d[u] = G.deg[u];
		adjCols[u] = set<int>();
		Q.emplace(satItem{ 0, d[u], u });
	}
	//algoritmo DSatur
	while (!Q.empty()) {
		//Obtener el vértice u con mayor grado de saturación, desvinculando con d. Quitar de la cola de prioridad y colorearlo.
		numConfChecks++;
		maxPtr = Q.begin();
		u = (*maxPtr).vertex;
		Q.erase(maxPtr);
		i = getFirstFeasCol(G, u, c, used);
		c[u] = i;
		//Actualice los grados de saturación y el valor d de todos los vecinos no coloreados; por lo tanto, modifique
        // sus elementos correspondientes en la cola de prioridad
		numConfChecks += G.deg[u];
		for (int v : G.AList[u]) {
			if (c[v] == -1) {
				Q.erase({ int(adjCols[v].size()), d[v], v });
				adjCols[v].insert(i);
				d[v]--;
				Q.emplace(satItem{ int(adjCols[v].size()), d[v], v });
			}
		}
	}
	return c;
}

//Funciones para el algoritmo Greedy IS------------------------------------------------------
Graph relabelVertices(Graph& G, vector<int>& P) {
	//Cree una copia de G con vértices reetiquetados de acuerdo con P
	Graph H;
	H.n = G.n, H.m = G.m, H.maxDeg = G.maxDeg;
	H.AList.resize(G.n, vector<int>());
	H.deg.resize(G.n, 0);
	for (int u = 0; u < G.n; u++) {
		numConfChecks += G.deg[u];
		for (int v : G.AList[u]) {
			H.AList[P[u]].push_back(P[v]);
			H.deg[P[u]]++;
		}
	}
	return H;
}
void updateSets(Graph& G, unordered_set<int>& X, unordered_set<int>& Y, vector<int>& c, int u) {
	//Elimine u de X (ahora está coloreado) y mueva todos los vecinos no coloreados de u de X a Y
	X.erase(u);
	numConfChecks += G.deg[u];
	for (int v : G.AList[u]) {
		if (c[v] == -1) {
			X.erase(v);
			Y.insert(v);
		}
	}
}
vector<int> greedyIS(Graph& G) {
	//Crea un gráfico H. Esta es una copia de G con vértices reetiquetados al azar.
	vector<int> P(G.n);
	for (int v = 0; v < G.n; v++) P[v] = v;
	random_shuffle(P.begin(), P.end());
	Graph H = relabelVertices(G, P);
	//Ahora colorea H
	vector<int> c(H.n, -1);
	unordered_set<int> X, Y;
	for (int v = 0; v < H.n; v++) X.insert(v);
	int i = 0, u;
	while (!X.empty()) {
		//Construcción de la clase de color i.
		while (!X.empty()) {
			u = *X.begin();
			c[u] = i;
			updateSets(H, X, Y, c, u);
		}
		X.swap(Y);
		i++;
	}
	//Convierta las etiquetas de vértice utilizadas en H para que correspondan a una solución para G.
	vector<int> final(G.n);
	for (int v = 0; v < G.n; v++) final[v] = c[P[v]];
	return final;
}

//Funciones para el algoritmo RLF------------------------------------------------------------
void populateNeighbourArrays(Graph& G, unordered_set<int>& X, vector<int>& NInX, vector<int>& NInY) {
	for (int u : X) {
		NInX[u] = 0;
		NInY[u] = 0;
	}
	for (int u : X) {
		numConfChecks += G.deg[u];
		for (int v : G.AList[u]) {
			if (X.count(v) == 1) NInX[u]++;
		}
	}
}
int chooseFirstVertex(unordered_set<int>& X, vector<int>& NInX) {
	//Seleccione el vértice en (no vacío) X que tiene el número máximo de vecinos en X
	int v = -1, max = -1;
	for (int u : X) {
		if (NInX[u] > max) {
			max = NInX[u];
			v = u;
		}
	}
	return v;
}
int chooseNextVertex(unordered_set<int>& X, vector<int>& NInY, vector<int>& NInX) {
	//Seleccione el vértice en (no vacío) X con un máximo de vecinos en Y; romper lazos de acuerdo con el mínimo de vecinos dentro de X
	int v = -1, max = -1, min = INT_MAX;
	for (int u : X) {
		if ((NInY[u] > max) || (NInY[u] == max && NInX[u] < min)) {
			max = NInY[u];
			min = NInX[u];
			v = u;
		}
	}
	return(v);
}
void updateSetsAndDegrees(Graph& G, unordered_set<int>& X, unordered_set<int>& Y, vector<int>& NInX, vector<int>& NInY, unordered_set<int>& D2, vector<int>& c, int u) {
	//Eliminar u de X (ahora está coloreado)
	X.erase(u);
	//Mueve todos los vecinos sin color de u de X a Y
	numConfChecks += G.deg[u];
	for (int v : G.AList[u]) {
		if (c[v] == -1) {
			X.erase(v);
			Y.insert(v);
		}
	}

	D2.clear();
	numConfChecks += G.deg[u];
	for (int v : G.AList[u]) {
		if (c[v] == -1) {
			D2.insert(v);
			numConfChecks += G.deg[v];
			for (int w : G.AList[v]) {
				if (c[w] == -1) {
					D2.insert(w);
				}
			}
		}
	}
	//Para cada vértice v en D2, ahora vuelva a calcular el número de vecinos (sin colorear) en X e Y
	for (int v : D2) {
		NInX[v] = 0;
		NInY[v] = 0;
		numConfChecks += G.deg[v];
		for (int w : G.AList[v]) {
			if (c[w] == -1) {
				if (X.count(w) == 1) NInX[v]++;
				else if (Y.count(w) == 1) NInY[v]++;
			}
		}
	}
}
vector<int> RLF(Graph& G) {
	vector<int> NInX(G.n), NInY(G.n), c(G.n, -1);
	unordered_set<int> X, Y, D2;
	for (int v = 0; v < G.n; v++) X.insert(v);
	int u, i = 0;
	while (!X.empty()) {
		//Construcción de la clase de color i. Primero calcule el contenido de las matrices vecinas, luego colorea el vértice u en X que tiene la mayor cantidad de vecinos en X
		populateNeighbourArrays(G, X, NInX, NInY);
		u = chooseFirstVertex(X, NInX);
		c[u] = i;
		updateSetsAndDegrees(G, X, Y, NInX, NInY, D2, c, u);
		while (!X.empty()) {
			//Colorea el vértice u en X que tiene el mayor número de vecinos en Y. Rompe los lazos de acuerdo con el mínimo de vecinos dentro de X
			u = chooseNextVertex(X, NInY, NInX);
			c[u] = i;
			updateSetsAndDegrees(G, X, Y, NInX, NInY, D2, c, u);
		}
		//Termina de construir el color i
		X.swap(Y);
		i++;
	}
	return c;
}

void table(vector<vector<int>> &sol, int k) {
    vector<string> dias = {"lunes","martes","miercoles","jueves","viernes"};

    for (int i=0; i<k; i++) {
        cout << dias[i] << ": ";
        for (auto j : sol[i])
            cout << j << " ";
        cout << "\n";
    }
    cout << '\n';
}

void readSolution(string path, int k) {
    ifstream archivo (path.c_str());
    string linea; int c=0;
    vector<vector<int>> sol(k);

    while (getline(archivo, linea)) {
        if (c > 0)
            sol[stoi(linea)].push_back(c);
        ++c;
    }

    table(sol, k); cout << '\n';
}



//Main algorithm-------------------------------------------------------------------------
int main(int argc, char** argv) {

	if (argc <= 1) {
		cout << "Algoritmos constructivos para colorear grafos\n\n"
			<< "USO:\n"
			<< "<InputFile>     (Requerido. El archivo debe estar en formato DIMACS)\n"
			<< "-a <int>        (elegir Algoritmo: 1 = Greedy (permutacion aleatoria de vertices) (default)\n"
			<< "                                   2 = Greedy (grados de vertice descendente / algoritmo Welsh-Powell)\n"
			<< "                                   3 = DSatur\n"
			<< "                                   4 = Greedy-IS (permutacion aleatoria de vertices)\n"
			<< "                                   5 = RLF)\n"
			<< "-r <int>        (Random seed(semilla). DEFAULT = 1)\n"
			<< "-v              (Info)\n"
			<< "****\n";
		exit(1);
	}

	int i, randomSeed = 1, algChoice = 1;
	numConfChecks = 0;
	verbose = 0;
	Graph G;
	try {
		for (i = 1; i < argc; i++) {
			if (strcmp("-r", argv[i]) == 0) {
				randomSeed = atoi(argv[++i]);
			}
			else if (strcmp("-a", argv[i]) == 0) {
				algChoice = atoi(argv[++i]);
			}
			else if (strcmp("-v", argv[i]) == 0) {
				verbose++;
			}
			else {
				//Configure el archivo de entrada, lea y cierre (la entrada debe estar en formato DIMACS)
				G = readInputFile(argv[i]);
			}
		}
	}
	catch (...) {
		logAndExit("Parametros invalidos. Comprueba el uso e intentalo de nuevo.\n");
	}

	//Establecer semilla aleatoria e iniciar el temporizador
	srand(randomSeed);
	clock_t runStart = clock();

	//Ejecutar el algoritmo elegido
	vector<int> c;
	if (algChoice == 1)			c = greedycol(G, false);
	else if (algChoice == 2)	c = greedycol(G, true);
	else if (algChoice == 3)	c = DSatur(G);
	else if (algChoice == 4)	c = greedyIS(G);
	else						c = RLF(G);

	//Detener el temporizador.
	int duration = (int)(((clock() - runStart) / double(CLOCKS_PER_SEC)) * 1000);

	//Convierta la solución a la representación de partición para mostrar
	int k = *max_element(c.begin(), c.end()) + 1;
	vector<vector<int>> S(k, vector<int>());
	for (i = 0; i < G.n; i++) S[c[i]].push_back(i);
	if (verbose >= 1) cout << " COLS     CPU-TIME(ms)\tCHECKS" << endl;
	if (verbose >= 1) cout << setw(5) << k << setw(11) << duration << "ms\t" << numConfChecks << endl;

    cout << '\n';
    prettyPrintSolution(S);
    cout << '\n';

	//enviar la solución a un archivo de texto
	ofstream solStrm;
	solStrm.open("solution.txt");
	solStrm << G.n << "\n";
	for (i = 0; i < G.n; i++) solStrm << c[i] << "\n";
	solStrm.close();

	if (algChoice == 1)			logAndExit("Greedy\t" + to_string(k) + "\t" + to_string(duration) + "\t" + to_string(numConfChecks) + "\n");
	else if (algChoice == 2)	logAndExit("Welsh-Powell\t" + to_string(k) + "\t" + to_string(duration) + "\t" + to_string(numConfChecks) + "\n");
	else if (algChoice == 3)	logAndExit("DSatur\t" + to_string(k) + "\t" + to_string(duration) + "\t" + to_string(numConfChecks) + "\n");
	else if (algChoice == 4)	logAndExit("GreedyIS\t" + to_string(k) + "\t" + to_string(duration) + "\t" + to_string(numConfChecks) + "\n");
	else						logAndExit("RLF\t" + to_string(k) + "\t" + to_string(duration) + "\t" + to_string(numConfChecks) + "\n");

    cout << '\n'; readSolution("solution.txt", k);

	return(0);
}
