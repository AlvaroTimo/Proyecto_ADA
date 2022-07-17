
//  Este es el algoritmo de Dsatur de Brelaz
//	Basado en el libro: Lewis, R. (2015) A Guide to Graph Colouring: Algorithms and Applications. Berlin, Springer. 	


#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <string.h>
#include <limits.h>

using namespace std;

unsigned long long numConfChecks;

inline
void swap(int &a, int &b)
{
	int temp;
	temp = a; a = b; b = temp;
}


inline
void prettyPrintSolution(vector< vector<int> > &candSol)
{
	int i, count = 0, group;
	cout<<"\n";
	for(group=0; group<candSol.size(); group++){
		cout<<"Color "<<group<<" = {";
		if(candSol[group].size() == 0) cout<<"vacio}\n";
		else {
			for(i=0; i<candSol[group].size()-1; i++){
				cout << candSol[group][i]<< ", ";
			}
			cout<<candSol[group][candSol[group].size()-1]<<"}\n";
			count = count + candSol[group].size();
		}
	}
	cout<<"Numero Total de Nodos = "<<count<<endl;
}

inline
void checkSolution(vector< vector<int> > &candSol, int numItems, vector< vector<bool> > &adjacent)
{
	//Ejecuta una serie de comprobaciones para ver si "candSol" representa una solución completa, válida y sin conflictos.
	int j, i, count = 0, group;
	bool valid = true;
	
	//Primero verifique que la solución tenga el tamaño correcto
	for(group=0; group<candSol.size(); group++){
		count = count + candSol[group].size();
	}
	
	if(count != numItems){ 
		cout<<"Error: el tamaño de la solución no es igual al tamaño del problema\n";
		valid = false;
	}

	//Ahora verifica que todos los nodos estén en la permutación exactamente una vez
	vector<int> a(numItems, 0);
	for(group=0; group<candSol.size(); group++){
		for(i=0; i<candSol[group].size(); i++){
			a[candSol[group][i]]++;
		}
	}
	for(i=0; i<numItems; i++){
		if(a[i] > 1){
			cout<<"Error: Vertice "<<i<<" ocurre "<<a[i]<<" veces en la solución\n";
			valid = false;
		}
		else if(a[i] < 1){
			cout<<"Error: Vertice "<<i<<" no está presente en la solución\n";
			valid = false;
		}
	}
		
	//Finalmente, verifica que cada clase de color contenga solo nodos que no estén en conflicto
	for(group=0; group<candSol.size(); group++){
		if(!candSol[group].empty()){
			for(i=0; i<candSol[group].size()-1; i++){
				for(j=i+1; j<candSol[group].size(); j++){
					if(adjacent[candSol[group][i]][candSol[group][j]]){
						cout<<"Error: Nodos "<<candSol[group][i]<<" and "<<candSol[group][j]<<" están en el mismo grupo, pero chocan"<<endl;
						valid = false;
					}
				}
			}
		}
	}
	if(valid) cout<<"La solución es válida"<<endl;
	else cout<<"Advertencia: Solution no es valida"<<endl;
}

void readInputFile(ifstream &inStream, int &numNodes, int &numEdges, vector< vector<bool> > &adjacent, vector<int> &degree,
	vector< vector<int> > &adjList)
{
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
				cerr << "Error reading 'p' line: no 'edge' keyword found.\n";
				cerr << "'" << str << "' found instead\n";
				exit(-1);
			}
			inStream >> numNodes;
			inStream >> numEdges;
			//Configurar la matriz de adyacencia bidimensional
			adjacent.clear();
			adjacent.resize(numNodes, vector<bool>(numNodes));			
			for (i=0;i<numNodes;i++) for(j=0;j<numNodes;j++) {
				if(i==j)adjacent[i][j]=true; 
				else adjacent[i][j] = false;
			}
			blem=0;
			break;
		case 'n':
			if (blem) {
				cerr << "Found 'n' line before a 'p' line.\n";
				exit(-1);
			}
			int node;
			inStream >> node;
			if (node < 1 || node > numNodes) {
				cerr << "Node number " << node << " is out of range!\n";
				exit(-1);
			}
			node--;	
			cout << "Tags (n Lines) not implemented in g object\n";
			break;
		case 'e':
			int node1, node2;
			inStream >> node1 >> node2;
			if (node1 < 1 || node1 > numNodes || node2 < 1 || node2 > numNodes) {
				cerr << "Node " << node1 << " or " << node2 << " is out of range!\n";
				exit(-1);
			}
			node1--;
			node2--;
			if (!adjacent[node1][node2]) {
				edges++;
			} else {
				multiple++;
				if (multiple<5) {
					cerr << "Warning: in graph file at line " << line << ": edge is defined more than once.\n";
					if (multiple == 4) {
						cerr << "  No more multiple edge warnings will be issued\n";
					}
				}
			}
			adjacent[node1][node2] = true;
			adjacent[node2][node1] = true;
			break;
		case 'd':
		case 'v':
		case 'x':
			cerr << "File line " << line << ":\n";
			cerr << "'" << c << "' lines are not implemented yet...\n";
			inStream.getline(str,999,'\n');
			break;
		case 'c':
			inStream.putback('c');
			inStream.get(str,999,'\n');
			break;
		default:
			cerr << "File line " << line << ":\n";
			cerr << "'" << c << "' is an unknown line code\n";
			exit(-1);
		}
		inStream.get(); //Mata la nueva línea;
	}
	inStream.close();
	if (multiple) {
		cerr << multiple << " multiple edges encountered\n";
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

inline
bool colourIsFeasible(vector< vector<int> > &sol, int c, int v, vector< vector<bool> > &adjacent,
					vector< vector<int> > &adjList, vector<int> &colNode)
{
	//Verifica si el vértice v se puede insertar de manera factible en el color c.
	int i;
	numConfChecks++;
	if(sol[c].size() > adjList[v].size()){
		//compruebe si algún vecino de v está actualmente en color c
		for(i=0; i<adjList[v].size(); i++){
			numConfChecks++;
			if(colNode[adjList[v][i]] == c) return false;
		}
		return true;
	}
	else {
		//comprueba si algún vértice en el color c es adyacente a v
		for(i=0; i<sol[c].size(); i++){
			numConfChecks++;
			if(adjacent[v][sol[c][i]]) return false;
		}
		return true;
	}
}

inline
void assignAColourDSatur(bool &foundColour, vector< vector<int> > &candSol, vector< vector<bool> > &adjacent, 
						vector<int> &permutation, int nodePos, vector<int> &satDeg, vector<int> &degree, int numNodes,
						vector< vector<int> > &adjList, vector<int> &colNode)
{
	int i, j, c=0, v=permutation[nodePos];
	bool alreadyAdj;

	while(c < candSol.size() && !foundColour){			
		//compruebe si el color c es factible para el vértice v
		if(colourIsFeasible(candSol, c, v, adjacent, adjList, colNode)){
			//v se puede agregar a este color
			foundColour = true;
			candSol[c].push_back(v);
			colNode[v] = c;
			//Ahora necesitamos actualizar satDeg. Para hacer esto, identificamos los nodos no coloreados i que son adyacentes a
			//este nodo v recién coloreado. Si i ya está adyacente a un nodo en color c, no hacemos nada,
			//de lo contrario, su grado de saturación aumenta...
			for(i=0; i<satDeg.size(); i++){
				numConfChecks++;
				if(adjacent[v][permutation[i]]){
					alreadyAdj = false;
					j=0;
					while(j<candSol[c].size()-1 && !alreadyAdj){
						numConfChecks++;
						if(adjacent[candSol[c][j]][permutation[i]]) alreadyAdj = true;
						j++;
					}
					if (!alreadyAdj)
						satDeg[i]++;
				}
			}
		}
		c++;
	}
}

inline
void DSaturCol(vector< vector<int> > &candSol, vector< vector<bool> > &adjacent, vector<int> &degree, int numNodes,
				vector< vector<int> > &adjList)
{
	int i, j, r;
	bool foundColour;
	
	//Haz un vector que represente todos los nodos.
	vector<int> permutation(numNodes);
	for (i=0;i<numNodes;i++)permutation[i]=i;
	//Permuta aleatoriamente los nodos y luego los organiza por orden creciente de grado
	//(esto permite más de 1 resultado posible del procedimiento de clasificación)
	for(i=permutation.size()-1; i>=0; i--){
		r = rand()%(i+1);
		swap(permutation[i],permutation[r]);
	}
	////Aquí se utiliza la ordenación de burbujas. Esto podría hacerse más eficiente.
	for(i=(permutation.size()-1); i>=0; i--){
		for(j=1; j<=i; j++){
			numConfChecks+=2;
			if(degree[permutation[j-1]] > degree[permutation[j]]){
				swap(permutation[j-1],permutation[j]);
			}
		}
	}
	
	//También tenemos un vector para contener los grados de saturación de cada nodo
	vector<int> satDeg(permutation.size(), 0);

	//También tenga un vector que contenga los colores de cada nodo.
	vector<int> colNode(numNodes,INT_MIN);

	//Inicializar candSol
	candSol.clear();
	candSol.push_back(vector<int>());
	
	//Colorea primero el nodo más a la derecha (tiene el grado más alto) y lo quita de la permutación
	candSol[0].push_back(permutation.back());
	colNode[permutation.back()] = 0;
	permutation.pop_back();
	//y actualiza la matriz de grados de saturación
	satDeg.pop_back();
	for(i=0; i<satDeg.size(); i++){
		numConfChecks++;
		if(adjacent[candSol[0][0]][permutation[i]]){
			satDeg[i]++;
		}
	}
	
	//Ahora colorea los nodos restantes.
	int nodePos = 0, maxSat;
	while(!permutation.empty()){
		//elija el nodo para colorear a continuación (el nodo más a la derecha que tiene el máximo satDegree)
		maxSat = INT_MIN;
		for(i=0; i<satDeg.size(); i++){
			if(satDeg[i] >= maxSat){
				maxSat = satDeg[i];
				nodePos = i;
			}
		}
		//ahora elija qué color asignar al nodo
		foundColour = false;
		assignAColourDSatur(foundColour, candSol, adjacent, permutation, nodePos, satDeg, degree, numNodes, adjList, colNode);
		if(!foundColour){	
			//Si estamos aquí tenemos que hacer un nuevo color ya que hemos probado todos los demás y ninguno es adecuado
			candSol.push_back(vector<int>());
			candSol.back().push_back(permutation[nodePos]);
			colNode[permutation[nodePos]] = candSol.size()-1;
			//Actualizamos la matriz de grados de saturación
			for(i=0; i<permutation.size(); i++){
				numConfChecks++;
				if(adjacent[permutation[nodePos]][permutation[i]]){
					satDeg[i]++;
				}
			}
		}
		//Finalmente, eliminamos el nodo de la permutación.
		permutation.erase(permutation.begin() + nodePos);
		satDeg.erase(satDeg.begin() + nodePos);
	}
}


int main(int argc, char ** argv){

	if(argc <= 1){
		cout<<"Algoritmo DSatur para colorear gráficos\n\n"
		<<"USO:\n"
		<<"<InputFile> (Obligatorio. El archivo debe estar en formato DIMACS)\n"
		<<"-r <int>        (Semilla aleatoria. PREDETERMINADO = 1)\n"
		<<"-v              (Verbosidad. Si está presente, la salida se envía a la pantalla. Si se repite -v, se da más salida.)\n"
		<<"****\n";
		exit(1);
	}
	
	//Variables
	vector< vector<bool> > adjacent;
	vector<int> degree;
	vector< vector<int> > adjList;
	int numNodes, numEdges=0, seed=1, duration, verbose = 0, i;
	numConfChecks = 0;
		  
	for(i=1; i<argc; i++){
		if (strcmp("-r", argv[i]) == 0 ) {
			seed = atoi(argv[++i]);
		} else if (strcmp("-v", argv[i]) == 0 ) {
			verbose++;
		} else {
			//Configuramos el archivo de entrada, lea y cierre (la entrada debe estar en formato DIMACS)
			ifstream inStream;
			inStream.open(argv[i]);
			readInputFile(inStream,numNodes,numEdges,adjacent,degree,adjList);
			inStream.close();
		}
	}

	//Establecer semilla aleatoria
	srand(seed);
	
	//Haz una comprobación para ver si tenemos el gráfico vacío. Si es así, termine de inmediato.

	if(numEdges <= 0){
		if(verbose>=1) cout<<"El gráfico no tiene bordes. Obviamente, la solución óptima es usar un color. saliendo."<<endl;
		exit(1);
	}
	

	//Iniciar el temporizador
	clock_t runStart = clock();
	
	//Ahora forme una solución llamada candSol (solución candidata)
	vector< vector<int> > candSol;
	DSaturCol(candSol, adjacent, degree, numNodes, adjList);
	
	//detener el temporizador.
	clock_t runFinish = clock();
	duration = (int)(((runFinish-runStart)/double(CLOCKS_PER_SEC))*1000);
	
	//Producir alguna salida
	if(verbose>=1) cout<<" COLS CPU-TIME(ms) \t COMPROBACIONES"<<endl;
	if(verbose>=1) cout<<setw(5)<<candSol.size()<<setw(11)<<duration<<"ms\t"<<numConfChecks<<endl;
	if(verbose>=2){
		prettyPrintSolution(candSol);	
	}
	
	//enviar la solución a un archivo de texto
	ofstream solStrm;
	solStrm.open("solution.txt");
	vector<int> grp(numNodes);
	for(i=0;i<candSol.size();i++){for(int j=0;j<candSol[i].size();j++){grp[candSol[i][j]] = i;}}
	solStrm<<numNodes<<"\n";
	for(i=0;i<numNodes;i++) solStrm<<grp[i]<<"\n";
	solStrm.close();

	ofstream resultsLog("resultsLog.log", ios::app);
	resultsLog<<candSol.size()<<endl;
	resultsLog.close();

}