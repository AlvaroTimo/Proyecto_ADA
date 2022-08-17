# Generacion de Horarios mediante coloración de grafos

## Integrantes

* Roy Angel Choquehuanca Anconeyrna
* Juan manuel soto begazo	
* Ticona Motta Alvaro Joel
* Apaza Condori Jhon Anthony

## Definicion del problema
La generación de horarios puede ser compleja dependiendo de cuántos cursos está compuesto, esta tarea en muchos colegios demora horas e incluso días, en el presente proponemos una automatización por medio de la coloración de grafos.
Para generar un horario universitario necesitamos considerar ciertas restricciones, ya sea respecto a los horarios de disponibilidad de los profesores, la cantidad de cursos a satisfacer y las clases que se deben generar. Este problema puede ser atacado mediante la coloración de grafos y es tema central de este trabajo. 

## Objetivo General
Utilizar el método de coloración de grafos para generar horarios.

## Objetivos especificos
- Solucionar el problema mediante código escalable, sobre el cual se pueda mejorar la implementación con opciones más avanzadas en un futuro.
- Generar una gráfica del grafo mediante graphviz.
- Generar un horario factible para cierto conjunto de datos.

## Input - Grafo
A continuación se muestran las primeras líneas del archivo **graph.txt**, que se incluye. Las líneas iniciales
comienzan con el carácter "c". Estos son comentarios que le dan al usuario información textual sobre el gráfico, los cuales son ignorados por los
programas. La línea simple que comienza con "p edge" especifica que las aristas se usarán para especificar el gráfico y que contiene 11 vértices y
29 aristas. Tenga en cuenta que los vértices están etiquetados desde 1 hacia arriba. Finalmente, las líneas que comienzan con 'e' dan las aristas
del gráfico. Como resultado, hay 29 líneas que comienzan con 'e'.

## Comandos para la ejecución de los algoritmos
Al ejecutar el archivo sin argumentos, se arroja lo siguiente:
```
> g++ colgraph.cpp -o main
> ./main
Algoritmos constructivos para colorear grafos

USO:
<InputFile>     (Requerido. El archivo debe estar en formato DIMACS)
-a <int>        (elegir Algoritmo: 1 = Greedy (permutacion aleatoria de vertices) (default)
                                   2 = Greedy (grados de vertice descendente / algoritmo Welsh-Powell)
                                   3 = DSatur
                                   4 = Greedy-IS (permutacion aleatoria de vertices)
                                   5 = RLF)
-r <int>        (Random seed(semilla). DEFAULT = 1)
-v              (Info)
****
```
Ejecutar el archivo con argumentos -a -r -v:
Ejemplo: -a 3 (Representa DSatur) 
```
> g++ colgraph.cpp -o main
> ./main graph.txt -a 3 -r 1 -v
```
