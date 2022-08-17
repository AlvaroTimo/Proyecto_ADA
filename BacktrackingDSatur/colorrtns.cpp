#include "mysys.h"
#include "graph.h"
#include "colorrtns.h"
#include <fstream>

/*
Una colección de rutinas para manipular vectores.
de colores, vértices, secuencias de grados, etc.
utilizado aquí y allá en todo el sistema	
*/

extern unsigned long long numConfChecks;
extern unsigned long long maxChecks;

void printinfo( popmembertype *member)
{
	//printf("CLRS =%d\tCLRSUM = %d\n",member->clrdata.numcolors,
	//	member->clrdata.total);
}

/* colores máximos impresos por línea */
#define MAXPERLINE  20

void printcoloring( popmembertype *member)
	/*
	Imprime la secuencia de colores, en orden de vértices
	primero al ultimo
	*/
{
	colortype c[MAXVERTEX];
	int i;
	int fmt;

	for(i=0;i<order;i++) {
		c[member->vc[i].vertex] = member->vc[i].color;

	}
	fmt = 0;
	for(i=0;i<order;i++) {
		printf("%3d ",c[i]);
		fmt++;
		if ( fmt >= MAXPERLINE ) {
			printf("\n");
			fmt = 0;
		}
	}
}

void printpurity( popmembertype *member)
{
	int i,j, purity;
	int purityset[MAXVERTEX];
	vertextype v,w;
	colortype cv,cw;

	if (partitionflag) {
		purity = 0;
		for(i=0;i<=partitionnumber;i++) purityset[i] =0;
		for (i=0;i<order;i++) {
			v= member->vc[i].vertex;
			cv=member->vc[i].color;
			for(j=0;j<i;j++) {
				w= member->vc[j].vertex;
				cw=member->vc[j].color;
				if ((partset[v] == partset[w]) && (cv == cw)){
					purity++;
					purityset[partset[v]]++;
				}
			}
		}
		printf("P= %4d:",purity);
		for(i=0;i<=partitionnumber;i++)
			printf("%3d ",purityset[i]);
		printf("\n");

	}
}


void getcolorinfo( popmembertype *member)
{
	vertextype i;
	struct vrtxandclr *v;

	/* get información de color */
	member->clrdata.numcolors = 1;
	member->clrdata.total = 0;
	v = member->vc;

	for(i=0;i<order;i++) {
		if (v[i].color > member->clrdata.numcolors) 
			member->clrdata.numcolors = v[i].color;
		member->clrdata.total += v[i].color;
	}
	member->clrdata.total += member->clrdata.numcolors * order;
}

/* permuta un subrango de un miembro			*/
/* permuta los elementos en el rango [first..last-1] 	*/
/* todas las permutaciones tienen la misma probalidad	*/
void permute(
	popmembertype *member,
	vertextype first, vertextype last)
{
	vertextype i,j;
	struct vrtxandclr k;

	for(i=first; i<last-1; i++) {
		j = i + ((vertextype) rand() % (last - i));
		k = member->vc[i];
		member->vc[i] = member->vc[j];
		member->vc[j] = k;
	}
}

void trivial_color( popmembertype *m)
{
	vertextype i;
	for(i=0;i<order;i++)
		m->vc[i].color = i+1;
}

void verifycolor(popmembertype *m)
{
	char verifyset[MAXVERTEX];
	int clrflagerror;
	int i,j;

	for(i=0;i<order;i++) verifyset[i] = 0;
	clrflagerror = 0;
	for(i=0;i<order;i++) {
		verifyset[m->vc[i].vertex] = 1;
		for(j=i+1;j<order;j++) {
			numConfChecks++;
			if ((edge(m->vc[i].vertex,m->vc[j].vertex)) && 
				(m->vc[i].color == m->vc[j].color))
				clrflagerror++;
		}
	}

	if (clrflagerror > 0) printf("COLORING ERRORS %d\n",clrflagerror);
	else {
		clrflagerror = 0;
		for(i=0;i<order;i++) if (verifyset[i] == 0) clrflagerror++;
		if (clrflagerror > 0) 
			printf("ERROR: %d missing vertices\n",clrflagerror);
		else ;
			//printf("Coloring Verified\n");
	}

}

void getacoloring(popmembertype *m, char *name, int *which)
{
	int i,cnt, colnum;

	FILE *fp;

	char fname[100], oneline[100],*s;


	for(i=0;i<order;i++)
		m->vc[i].vertex = i;

	strncpy(fname,name,80);
	strncat(fname,".res",5);

	if ( (fp = fopen(fname,"r") ) == NULL) {
		printf("WARNING: getacoloring - cannot open file %s \n",
			fname);
		printf("\tUSING TRIVIAL COLORING INSTEAD\n");
		trivial_color(m);
	} else {
		cnt = 0;
		while ( !feof(fp) ) {
			s = fgets( oneline, 81 , fp);
			if (0 == strncmp(oneline, "CLRS",4)) {
				cnt++;
				printf("[ %d] %s\n",cnt, oneline);
			}
		}

		if (cnt > 0) {
			printf("Hay %d colorantes. Cual quieres? ",
				cnt);
			scanf("%d",&colnum);

			/* predeterminado tomar el ultimo*/
			if ( (colnum > cnt) || (colnum < 1) ) colnum = cnt;
			printf("%d\n",colnum);

			*which = colnum;

			rewind(fp);
			cnt = 0;
			while (cnt < colnum) {
				s = fgets( oneline, 81 , fp);
				if (0 == strncmp(oneline, "CLRS",4)) {
					cnt++;
				}
			}
			for(i=0;i<order;i++) {
				if ( 0 == fscanf(fp,"%hu",&(m->vc[i].color)) ) {
					printf("MAL FORAMTO DE ARCHIVO\n");
					exit(1);
				}
			}
		} else {
			printf("NO HAY COLORANTES PRESENTES EN EL ARCHIVO\n");
			printf("Usar colores triviales\n");
			trivial_color(m);
		}
		fclose(fp);
	}

}

void printperm(popmembertype *m)
{
	int i;
	for(i=0;i<order;i++)
		printf("%d %d\n",m->vc[i].vertex, m->vc[i].color);
}


int degseq[MAXVERTEX+1];

int decdeg( struct vrtxandclr *a, struct vrtxandclr *b)
	/*
	Rutina de compracion parac clasificar por grado hacia abajo
	*/
{
	if (degseq[a->vertex] < degseq[b->vertex]) return(1);
	else if (degseq[a->vertex] == degseq[b->vertex]) return(0);
	else return(-1);
}

int incdeg(struct vrtxandclr *a, struct vrtxandclr *b)
	/*
	Rutina de comparacion para clasificar por grado hacia arriba
	*/
{
	if (degseq[a->vertex] > degseq[b->vertex]) return(1);
	else if (degseq[a->vertex] == degseq[b->vertex]) return(0);
	else return(-1);
}

void computedeg()
	/*
	Calcular la secuencia de grados
	*/
{
	int i,j;
	adjacencytype *x;
	for(i=0;i<order;i++) degseq[i] =0;
	for(i=1;i<order;i++) {
		numConfChecks++;
		initnbr(x,i);
		for(j=0;j<i;j++){
			numConfChecks++;
			if (isnbr(x,j)) {
				degseq[i]++;
				degseq[j]++;
			}
		}
	}
}

void fileres(char *name, popmembertype *m, char *prgm)
{
	//Imprimir la solucion a un archivo
	int i;
	colortype c[MAXVERTEX];

	for(i=0;i<order;i++) {
		c[m->vc[i].vertex] = m->vc[i].color;
	}

	ofstream solStrm;
	solStrm.open("solution.txt");
	solStrm<<order<<"\n";
	for(i=0;i<order;i++) solStrm<<c[i]-1<<endl;
	solStrm.close();

}
