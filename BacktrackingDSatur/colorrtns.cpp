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
