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

}
