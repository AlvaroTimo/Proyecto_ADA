#ifndef COLORRTNDEF
#define COLORRTNDEF

#include "graph.h"

/* ESTRUCTURAS DE ALMACENAMIENTO DE COLOR */


typedef unsigned short colortype;

struct clrinfo {
	colortype numcolors; /* numero de colores utilizados */
	int     total;  /* suma de colores (peso) usado */
};
typedef struct clrinfo  clrinfotype;

struct vrtxandclr {
	vertextype vertex;
	colortype color;
};
typedef struct vrtxandclr vrtxandclrtype;

struct popmember {
	clrinfotype clrdata;
	vrtxandclrtype vc[MAXVERTEX];
};
typedef struct popmember popmembertype;

/* RUTINAS DE MANIPUCACION DEL COLOR */
extern void printinfo( popmembertype *member);
/* imprimir el numero de colores y la suma de colores */

extern void printcoloring( popmembertype *member);
/* imprime los colores de los vertices en orden */

extern void printpurity( popmembertype *member);
/* el calculo de pureza para la cercania al color oculto */

extern void getcolorinfo( popmembertype *member);
/* calcula el maximo y la suma de colores en el miembro */

extern void permute( popmembertype *member, vertextype first, vertextype last);
/* permutar aleatoriamente el orden de los vertices de los miembros en
el rango de primero a ultimo -1 */

extern void trivial_color( popmembertype *m);
/* aplicar los colores 1 en orden a los vertices de m, cualquiera que 
sea el orden en que estan */

extern void verifycolor(popmembertype *m);
/* verificar la coloracion de m en cuanto a la correcion y la impresion
y mensaje apropiado */

/* imprimir un miembro de permutacion principalmente para depurar*/
extern void printperm(popmembertype *m);

extern void getacoloring(popmembertype *m, char *name, int *which);
/* abre el archivo indicado y obten el coloreado solicitado por el usuario */

/* PROPIEDADES DEL GRAFO*/

extern int degseq[];

extern int decdeg( vrtxandclrtype *a, vrtxandclrtype *b);
/* rutina de comparacion para ordenacion decreciente por grado*/

extern int incdeg(vrtxandclrtype *a, vrtxandclrtype *b);
/* rutina de comparacion para aumentar la ordenacion por grado */

extern void computedeg();
/* calcular la secuencia de grados.  */

/* SALIDA FINAL AL ARCHIVO DE RESULTADOS */
extern void fileres(char *name, popmembertype *m, char *prgm);
/* el nombre se agregara con .res, los datos de color se agregaran a 
el nombre del archivo.res , prgm es el nombre del programa */

extern void about(char *pgrmname);
#endif
