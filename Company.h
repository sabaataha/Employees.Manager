#ifndef COMPANY_H
#define COMPANY_H

#include "Employee.h"
#include "AvlTree.h"
#include <ostream>


using namespace std;

class Company {
public:
    int compId;
    int numOfEmployees;
    int value;
    AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees> *employeesInCompanyBySalaries;
    AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees> *employeesInCompanyById;

    Company(int id, int value) : compId(id), numOfEmployees(0), value(value),
                                 employeesInCompanyBySalaries(
                                         new AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees>()),
                                 employeesInCompanyById(
                                         new AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees>()) {

    }

    Company() {}

    ~Company() {
        delete employeesInCompanyBySalaries;
        delete employeesInCompanyById;
    }

    Company &operator=(const Company &other) {
        if (this == &other) {
            return *this;
        }
        this->compId = other.compId;
        this->value = other.value;
        this->numOfEmployees = other.numOfEmployees;
        this->employeesInCompanyBySalaries = other.employeesInCompanyBySalaries;
        this->employeesInCompanyById = other.employeesInCompanyById;

        return *this;
    }
};

class IsEqualCompanies {
public:
    bool operator()(Company *c1, Company *c2) const {
        return c1->compId == c2->compId;
    }
};

class CompareCompaniesByID {
public:
    bool operator()(Company *c1, Company *c2) const {
        return c1->compId < c2->compId;
    }
};


#endif