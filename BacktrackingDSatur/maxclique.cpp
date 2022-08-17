#include "mysys.h"
#include <math.h>
#include <stdlib.h>
#include "colorrtns.h"
#include "maxclique.h"

#define MAXIS 500
#define MAXCUT 100
#define isnotnbr(x,i)   !(((x)[(i) >> SHIFT]) & (1 << ((i) & MASK)))

extern unsigned long long numConfChecks;


typedef struct isinfo {
	vertextype	possible, /*vertices no incidente a ninguno en IS */
	degree;   /*grado de vertice*/
} istype;


typedef istype twoDarray[MAXIS][MAXVERTEX];
typedef twoDarray *twoDarrayp;

/* globales */
int degree[MAXVERTEX];

int middeg;

int udgcmp( istype *a, istype *b)
	/*
	Rutina de comparacion para clasificar por grado hacia abajo
	*/
{
	if (a->degree < b->degree) return(1);
	else if (a->degree == b->degree) return(0);
	else return(-1);
}

int dgcmp( istype *a, istype *b)
	/*
	Rutina de comparacion para ordenar por grado
	*/
{
	if (a->degree > b->degree) return(1);
	else if (a->degree == b->degree) return(0);
	else return(-1);
}

int midcmp( istype *a, istype *b)
	/*
	rutina de compraracion para ordenar los valores medios al frente
	*/
{
	if (abs(middeg - (int) (a->degree)) > abs(middeg - (int) (b->degree)))
		return(1);
	else if (a->degree == b->degree) return(0);
	else    return(-1);
}

/*
hallazgo por fuerza bruta
usando la funcion indset() de maxis, por lo que los nombres de las variables 
son no necesariamente apropiado
*/
void clique(
	int	freesize,	/* numero de vertices restantes*/
	int	*retsize,	/* tamaño del mejor conjunto independiente encontrado*/
	vertextype bestset[],	/* conjunto independiente devuelto */
	vertextype okaylist[], 	/* lista de vertices con los que empezamos */
	int	cutlimit[],
	int	cutvertex[],
	int	limit,
	int	usort,
	int	msort)
{
	/*
	Una reesciruta completa del programa para selccionar la camarilla maxima:
	Estrutura de datos
	*/
	vertextype curset[MAXIS]; /* conjunto inpendiente actual */
	twoDarrayp indsetinf;

	istype *nptr, *pptr;
	adjacencytype  *x,*y; /* acelerar punteros*/

	int nextv[MAXIS]; /* puntero al siguiente vertice para probar en lo posible */
	int numposs[MAXIS]; /* numero en los posible */

	int degseq[MAXVERTEX];

	int nextis; /* puntero de pila para maldecir */
	int bestis, bestdeg; /* el tamaño cuenta para el mejor conjunto */
	int lcldeg; /* para calcular el grado de IS */

	int i,j,k; /* para controles de bucle etc.*/
	int Prev; /* temporal para la velocidad */

	int firsttime; /* control de profundidad */
	int cutoff[MAXCUT];
	int usortlimit,msortlimit;

	int degtot;

	/* coeficientes para tasas de corte cuadraticas */

	indsetinf = (twoDarrayp) malloc (sizeof(twoDarray));
	if (indsetinf == NULL) printf("ERROR: indset: not enough memory.\n");

	/* inicializar 0th al conjunto inicial de vertices*/
	numposs[0] = freesize;
	pptr = (*indsetinf)[0];
	for(i=0;i<freesize;i++) {
		pptr[i].possible = okaylist[i];
		pptr[i].degree = 0;
		numConfChecks++;
		initnbr(x,pptr[i].possible);
		for(k=0;k<i;k++)
			if (isnotnbr(x,(pptr[k].possible))) {
				pptr[i].degree++;
				pptr[k].degree++;
			}
	}	
	nextv[0] = 0;

	usortlimit = (freesize * usort) /100;
	msortlimit = (freesize * msort) /100;
#ifdef DBG
	printf("freesize = %d, usortlimit = %d, msortlimit = %d\n",
		freesize,usortlimit, msortlimit);
#endif
	k=0;
	while (cutvertex[k] > numposs[0]) k++;
	if (cutlimit[k] < numposs[0]) 
		cutoff[0] = cutlimit[k];
	else	cutoff[0] = numposs[0];

	/* set secuencia de grados */
	degtot = 0;
	for(i=0;i<freesize;i++) {
		degseq[pptr[i].possible] = pptr[i].degree;
		degtot += pptr[i].degree;
	}

	if (freesize >=1) middeg = degtot / freesize;
	else middeg = degtot;

	/* ordenar medios al frente */
	if (numposs[0] >= usortlimit) {
#ifdef DBG
		printf("Primera clasificacion grado decreciente\n");
#endif
		qsort((char *) pptr, (int) numposs[0], sizeof(istype), 
			(compfunc)udgcmp);
	} else if (numposs[0] >= msortlimit) {
#ifdef DBG
		printf("Primera clasificacion grado medio\n");
#endif
		qsort((char *) pptr, (int) numposs[0], sizeof(istype), 
			(compfunc)midcmp);
	}
	else {
#ifdef DBG
		printf("Primera clasificacion grado creciente\n");
#endif
		qsort((char *) pptr, (int) numposs[0], sizeof(istype), 
			(compfunc)dgcmp);
	}


#ifdef DSEQ
	printf("FIRST SORT\n");
	printf("numposs[0] = %d\n",numposs[0]);
	for(i=0;i<numposs[0];i++)
		printf("(%d,%d) ",(*indsetinf)[0][i].possible,
		(*indsetinf)[0][i].degree);
	printf("\n");
	fflush(stdout);
#endif

	bestis = 0;
	bestdeg = 0;
	nextis = 1;
	firsttime = 1; 
	while ((nextis > limit) || firsttime) { 
		if (nextis >= limit) firsttime = 0;
		/* seleccionar el siguiente vertice*/
		Prev = nextis -1;

#ifdef DBG
		printf("deglim of Prev = %d\n",cutoff[Prev]);
#endif

		if  ( nextv[Prev] >= cutoff[Prev] ) {
			/* BACKTRACK */
			nextis--;
		}
		else if (bestis > (Prev+(numposs[Prev]-nextv[Prev]))) {
			/* RETROCESO LIMITE -hay muy pocos vertices
			izquierda para consturir un mejor conjunto
			esto es mas util en k-colorable Graph
			*/
			nextis--;
		}
		else { 
			/* usar algunas variables de acelacion*/
			nptr = (*indsetinf)[nextis];
			pptr = (*indsetinf)[Prev];

			/* seleccione el siguiente vertice */
			curset[nextis] = pptr[nextv[Prev]].possible;
			numConfChecks++;
			initnbr(x,(curset[nextis]));

			/* restrablecer Anterior siguiente */
			nextv[Prev]++;

			/* crear la lista posible*/
			nextv[nextis] = 0;
			j=0;
			/* nota: consideramos solo los veritces restantes
			de Lista anterior posible */
			for(i=nextv[Prev];i<numposs[Prev];i++) {
				if ( !(isnotnbr(x,(pptr[i].possible))) ) {
					nptr[j].possible = pptr[i].possible;
					nptr[j].degree = 0;
					numConfChecks++;
					initnbr(y, (nptr[j].possible));
					for(k=0;k<j;k++) 
						if (isnotnbr(y,(nptr[k].possible))) {
							nptr[k].degree++;
							nptr[j].degree++;
						}
						j++;
				}
			}
			numposs[nextis]=j;
			degtot = 0; /* mindeg = order; */
			for(i=0;i<j;i++) {
				degtot += nptr[i].degree;
			}
			if (j > 0) middeg = degtot / j;
			else middeg = degtot;

			if (numposs[nextis] >= usortlimit) {
#ifdef DBG
				printf("Clasificación de grado decreciente\n");
#endif
				qsort((char *) nptr, (int) numposs[nextis], 
					sizeof(istype), (compfunc)udgcmp);
			} else if (numposs[nextis] >= msortlimit) {
#ifdef DBG
				printf("Clasificación de grado medio\n");
#endif
				qsort((char *) nptr, (int) numposs[nextis], 
					sizeof(istype), (compfunc)midcmp);
			} else {
#ifdef DBG
				printf("Clasificación de grado creciente\n");
#endif
				qsort((char *) nptr, (int) numposs[nextis], 
					sizeof(istype), (compfunc)dgcmp); 
			}

#ifdef DSEQ
			printf("INTERNAL SORT nextis=%d\n",nextis);
			printf("numposs[nextis] = %d\n",numposs[nextis]);
			for(i=0;i<numposs[nextis];i++)
				printf("(%d,%d) ",(*indsetinf)[nextis][i].possible,
				(*indsetinf)[nextis][i].degree);
			printf("\n");
			fflush(stdout);
#endif

			k=0;
			while(cutvertex[k] > numposs[nextis]) k++;
			if (cutlimit[k] < numposs[nextis])
				cutoff[nextis] = cutlimit[k];
			else cutoff[nextis] = numposs[nextis];

#ifdef DBG
			printf("numposs[%d] = %d\n",nextis,numposs[nextis]);
			if (DBG & 2) {
				for(i=0;i<numposs[nextis];i++)
					printf("(%d,%d) ",(*indsetinf)[nextis][i].possible,
					(*indsetinf)[nextis][i].degree);
				printf("\n");
			}
			fflush(stdout);
#endif


			/* Lleva un registro de los mejores hasta ahora */
			if (bestis < nextis ) {
				/* copiar el conjunto */
				bestdeg = 0;
				for(i=1;i<=nextis;i++) {
					bestset[i] = curset[i];
					bestdeg += degseq[curset[i]];
				}
				bestis = nextis;
			}
			else if (bestis == nextis) {
				/* grado de cómputo */
				lcldeg =0;
				for(i=1;i<=nextis;i++)
					lcldeg += degseq[curset[i]];
				if (bestdeg < lcldeg){
					for(i=1;i<=nextis;i++)
						bestset[i] = curset[i];
					bestdeg = lcldeg;
				}
			}


			/* próxima iteración */
			nextis++;
		}
	}
	*retsize = bestis;
	free(indsetinf);
}

//static	int limit,usort,msort;
//static	int cutlimit[MAXCUT], cutvertex[MAXCUT]; /* branch & bound values */

int maxclique( popmembertype *m)
{
	int i,j;
	//vertextype okaylist[MAXVERTEX];
	//vertextype bc[MAXIS];
	int bcindex;
	int tmp, rnd;

	istype info[MAXVERTEX];

	int updown;

	updown = 0;
	
	/* calcular grados*/
	for (i=0;i<order;i++) {
		info[i].degree=0;
		info[i].possible=i;
	}

	for (i=1;i<order;i++){
		for (j=0;j<i;j++){
			numConfChecks++;
			if(edge(i,j)) {
				info[i].degree++;
				info[j].degree++;
			}
		}
	}
	bcindex=1;

	//Permute aleatoriamente la matriz "info" antes de que se ordene de acuerdo con
	//la licenciatura. Esto significa que diferentes semillas pueden dar diferentes ordenaciones de los nodos. 
	for(i=order-1; i>=0; i--){
		rnd = rand()%(i+1);
		//Ahora intercambie las partes relevantes de información
		tmp = info[i].degree; 	info[i].degree = info[rnd].degree; 		info[rnd].degree = tmp;
		tmp = info[i].possible; info[i].possible = info[rnd].possible;	info[rnd].possible = tmp;
	}
	
	/* ordenar vértices por grado*/
	if (updown==0)  /* decreasing: THIS IS THE ON */
		qsort( info,order,sizeof(istype), (compfunc)udgcmp);
	else            /* increasing */
		qsort( info,order,sizeof(istype), (compfunc)dgcmp);

	for (i=0;i<order;i++) m->vc[i].vertex = info[i].possible;

	return(bcindex);
}
