/******************************************************************************/
//  Este codigo implementa el algoritmo greedy simple para colear grafos
//  El codigo fue escrito por R. Lewis www.rhydLewis.eu
//	
//	See: Lewis, R. (2015) A Guide to Graph Colouring: Algorithms and Applications. Berlin, Springer. 
//       ISBN: 978-3-319-25728-0. http://www.springer.com/us/book/9783319257280
//	
/******************************************************************************/
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <iomanip>

using namespace std;

unsigned long long numConfChecks;

//-------------------------------------------------------------------------------------
inline
void swap(int &a, int &b){
	int temp; temp = a; a = b; b = temp;
}

void readInputFile(ifstream &inStream, int &numNodes, int &numEdges, vector< vector<bool> > &adjacent, vector<int> &degree, vector< vector<int> > &adjList){
	//Lee un archivo de formato DIMACS y crea la matriz de grados y la matriz de adyacencia correspondientes
	char c;
	char str[1000];
	int line=0, i, j;
	numEdges=0;
	int edges=-1;
	int blem=1;
	int multiple = 0;
	while(!inStream.eof()) {
		line++;
		inStream.get(c);
		if (inStream.eof()) break;
		switch (c) {
		case 'p':
			inStream.get(c);
			inStream.getline(str,999,' ');
			if (strcmp(str,"edge") && strcmp(str,"edges")) {
				cerr << "Erro al leer la línea 'p': no se encontro la palabra clave 'edge'.\n";
				cerr << "'" << str << "' encontrado en su lugar\n";
				exit(-1);
			}
			inStream >> numNodes;
			inStream >> numEdges;
			//Configurar la matriz de adyacencia 2d
			adjacent.clear();
			adjacent.resize(numNodes, vector<bool>(numNodes));			
			for (i=0;i<numNodes;i++) for(j=0;j<numNodes;j++){
				if(i==j)adjacent[i][j]=true; 
				else adjacent[i][j] = false;
			}
			blem=0;
			break;
		case 'n':
			if (blem) {
				cerr << "Linea 'n' encontrada antes de una linea 'p'.\n";
				exit(-1);
			}
			int node;
			inStream >> node;
			if (node < 1 || node > numNodes) {
				cerr << "Numero de nodo " << node << " esta fuera de rango!\n";
				exit(-1);
			}
			node--;	
			cout << "Etiquetas (n lineas) no implementadas en el objeto g\n";
			break;
		case 'e':
			int node1, node2;
			inStream >> node1 >> node2;
			if (node1 < 1 || node1 > numNodes || node2 < 1 || node2 > numNodes){
				cerr << "Nodo " << node1 << " o " << node2 << " esta fuera de rango!\n";
				exit(-1);
			}
			node1--;
			node2--;
			if (!adjacent[node1][node2]){
				edges++;
			} else{
				multiple++;
				if (multiple<5) {
					cerr << "Advertencia: en el archivo grafico en la linea " << line << ": el borde se define mas de una vez.\n";
					if (multiple == 4) {
						cerr << "  No se emitiran mas advertencias de bordes multiples\n";
					}
				}
			}
			adjacent[node1][node2] = true;
			adjacent[node2][node1] = true;
			break;
		case 'd':
		case 'v':
		case 'x':
			cerr << "Linea de archivo " << line << ":\n";
			cerr << "'" << c << "' las lineas aun no estan implementadas...\n";
			inStream.getline(str,999,'\n');
			break;
		case 'c':
			inStream.putback('c');
			inStream.get(str,999,'\n');
			break;
		default:
			cerr << "Linea de archivo " << line << ":\n";
			cerr << "'" << c << "' es un codigo de linea desconocido\n";
			exit(-1);
		}
		inStream.get(); // Elimina la nueva linea
	}
	inStream.close();
	if (multiple){
		cerr << multiple << " varios bordes encontrados\n";
	}
	//Ahora use la matriz de adyacencia para construir la matriz de grados y la lista de adyacencia
	degree.resize(numNodes, 0);
	adjList.resize(numNodes, vector<int>());
	for(i=0; i<numNodes; i++){
		for(j=0; j<numNodes; j++){
			if(adjacent[i][j] && i!=j){
				adjList[i].push_back(j);
				degree[i]++;
			}
		}
	}
}

//-------------------------------------------------------------------------------------
inline
bool colourIsFeasible(vector< vector<int> > &sol, int c, int v, vector< vector<int> > &adjList, vector<int> &colNode, vector< vector<bool> > &adjacent){
	//Verifica si el vertice v se puede insertar de manera factible en el color c en sol	
	int i;
	numConfChecks++;
	if(sol[c].size() > adjList[v].size()){
		//Comprueba si algun vecino de v esta actualmente en color c
		for(i=0; i<adjList[v].size(); i++){
			numConfChecks++;
			if(colNode[adjList[v][i]] == c) return false;
		}
		return true;
	}
	else{
		//Comprueba si algun vertice en el color c es adyacente a v
		for(i=0; i<sol[c].size(); i++){
			numConfChecks++;
			if(adjacent[v][sol[c][i]]) return false;
		}
		return true;
	}
}

//-------------------------------------------------------------------------------------
inline 
void makeSolution(vector< vector<int> > &candSol, int verbose, vector<int> &degree, vector< vector<int> > &adjList, vector<int> &colNode, vector< vector<bool> > &adjacent, int numNodes){
	//1) Cree un vector vacio que represente todos los nodos no colocados(todos) y permute
	int i, r, j;
	vector<int> a(numNodes);
	for (i=0;i<numNodes;i++) a[i]=i;
	for(i=numNodes-1; i>=0; i--){	
		r = rand()%(i+1);
		swap(a[i],a[r]); 
	}

	//2) Ahora colorea usando el algoritmo codicioso
	//Primero, coloque el primer nodo en el primer grupo
	candSol.clear();
	candSol.push_back(vector<int>());
	candSol[0].push_back(a[0]);
	colNode[a[0]] = 0;

	//Ahora revise los nodos restantes y vea si son adecuados para cualquier color existente. Si no es asi, creamos un nuevo color 
	for(i=1; i<numNodes; i++){
		for(j=0; j<candSol.size(); j++){
			if(colourIsFeasible(candSol,j,a[i],adjList,colNode,adjacent)){
				//el elemento se puede insertar en este grupo
				candSol[j].push_back(a[i]);
				colNode[a[i]] = j;
				break;
			}
		}
		if(j>=candSol.size()){
			//Si estamos aqui, el elemento no se podria insertar en ninguno de los grupos existentes, se crea uno nuevo
			candSol.push_back(vector<int>());
			candSol.back().push_back(a[i]);
			colNode[a[i]] = candSol.size()-1;
		}
	}
}

//-------------------------------------------------------------------------------------
inline
void prettyPrintSolution(vector< vector<int> > &candSol){
	int i, count = 0, group;
	cout<<"\n\n";
	for(group=0; group<candSol.size(); group++){
		cout<<"C-"<<group<<"\t= {";
		if(candSol[group].size() == 0) cout<<"empty}\n";
		else {
			for(i=0; i<candSol[group].size()-1; i++){
				cout << candSol[group][i]<< ", ";
			}
			cout<<candSol[group][candSol[group].size()-1]<<"}\n";
			count = count + candSol[group].size();
		}
	}
	cout<<"Numero total de nodos = "<<count<<endl;
}

//---------------------------------------------------------------
inline
void checkSolution(vector< vector<int> > &candSol, vector< vector<int> > &adjacent, int numNodes){
	int j, i, count = 0, group;
	bool valid = true;

	//primero verifique que la permutacion tenga la longitud correcta
	for(group=0; group<candSol.size(); group++){
		count = count + candSol[group].size();
	}

	if(count != numNodes){ 
		cout<<"Error: la longitud de las permutaciones no es igual al tamanhio del problema\n";
		valid = false;
	}

	//Ahora revisa que todos los nodos esten en la permutacion
	vector<int> a(numNodes,0);
	for(group=0; group<candSol.size(); group++){
		for(i=0; i<candSol[group].size(); i++){
			a[candSol[group][i]]++;
		}
	}
	for(i=0; i<numNodes; i++){
		if(a[i] != 1){
			cout<<"Error: Vertices "<<i<<" debe estar en la solucion exactamente una vez.\n";
			valid = false;
		}
	}
	
	//Finalemente, verifique si hay colorantes ilegales: es decir, verifique que cada clase de color contenga nodos que no esten en conflicto
	for(group=0; group<candSol.size(); group++){
		for(i=0; i<candSol[group].size()-1; i++){
			for(j=i+1; j<candSol[group].size(); j++){
				if(adjacent[candSol[group][i]][candSol[group][j]]){
					cout<<"Error: Nodos "<<candSol[group][i]<<" y "<<candSol[group][j]<<" estan en el mismo grupo, pero chocan"<<endl;
					valid = false;
				}
			}
		}
	}
	if(!valid) cout<<"Esta solucion no es valida"<<endl;
}

int main(int argc, char ** argv) {

	if(argc <= 1){
		cout<<"Algoritmo Greedy para colorear grafos\n\n"
			<<"USAGE:\n"
			<<"<InputFile>     (Required. File must be in DIMACS format)\n"
			<<"-r <int>        (Random seed. DEFAULT = 1)\n"
			<<"-v              (Verbosity. If present, output is sent to screen. If -v is repeated, more output is given.)\n"
			<<"****\n";
		exit(1);
	}

	int i, verbose = 0, randomSeed = 1, numNodes, numEdges=0;
	vector<int> degree;
	vector< vector<int> > adjList;
	vector< vector<bool> > adjacent;
	numConfChecks=0;

	for (i=1; i<argc; i++){
		if (strcmp("-r", argv[i]) == 0 ){
			randomSeed = atoi(argv[++i]);
		} else if (strcmp("-v", argv[i]) == 0 ){
			verbose++;
		} else{
			//Configure el archivo de entrada, lea y cierre(formato DIMACS)
			ifstream inStream;
			inStream.open(argv[i]);
			readInputFile(inStream,numNodes,numEdges,adjacent,degree,adjList);
			inStream.close();
		}
	}

	//Establecer semilla aleatoria
	srand(randomSeed);
	
	//Verifique que no haya bordes. Si es asi, salga de inmediato
	if(numEdges <= 0){
		if(verbose>=1) cout<<"El grafo no tiene bordes. Obviamente, la solucion optima es usar un color. saliendo."<<endl;
		exit(1);
	}

	//Iniciar el temporizador
	clock_t runStart = clock();

	//Declarar las estructuras utilizadas para contener la solucion
	vector< vector<int> > candSol;
	vector<int> colNode(numNodes, INT_MIN);

	makeSolution(candSol, verbose, degree,adjList,colNode, adjacent, numNodes);

	//Detener el temporizador
	clock_t runFinish = clock();
	int duration = (int)(((runFinish-runStart)/double(CLOCKS_PER_SEC))*1000);
	
	if(verbose>=1) cout<<" COLS     CPU-TIME(ms)\tCHECKS"<<endl;
	if(verbose>=1) cout<<setw(5)<<candSol.size()<<setw(11)<<duration<<"ms\t"<<numConfChecks<<endl;
	if(verbose>=2){
		prettyPrintSolution(candSol);	
	}
	ofstream resultsLog("resultsLog.log", ios::app);
	resultsLog<<candSol.size()<<endl;
	resultsLog.close();
	
	//enviar la solucion a un archivo de texto
	ofstream solStrm;
	solStrm.open("solution.txt");
	vector<int> grp(numNodes);
	for(i=0;i<candSol.size();i++){for(int j=0;j<candSol[i].size();j++){grp[candSol[i][j]] = i;}}
	solStrm<<numNodes<<"\n";
	for(i=0;i<numNodes;i++) solStrm<<grp[i]<<"\n";
	solStrm.close();
}
