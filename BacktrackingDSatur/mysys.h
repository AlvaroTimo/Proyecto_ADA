#ifndef MYSYS
#define MYSYS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>

using namespace std;

/* informacion de tiempo */
extern long seconds,microsecs;
extern struct rusage tmp;


extern  int getrusage(int who, struct rusage *rusage); 

#include <signal.h>
extern int setrlimit(), getrlimit();

/* útil para ayudar a vincular desde archivos de salida a archivos res */
extern int getpid();



// #ifndef linux
extern void ungetc();

extern int printf();
extern int fprintf();
extern int scanf();
extern int fscanf();

extern int fgetc();

extern int fread();
extern void fflush();
extern void fclose();
extern void rewind();
// #endif


/* se requiere lo siguiente para hacer qsort llamando argumentos
silencioso */
typedef int (*compfunc)(const void *, const void *);

#endif
