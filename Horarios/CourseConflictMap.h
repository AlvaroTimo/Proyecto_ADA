#include<vector>
#include "StudentList.h"
#include "CoursePair.h"
#include "Course.h"

#ifndef COURSECONFLICTMAP_H_
#define COURSECONFLICTMAP_H_

using namespace std;

class CourseConflictMap {
public:
    CourseConflictMap();
    virtual ~CourseConflictMap();
    void addConflict(const Course& c1, const Course& c2, Student student);
    StudentList at(const Course& c1, const Course& c2);
private:
    vector<vector<StudentList>> innerMapping;
};

/* REQUIERE QUE TODOS LOS CURSOS YA SE HAYAN INICIALIZADO
 * Inicializa la asignación de vectores a una matriz triangular superior, menos la diagonal
 */
CourseConflictMap::CourseConflictMap() {
    const size_t dim = Course::getAllCourses().size();
    for(size_t i{0}; i < dim-1; i++) {
        this->innerMapping.push_back(vector<StudentList>(dim-i-1, StudentList()));
    }
}

CourseConflictMap::~CourseConflictMap() {}

/* Agrega un conflicto al mapeo interno
 * Utiliza una matriz triangular superior, por lo que la primera enumeración del curso debe ser menor que la segunda
 */
void CourseConflictMap::addConflict(const Course& c1, const Course& c2, Student student) {
    if(c1.getEnumID() < c2.getEnumID()) {
        this->innerMapping.at(c1.getEnumID()).at(c2.getEnumID()-c1.getEnumID()-1).addStudent(student);
    } else {
        this->innerMapping.at(c2.getEnumID()).at(c1.getEnumID()-c2.getEnumID()-1).addStudent(student);
    }
}

/* Obtiene un conflicto del mapeo interno
 * Utiliza una matriz triangular superior, por lo que la primera enumeración del curso debe ser menor que la segunda
 */
StudentList CourseConflictMap::at(const Course& c1, const Course& c2) {
    if(c1.getEnumID() < c2.getEnumID()) {
        return this->innerMapping.at(c1.getEnumID()).at(c2.getEnumID()-c1.getEnumID()-1);
    } else {
        return this->innerMapping.at(c2.getEnumID()).at(c1.getEnumID()-c2.getEnumID()-1);
    }
}
#endif
