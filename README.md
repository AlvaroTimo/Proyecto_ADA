# Generacion de Horarios mediante coloración de grafos

## Integrantes

* Roy Angel Choquehuanca Anconeyra
* Juan Manuel Soto Begazo	
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

# Estructura del Proyecto
Durante el desarrollo del segundo parcial nos centramos en la busqueda de algoritmos que nos ayuden principalmente el problema de coloración de grafos, entre los cuales por ejemplo podemos encontrar las implementaciones del BacktrackingDSatur, el algortimo DSatur y el algoritmo Greedy. Todo esto se presenta en la sección algoritmos para colorear grafos.

Durante el periodo del tercer parcial nos centramos en la implementación para generar un horario a partir de los requerimientos de alumnos para llevar cursos en cierto semestre por ejempo, y en base a los cursos disponibles por ejemplo.
Es asi que la implementación se encuentra en la carpeta Horarios y su explicación se encuentra en la segunda seccción del proyecto.


# Algoritmos para colorear grafos
## Input - Grafo
A continuación se muestran las primeras líneas del archivo **graph.txt**, que se incluye. Las líneas iniciales
comienzan con el carácter "c". Estos son comentarios que le dan al usuario información textual sobre el gráfico, los cuales son ignorados por los
programas. La línea simple que comienza con "p edge" especifica que las aristas se usarán para especificar el gráfico y que contiene 11 vértices y
29 aristas. Tenga en cuenta que los vértices están etiquetados desde 1 hacia arriba. Finalmente, las líneas que comienzan con 'e' dan las aristas
del gráfico. Como resultado, hay 29 líneas que comienzan con 'e'.

## Comandos para la ejecución de los algoritmos
```
> g++ colgraph.cpp -o main
> ./main
```
Al ejecutar el archivo sin argumentos, se arroja lo siguiente:
```
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
Ejecutar el archivo con argumentos -a -r -v
-Ejemplo: -a 1 (Representa 1=Greedy) 
```
> g++ colgraph.cpp -o main
> ./main graph.txt -a 1 -r 1 -v
```
```
 COLS     CPU-TIME(ms)  CHECKS
    5          0ms      116

C-0     = {1, 6, 10, 11}
C-1     = {2, 3, 9}
C-2     = {7, 8}
C-3     = {4}
C-4     = {5}
Numero total de nodos = 11

Greedy  5       0       116

lunes: 1 6 10 11
martes: 2 3 9
miercoles: 7 8
jueves: 4
viernes: 5 

```
# Implementación para generar horarios
Para la generación de horarios debemos centrarnos en la carpeta **Horarios**, en la cual se encuentra la implementación para la generación de horarios.
Dentro de esta carpeta podemos encontrar la sección Inputs en la cual se presenta un pequenio test para comprobar la funcionalidad de este.
Esta implementación esta basada en el algoritmo Greedy y en trabajos futuros nos gustaria implementar otros algortimos adicionalmente. 

### Inputs
Dentro de esta carptea tenemos dos archivos, el archivo **Clases.csv**  nos indica los cursos que estan disponibles en una jornada normal, estos son nuestros Nodos en el grafo. El archivo **Requerimientos.csv** nos indica que cursos solicita cada estudiante, estos funcionan como aristas y nos limitan las formas de generar el horario.

### Outputs
La carpeta Outputs solo contiene un fichero vacio, cuando se ejecuta el archivo **main.cpp** ubicado en la carpeta **Horarios** se generan automaticamente los outputs.
Despues de la ejecución, aparecen 4 archivos, 2 del tipo *.cvs* : 
- *AlumnosAsignadosClases.csv* 
- *MatrizConlictos.csv* 

y 2 del tipo *.txt*:
- *Conflictos.txt* 
- *Schedule.txt*

## Generación de Horarios
Dirigirse a la carpeta **Horarios** y luego ejecutar el archivo **main.cpp**.

# Trabajos futuros
- Implementar un algoritmo genético para la generación de horarios.
- Implementar una interfaz gráfica con ayuda de QT
- Comparar los resultados obtenidos con el método Greedy frente a un algoritmo genético.
