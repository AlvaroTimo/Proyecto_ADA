# Generacion de Horarios mediante coloración de grafos

## Integrantes

* Roy Angel Choquehuanca Anconeyra
* Juan Manuel Soto Begazo	
* Ticona Motta Alvaro Joel
* Apaza Condori Jhon Anthony

## Link Video Presentacion
[Link Aqui](https://www.youtube.com/watch?v=yYR037PGNi4&ab_channel=ALVAROJOELTICONAMOTTA)

## Definicion del problema
La generación de horarios puede ser compleja dependiendo de cuántos cursos está compuesto, esta tarea en muchos colegios demora horas e incluso días, en el presente proponemos una automatización por medio de la coloración de grafos.
Para generar un horario universitario necesitamos considerar ciertas restricciones, ya sea respecto a los horarios de disponibilidad de los profesores, la cantidad de cursos a satisfacer y las clases que se deben generar. Este problema puede ser atacado mediante la coloración de grafos y es tema central de este trabajo. 

## Objetivo General
Utilizar el método de coloración de grafos para generar horarios.

## Objetivos especificos
- Solucionar el problema mediante código escalable, sobre el cual se pueda mejorar la implementación con opciones más avanzadas en un futuro.
- Generar una gráfica del grafo mediante graphviz.
- Generar un horario factible para cierto conjunto de datos.
# Aspectos generales para la instalación
El proyecto esta escrito en el lenguaje de programación C++, no tiene ninguna limitación con respecto a la plataforma donde se realize la instalación.
Requerimiento principal:
 - C++ 11, C++14 o versiones posteriores
 
# Estructura del Proyecto
Durante el desarrollo del segundo parcial nos centramos en la busqueda de algoritmos que nos ayuden principalmente el problema de coloración de grafos, entre los cuales por ejemplo podemos encontrar las implementaciones del BacktrackingDSatur, el algortimo DSatur y el algoritmo Greedy. Todo esto se presenta en la sección algoritmos para colorear grafos.

Durante el periodo del tercer parcial nos centramos en la implementación para generar un horario a partir de los requerimientos de alumnos para llevar cursos en cierto semestre por ejempo, y en base a los cursos disponibles por ejemplo.
Es asi que la implementación se encuentra en la carpeta Horarios y su explicación se encuentra en la segunda seccción del proyecto.


# Algoritmos para colorear grafos
En este proyecto tenemos los algoritmos BacktrackingDSatur, DSatur y Greedy, adicionalmente tenemos una carpeta llamada ColGraph que contiene una "interfaz" donde podemos elegir que metodo de coloración elegir.

## Input - Grafo
A continuación se muestran las primeras líneas del archivo **graph.txt**, que se incluye. Las líneas iniciales
comienzan con el carácter "c". Estos son comentarios que le dan al usuario información textual sobre el gráfico, los cuales son ignorados por los
programas. La línea simple que comienza con "p edge" especifica que las aristas se usarán para especificar el gráfico y que contiene 11 vértices y
29 aristas. Tenga en cuenta que los vértices están etiquetados desde 1 hacia arriba. Finalmente, las líneas que comienzan con 'e' dan las aristas
del gráfico. Como resultado, hay 29 líneas que comienzan con 'e'.

## Comandos para la ejecución de los algoritmos
### Greedy
Para compilar el algoritmo primeramente ejecutamos las siguientes lienas de codigo dentro de la carpeta correspondiente, es decir, dentro de la carpeta Greedy.
```
g++ main.cpp -o main
./main
``` 
Luego de esto se mostrara una ventana donde usted puede cambiar los parametros que desee, en nuestro caso utilizaremos las mismas restricciones para el DSATUR como para el Greedy, esto con el objetivo de ver cual produce una solución con menos colores.
```
./main graph.txt -r 1 -v
```
Luego de lo cual obtendremos como respuesta en la columna COLS el resultado obtenido, siendo esta la cantidad de colores utilizada.

### DSATUR
Para compilar el algoritmo primeramente ejecutamos las siguientes lienas de codigo dentro de la carpeta correspondiente, es decir, dentro de la carpeta DSATUR.
```
g++ DSATUR.cpp -o main
./main
``` 
Introduciomos los parametros.
```
./main graph.txt -r 1 -v
```
Como podra comprobar el número de colores como resultado de DSATUR es menor a la cantidad de los colores utilizados por el Greedy.

### ColGraph
En la carpeta ColGraph tenemos una variedad de algoritmos los cuales se encuentran disponibles para ser probados.
Correr la siguiente línea de código para compilar:
```
g++ colgraph.cpp -o main
./main
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
g++ colgraph.cpp -o main
./main graph.txt -a 1 -r 1 -v
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
