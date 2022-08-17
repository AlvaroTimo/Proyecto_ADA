#ifndef COURSE_H_
#define COURSE_H_
#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Course {
public:
    Course(string code, string title, string prof);
    Course(int invalidPlaceholder);
    virtual ~Course();
    static vector<Course> getAllCourses();
    static Course getCourseByCode(const string& code);
    static Course getCourseByEnumID(const int& enumID);
    string getCode() const;
    string getTitle() const;
    string getProf() const;
    int getEnumID() const;
    bool isValid() const;
private:
    static int enum_ctr;
    static vector<Course> all_courses;
    int enumID;
    string code;
    string title;
    string prof;
};

/* Usado para identificador único */
int Course::enum_ctr{0};
/* Vector estático de todos los objetos de curso únicos */
vector<Course> Course::all_courses{};

/* Instancia un nuevo curso y le da un identificador entero único*/
Course::Course(string code, string title, string prof) :
        code{code}, title{title}, prof{prof}, enumID{enum_ctr} {
    // Si este curso comparte código con otro (duplicado), no lo agregue al vector estático all_courses
    for(const Course& c : getAllCourses()) {
        if(c.getCode() == code) {
            cout << "Listado de cursos duplicados \"" << code << "\". Ignorando el último duplicado." << endl;
            return;
        }
    }
    enum_ctr++; // Incrementar la variable de identificador único
    all_courses.push_back(*this); // Agrega esto al vector all_courses
}

/* Constructor para un curso inválido. Usado como bandera, no como objeto, en la práctica */
Course::Course(int invalidPlaceholder) :
        code{"N/A"}, title{"N/A"}, prof{"N/A"}, enumID{-1} {}

Course::~Course() {}

vector<Course> Course::getAllCourses() {
    return all_courses;
}

Course Course::getCourseByCode(const string& code) {
    // Buscar este curso en vector de cursos conocidos
    for(const Course& c : all_courses) {
        if(code == c.code)
            return c;
    }
    // Si no se puede encontrar el curso, dígalo y cree uno inválido
    return Course{-1};
}

Course Course::getCourseByEnumID(const int& enumID) {
    return all_courses.at(enumID);
}

string Course::getCode() const {
    return this->code;
}
string Course::getTitle() const {
    return this->title;
}
string Course::getProf() const {
    return this->prof;
}
int Course::getEnumID() const {
    return this->enumID;
}

bool Course::isValid() const {
    return this->enumID != -1;
}
#endif
