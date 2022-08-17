#include<vector>
#include "Student.h"

#ifndef STUDENTLIST_H_
#define STUDENTLIST_H_

using namespace std;

class StudentList {
public:
    StudentList();
    virtual ~StudentList();
    Student operator[](const int& index) const;
    Student& operator[](const int& index);
    void addStudent(Student student);
    vector<Student> getInnerVec() const;
    int getSize() const;
private:
    vector<Student> innerVec;
};

StudentList::StudentList() {}

StudentList::~StudentList() {}

Student StudentList::operator[](const int& index) const {
    return this->innerVec[index];
}

Student& StudentList::operator[](const int& index) {
    return this->innerVec[index];
}

void StudentList::addStudent(Student student) {
    this->innerVec.push_back(student);
}
vector<Student> StudentList::getInnerVec() const {
    return this->innerVec;
}

int StudentList::getSize() const {
    return this->innerVec.size();
}
#endif
