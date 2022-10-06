#ifndef EMPLOYEE_H
#define EMPLOYEE_H


#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include "GenericNode.h"

using namespace std;


class Employee {
public:
    int employeeId;
    int salary;
    int grade;
    int companyId;

    Employee(int id, int salary, int grade, int cId) : employeeId(id), salary(salary), grade(grade), companyId(cId) {}

    Employee() = default;

    ~Employee() = default;

    Employee(const Employee &employee) = default;

    Employee &operator=(const Employee &other) {
        if (this == &other) {
            return *this;
        }
        this->employeeId = other.employeeId;
        this->salary = other.salary;
        this->grade = other.grade;
        this->companyId = other.companyId;
        return *this;
    }
};

class CompareEmployeeBySalary {
public:
    bool operator()(Employee *e1, Employee *e2) const {
        if (e1->salary < e2->salary) {
            return true;
        } else if (e1->salary > e2->salary) {
            return false;
        } else {
            return (e1->employeeId > e2->employeeId);
        }
    }
};

class CompareEmployeeByID {
public:
    bool operator()(Employee *e1, Employee *e2) const {
        return e1->employeeId < e2->employeeId;
    }
};

class IsEqualEmployees {
public:
    bool operator()(Employee *e1, Employee *e2) const {
        return e1->employeeId == e2->employeeId;
    }
};

#endif