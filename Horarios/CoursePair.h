#ifndef COURSEPAIR_H_
#define COURSEPAIR_H_

#include "Course.h"
#include "CoursePair.h"

using namespace std;

class CoursePair {
public:
    CoursePair(Course c1, Course c2);
    virtual ~CoursePair();
    static vector<CoursePair> getAllUniqueCoursePairs();
    Course getCourse1() const;
    Course getCourse2() const;
private:
    Course c1;
    Course c2;
};

CoursePair::CoursePair(Course c1, Course c2) :
        c1{c1}, c2{c2} {}

CoursePair::~CoursePair() {}

/* Crea entradas de matriz triangular superior, menos la diagonal, para una matriz de conflictos
  */
vector<CoursePair> CoursePair::getAllUniqueCoursePairs() {
    vector<CoursePair> uniqPairs;
    vector<Course> allCourses = Course::getAllCourses();
    // Iterar sobre todos los cursos, excepto el último
    for(size_t i{0}; i < allCourses.size()-1; i++) {
        // Empezar a iterar uno encima de i
        for(size_t j{i+1}; j < allCourses.size(); j++) {
            uniqPairs.push_back(CoursePair(allCourses[i], allCourses[j]));
        }
    }
    return uniqPairs;
}

Course CoursePair::getCourse1() const {
    return this->c1;
}
Course CoursePair::getCourse2() const {
    return this->c2;
}

#endif
