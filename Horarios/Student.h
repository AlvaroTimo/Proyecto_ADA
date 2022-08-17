#ifndef STUDENT_H_
#define STUDENT_H_

#include<string>
#include<vector>
#include "Course.h"
#include "CoursePair.h"

using namespace std;

class Student {
public:
    Student(string gtid, string name, vector<Course> courseRequests);
    virtual ~Student();
    string getGTID() const;
    string getName() const;
    vector<Course> getCourseRequests() const;
    vector<CoursePair> getCourseConflicts() const;
private:
    string gtid;
    string name;
    vector<Course> courseRequests;
};


Student::Student(string gtid, string name, vector<Course> courseRequests) :
        gtid{gtid}, name{name}, courseRequests{courseRequests} {}

Student::~Student() {}

string Student::getGTID() const {
    return this->gtid;
}

string Student::getName() const {
    return this->name;
}

vector<Course> Student::getCourseRequests() const {
    return this->courseRequests;
}

/* Devuelve todos los conflictos que este alumno pueda tener */
vector<CoursePair> Student::getCourseConflicts() const {
    vector<CoursePair> conflicts;
    // Iterar sobre todos los cursos, excepto el último
    for(size_t i{0}; i < this->courseRequests.size()-1; i++) {
        // Empezar a iterar uno encima de i
        for(size_t j{i+1}; j < this->courseRequests.size(); j++) {
            conflicts.push_back(CoursePair(static_cast<Course>(this->courseRequests[i]), static_cast<Course>(this->courseRequests[j])));
        }
    }
    return conflicts;
}


#endif /* STUDENT_H_ */
