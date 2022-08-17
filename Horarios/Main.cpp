#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include "Course.h"
#include "CourseConflictMap.h"
#include "CoursePair.h"
#include "Student.h"
#include "StudentList.h"

using namespace std;

// Funcion Auxiliar
/* Tokeniza una linea de archivo CSV, donde las columnas estan separadas por comas, las filas por saltos de linea y las comillas encapsulan el texto directamente*/
vector<string> tokenizeFile(istream& stream) {
    vector<string> tokens;
    string str;
    // Leer hasta la proxima instancia de "
    while(getline(stream, str, '"')) {
        stringstream strStream{str};
        // Leer desde strStream hasta la primera coma
        while(getline(strStream, str, ',')) {
            // Push token
            tokens.push_back(str);
        }
        // Si se encuentra una cotizacion de cierre (es decir, no EOF)
        if(getline(stream, str, '"')) {
            // Push token
            tokens.push_back(str);
            if(',' == stream.peek()) {
                getline(stream, str, ',');
            }
        }
    }
    return tokens;
}

int main() {
    // TEST
    string classesFile = "Inputs/Clases.csv";
    string studentsFile = "Inputs/Requerimientos.csv";

    string dummyStr;

    // Inicializar el input para clases
    ifstream courseInStream{classesFile, ios::in};
    if(!courseInStream) {
        cerr << "ERROR: El archivo \"" << classesFile << "\" no se pudo abrir." << endl;
        exit(EXIT_FAILURE);
    }
    // Leer cabecera de clases
    getline(courseInStream, dummyStr);
    string str;
    // Cree todos los courses (vector completo retenido como miembro estatico de la clase del curso)
    while(getline(courseInStream, str)) {
        stringstream strStream{str};
        vector<string> tokens = tokenizeFile(strStream);
        if(tokens.size() < 3) {
            cout << "El archivo \"" << classesFile << "\" tiene una linea incompleta. Esta sera ignorada." << endl;
            continue;
        }
        string code = tokens.at(0);
        string title = tokens.at(1);
        string prof = tokens.at(2);
        Course{code, title, prof};
    }

    ifstream studentInStream{studentsFile, ios::in};
    if(!studentInStream) {
        cerr << "ERROR: El archivo \"" << studentsFile << "\" no se pudo abrir." << endl;
        exit(EXIT_FAILURE);
    }

    vector<Student> students;
    getline(studentInStream, dummyStr);
    // Construir todos los estudiantes y agregarlos al vector local
    // Tambien crea listas (vector de StudentList) para cada curso
    vector<StudentList> rosters = vector<StudentList>(Course::getAllCourses().size(), StudentList());
    while(getline(studentInStream, str)) {
        stringstream strStream{str};
        vector<string> tokens = tokenizeFile(strStream);
        if(tokens.size() < 3) {
            cout << "El archivo \"" << studentsFile << "\" tiene una linea incompleta. Esta sera ignorada." << endl;
            continue;
        }
        string cui = tokens.at(0);
        string name = tokens.at(1);
        vector<Course> curStudCourses;
        for(size_t i{2}; i < tokens.size(); i++) {
            Course c{Course::getCourseByCode(tokens.at(i))};
            if(c.isValid()) {
                curStudCourses.push_back(Course::getCourseByCode(tokens.at(i)));
            } else {
                cout << "Estudiante \"" << name << "\" requiere el curso \"" << tokens.at(i) << "\", pero este no se encuentra disponible. Peticion Ignorada." << endl;
            }
        }
        Student curStudent{cui, name, curStudCourses};
        students.push_back(curStudent);
        for(const Course& c : curStudCourses) {
            // Proteja contra courses adicionales agregados por estudiantes e indexacion mas alla del tamaño del vector
            while (c.getEnumID() >= rosters.size()) {
                rosters.push_back(StudentList());
            }
            rosters.at(c.getEnumID()).addStudent(curStudent);
        }
    }

    // SOLO CREAR ESTO CUANDO LOS courses ESTeN COMPLETAMENTE INICIALES
    CourseConflictMap conflicts;
    // Para todos los estudiantes, agregue todos sus conflictos al objeto de conflictos
    for(const Student& student : students) {
        for(const CoursePair& cpair : student.getCourseConflicts()) {
            conflicts.addConflict(cpair.getCourse1(), cpair.getCourse2(), student);
        }
    }

    // Agregar bordes a la matriz de conflicto para conflictos de profesores
    CourseConflictMap conflictsProfs{conflicts};
    for(size_t i{0}; i < Course::getAllCourses().size(); i++) {
        for(size_t j{i+1}; j < Course::getAllCourses().size(); j++) {
            Course c1{Course::getCourseByEnumID(i)};
            Course c2{Course::getCourseByEnumID(j)};
            if(c1.getProf() == c2.getProf()) {
                conflictsProfs.addConflict(c1, c2, Student{"N/A", c1.getProf(), vector<Course>()});
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////GREDDY ALGORITHM ////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Construir todos los timeslots de los courses
    // En efecto, cada curso pasa por todos los timeslots existentes actualmente y verifica si el curso puede ir en cada uno.
    // Una vez que el curso encuentra un timeslot al que puede unirse, el curso se une al timeslot.
    // Si el curso nunca encuentra ese timeslot, el curso se colocara en un nuevo timeslot
    vector<vector<Course>> timeslots;
    for(Course c : Course::getAllCourses()) {
        bool hasBeenInserted = false;
        for(vector<Course>& curBlock : timeslots) {
            // Si el timeslot esta lleno, saltelo
            if(curBlock.size() >= 6) {
                continue;
            }
            // Si el curso ya ha encontrado un timeslot, deje de buscar un timeslot
            if(hasBeenInserted) {
                break;
            }
            // Si algun curso actualmente en el timeslot entra en conflicto con este nuevo curso, interrumpa e intente con el siguiente timeslot
            bool canInsertCurCourse = true;
            for(Course c2 : curBlock) {
                if(conflictsProfs.at(c, c2).getSize() != 0) {
                    canInsertCurCourse = false;
                    break;
                }
            }
            // Si ningun curso actualmente en el timeslot entra en conflicto con este nuevo curso, agregue el nuevo curso al timeslot
            if(canInsertCurCourse) {
                curBlock.push_back(c);
                hasBeenInserted = true;
            }
        }
        // Si ningun timeslot existente actualmente puede contener este nuevo curso, asignele su propio timeslot
        if(!hasBeenInserted) {
            vector<Course> newBlock(1, c);
            timeslots.push_back(newBlock);
        }
    }

    ofstream conflMatFile{"Outputs/MatrizConlictos.csv", ios::out};
    if(!conflMatFile) {
        cerr << "Error al intentar crear el archivo MatrizConlictos.csv" << endl;
        exit(EXIT_FAILURE);
    }

    // Escribir el encabezado del archivo
    for(const Course& c1 : Course::getAllCourses()) {
        conflMatFile << "," << c1.getCode();
    }
    conflMatFile << endl;
    for(const Course& c1 : Course::getAllCourses()) {
        // Escribir las entradas de la columna lateral
        conflMatFile << c1.getCode();
        for(const Course& c2 : Course::getAllCourses()) {
            if(c1.getEnumID() == c2.getEnumID()) {
                // Deja la celda 0 para conflictos imposibles
                conflMatFile << ",0";
                continue;
            }
            // Muestra el conteo de conflictos
            conflMatFile << "," << conflicts.at(c1, c2).getSize();
        }
        conflMatFile << endl;
    }

    ofstream whoHasConflFile{"Outputs/Conflictos.txt", ios::out};
    if(!whoHasConflFile) {
        cerr << "Error al intentar crear el archivo OutputConflictos.txt" << endl;
        exit(EXIT_FAILURE);
    }

    // Imprime el encabezado al archivo
    whoHasConflFile << "Lista de pares de clases y estudiantes con conflictos para ese par de clases:\n";
    for(const CoursePair& cpair : CoursePair::getAllUniqueCoursePairs()) {
        // Imprime el titulo del par del curso actual
        whoHasConflFile << "Para la pareja del cursos: " << cpair.getCourse1().getCode()
                << " : " << cpair.getCourse2().getCode() << endl;
        // Escriba el nombre de cada estudiante debajo del titulo
        for(const Student& student : conflicts.at(cpair.getCourse1(), cpair.getCourse2()).getInnerVec()) {
            whoHasConflFile << student.getName() << endl;
        }
        // Regiones separadas con un caracter de nueva linea adicional
        whoHasConflFile << endl;
    }

    ofstream scheduleFile{"Outputs/Schedule.txt", ios::out};
    if(!scheduleFile) {
        cerr << "Error al intentar crear el archivo Schedule.txt" << endl;
        exit(EXIT_FAILURE);
    }

    // Imprime problemas de timeslots en la consola
    if(timeslots.size() > 24) {
        cout << "No hay suficiente tiempo en un día para tantas clases" << endl;
        cout << "Es necesario sacar a algunas clases, conflictos, o buscar más aulas" << endl;
    } else if(timeslots.size() > 16) {
        cout << "Las clases se extenderán pasada la medianoche. Eso podría molestar a algunos estudiantes." << endl;
    } else if(timeslots.size() > 14) {
        cout << "Las clases se desarrollarán pasadas las 22:00 horas. Eso podría molestar a algunos estudiantes.." << endl;
    }

    size_t blockHour{8};
    for(const vector<Course>& curBlock : timeslots) {
        // Imprime la hora del bloque
        scheduleFile << (blockHour++ % 24) << ":00";
        // Imprimir todos los courses en esta hora de bloque
        for(const Course& c : curBlock) {
            scheduleFile << " " << c.getCode();
        }
        scheduleFile << endl;
    }

    ofstream classRollsFile{"Outputs/AlumnosAsignadosClases.csv", ios::out};
    if(!classRollsFile) {
        cerr << "Error al intentar crear el archivo AlumnosAsignadosClases.csv" << endl;
        exit(EXIT_FAILURE);
    }

    for(size_t i{0}; i < rosters.size(); i++) {
        // Imprime el nombre del curso
        classRollsFile << Course::getCourseByEnumID(i).getCode() << ":" << endl;
        // Imprimir todos los estudiantes en el curso, entre comillas para mantener una sola columna
        for(const Student& curStud : rosters.at(i).getInnerVec()) {
            classRollsFile << "\"" << curStud.getName() << "\"" << endl;
        }
        classRollsFile << endl;
    }
}