#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include "Company.h"
#include "AvlTree.h"
#include "Employee.h"
#include "GenericNode.h"
#include "library1.h"

#define EMPTY -1


class EmployeeManager {
private :
    static Employee **mergeArraysBySal(Employee *arr1[], Employee *arr2[], int m, int n) {
        Employee **mergedArr = new Employee *[m + n];
        int i = 0, j = 0, k = 0;
        CompareEmployeeBySalary comp;
        while (i < m && j < n) {
            if (comp(arr1[i], arr2[j])) {
                mergedArr[k] = arr1[i];
                i++;
            } else {
                mergedArr[k] = arr2[j];
                j++;
            }
            k++;
        }
        // if m>n
        while (i < m) {
            mergedArr[k] = arr1[i];
            i++;
            k++;
        }
        // if n<m
        while (j < n) {
            mergedArr[k] = arr2[j];
            j++;
            k++;
        }
        return mergedArr;
    }

    static Employee **mergeArraysByID(Employee *arr1[], Employee *arr2[], int m, int n) {
        Employee **mergedArr = new Employee *[m + n];
        int i = 0, j = 0, k = 0;
        CompareEmployeeByID comp;
        while (i < m && j < n) {
            if (comp(arr1[i], arr2[j])) {
                mergedArr[k] = arr1[i];
                i++;
            } else {
                mergedArr[k] = arr2[j];
                j++;
            }
            k++;
        }
        // if m>n
        while (i < m) {
            mergedArr[k] = arr1[i];
            i++;
            k++;
        }
        // if n<m
        while (j < n) {
            mergedArr[k] = arr2[j];
            j++;
            k++;
        }
        return mergedArr;
    }

public:
    AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees> *employeesSortedBySalaries;
    AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees> *employeesSortedByIDs;
    AVLtree<Company *, CompareCompaniesByID, IsEqualCompanies> *companies;
    AVLtree<Company *, CompareCompaniesByID, IsEqualCompanies> *companiesWithEmployees;

    EmployeeManager() : employeesSortedBySalaries(new AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees>()),
                        employeesSortedByIDs(new AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees>()),
                        companies(new AVLtree<Company *, CompareCompaniesByID, IsEqualCompanies>()),
                        companiesWithEmployees(new AVLtree<Company *, CompareCompaniesByID, IsEqualCompanies>()) {}


    ~EmployeeManager() {
        int num_of_Employees = employeesSortedBySalaries->num_of_nodes;
        Employee **employeesArray = new Employee *[num_of_Employees];
        employeesSortedBySalaries->storeInOrderToArray(employeesArray);
        for (int i = 0; i < num_of_Employees; i++) {
            delete employeesArray[i];
        }

        int num_of_companies = companies->num_of_nodes;
        Company **companiesArray = new Company *[num_of_companies];
        companies->storeInOrderToArray(companiesArray);
        for (int i = 0; i < num_of_companies; i++) {
            delete companiesArray[i];
        }
        delete[] employeesArray;
        delete[] companiesArray;
        delete employeesSortedBySalaries;
        delete employeesSortedByIDs;
        delete companies;
        delete companiesWithEmployees;
    }


    EmployeeManager(const EmployeeManager &em) = default;


    void *Init() {
        EmployeeManager *EM = new EmployeeManager();
        if (EM == nullptr) {
            return nullptr;
        }
        return EM;
    }


    StatusType AddCompany(int CompanyID, int Value) {
        if (CompanyID <= 0 || Value <= 0) {
            return INVALID_INPUT;
        }
        Company *company = new Company(CompanyID, Value);
        if (company == nullptr) {
            return ALLOCATION_ERROR;
        }
        try {
            this->companies->insert(company);
        }
        catch (NodeAlreadyExists &e) {
            delete company;
            return FAILURE;
        }
        return SUCCESS;
    }


    StatusType AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade) {
        if (EmployeeID <= 0 || CompanyID <= 0 || Salary <= 0 || Grade < 0) {
            return INVALID_INPUT;
        }

        Employee *employee = new Employee(EmployeeID, Salary, Grade, CompanyID);
        if (employee == nullptr) {
            return ALLOCATION_ERROR;
        }

        //check if the employee already exists
        try {
            this->employeesSortedBySalaries->insert(employee);
        }
        catch (NodeAlreadyExists &e) {
            delete employee;
            return FAILURE;
        }
        try {
            this->employeesSortedByIDs->insert(employee);
        }
        catch (NodeAlreadyExists &e) {
            this->employeesSortedBySalaries->remove(employee);
            delete employee;
            return FAILURE;
        }

        //check if there is a company with the comId
        Company *companyToFind = new Company(CompanyID, 0);
        Company **compRes;
        compRes = this->companies->findData(companyToFind);
        if (compRes == nullptr) {
            this->employeesSortedBySalaries->remove(employee);
            this->employeesSortedByIDs->remove(employee);
            delete employee;
            delete companyToFind;
            return FAILURE;
        }

        (*compRes)->employeesInCompanyBySalaries->insert(employee);
        (*compRes)->employeesInCompanyById->insert(employee);
        (*compRes)->numOfEmployees++;

        //adds to companiesWithEmployees
        Company **compResWithEmployees;
        compResWithEmployees = this->companiesWithEmployees->findData(*compRes);

        if (compResWithEmployees == nullptr) {
            this->companiesWithEmployees->insert(*compRes);
        }
        delete companyToFind;
        return SUCCESS;
    }


    StatusType RemoveEmployee(int EmployeeID) {
        if (EmployeeID <= 0) {
            return INVALID_INPUT;
        }

        //check if the employee exists
        Employee *empToFind = new Employee(EmployeeID, EMPTY, EMPTY, EMPTY);
        if (empToFind == nullptr) {
            return ALLOCATION_ERROR;
        }

        Employee **emp;
        emp = this->employeesSortedByIDs->findData(empToFind);
        if (emp == nullptr) {
            delete empToFind;
            return FAILURE;
        }
        Employee *emp_pointer = *emp;

        int empCompId = (*emp)->companyId;
        int empSalary = (*emp)->salary;
        empToFind->salary = empSalary;
        Company *compToFind = new Company(empCompId, EMPTY);
        if (compToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Company **compRes;
        compRes = this->companiesWithEmployees->findData(compToFind);
        // update in companies tree
        (*compRes)->numOfEmployees--;

        this->employeesSortedBySalaries->remove(emp_pointer);
        this->employeesSortedByIDs->remove(emp_pointer);

        (*compRes)->employeesInCompanyBySalaries->remove(emp_pointer);
        (*compRes)->employeesInCompanyById->remove(emp_pointer);
        // update in companies with employee tree
        if ((*compRes)->numOfEmployees == 0) { //it is the only member of the group
            this->companiesWithEmployees->remove(*compRes);
        }
        delete empToFind;
        delete compToFind;
        delete (emp_pointer);
        return SUCCESS;
    }


    StatusType RemoveCompany(int CompanyID) {
        if (CompanyID <= 0) {
            return INVALID_INPUT;
        }
        Company *compToFind = new Company(CompanyID, 0);
        if (compToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Company **compRes1, **compRes2;
        compRes1 = this->companies->findData(compToFind);
        compRes2 = this->companiesWithEmployees->findData(compToFind);
        if (compRes1 == nullptr || compRes2 != nullptr) {
            delete compToFind;
            return FAILURE;
        }
        Company *compPointer = *compRes1;
        this->companies->remove(compPointer);
        delete compToFind;
        delete (compPointer);
        return SUCCESS;
    }


    StatusType GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees) {
        if (CompanyID <= 0 || Value == nullptr || NumEmployees == nullptr) {
            return INVALID_INPUT;
        }
        Company *companyToFind = new Company(CompanyID, EMPTY);
        Company **compRes;
        compRes = this->companies->findData(companyToFind);
        if (compRes == nullptr) {
            delete companyToFind;
            return FAILURE;
        }
        *Value = (*compRes)->value;
        *NumEmployees = (*compRes)->numOfEmployees;
        delete companyToFind;
        return SUCCESS;
    }


    StatusType GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade) {
        if (EmployeeID <= 0 || EmployerID == nullptr || Salary == nullptr || Grade == nullptr) {
            return INVALID_INPUT;
        }
        Employee *empToFind = new Employee(EmployeeID, EMPTY, EMPTY, EMPTY);
        if (empToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Employee **emp;
        emp = this->employeesSortedByIDs->findData(empToFind);
        if (emp == nullptr) {
            delete empToFind;
            return FAILURE;
        }
        *EmployerID = (*emp)->companyId;
        *Salary = (*emp)->salary;
        *Grade = (*emp)->grade;
        delete empToFind;
        return SUCCESS;
    }


    StatusType IncreaseCompanyValue(int CompanyID, int ValueIncrease) {
        if (CompanyID <= 0 || ValueIncrease <= 0) {
            return INVALID_INPUT;
        }
        Company *compToFind = new Company(CompanyID, EMPTY);
        if (compToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Company **compRes;
        compRes = this->companies->findData(compToFind);
        if (compRes == nullptr) {
            delete compToFind;
            return FAILURE;
        }
        (*compRes)->value += ValueIncrease;
        delete compToFind;
        return SUCCESS;
    }


    StatusType PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade) {
        if (EmployeeID <= 0 || SalaryIncrease <= 0) {
            return INVALID_INPUT;
        }
        Employee *empToFind = new Employee(EmployeeID, EMPTY, EMPTY, EMPTY);
        if (empToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Employee **emp;
        Employee *emp_pointer;
        emp = this->employeesSortedByIDs->findData(empToFind);
        if (emp == nullptr) {
            delete empToFind;
            return FAILURE;
        }
        emp_pointer = (*emp);
        int empSalary = (*emp)->salary;
        int empCompID = (*emp)->companyId;
        int empGrade = (*emp)->grade;
        if (BumpGrade > 0) {
            empGrade = (*emp)->grade + 1;
        }
        Employee *newEmp = new Employee(EmployeeID, empSalary + SalaryIncrease, empGrade, empCompID);
        this->employeesSortedByIDs->remove(emp_pointer);  //O(log n)
        this->employeesSortedByIDs->insert(newEmp); //O(log n)
        this->employeesSortedBySalaries->remove(emp_pointer);  //O(log n)
        this->employeesSortedBySalaries->insert(newEmp); //O(log n)


        Company *compToFind = new Company(empCompID, EMPTY);
        Company **compRes;
        compRes = this->companiesWithEmployees->findData(compToFind);

        (*compRes)->employeesInCompanyBySalaries->remove(emp_pointer); //max O(log n)
        (*compRes)->employeesInCompanyBySalaries->insert(newEmp); //max O(log n)

        (*compRes)->employeesInCompanyById->remove(emp_pointer); //max O(log n)
        (*compRes)->employeesInCompanyById->insert(newEmp); //max O(log n)

        delete (emp_pointer);
        delete compToFind;
        delete empToFind;
        return SUCCESS;
    }


    StatusType HireEmployee(int EmployeeID, int NewCompanyID) {
        if (EmployeeID <= 0 || NewCompanyID <= 0) {
            return INVALID_INPUT;
        }

        /** REMOVE **/
        //check if the employee exists
        Employee *empToFind = new Employee(EmployeeID, EMPTY, EMPTY, EMPTY);
        if (empToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Employee **emp;
        Employee *emp_pointer;
        emp = this->employeesSortedByIDs->findData(empToFind);
        if (emp == nullptr) {
            delete empToFind;
            return FAILURE;
        }
        emp_pointer = *emp;

        int oldCompId = (*emp)->companyId;
        int empSalary = (*emp)->salary;
        int empGrade = (*emp)->grade;

        if (oldCompId == NewCompanyID) {
            delete empToFind;
            return FAILURE;
        }

        //check if the new comp exists
        Company *companyToFind = new Company(NewCompanyID, EMPTY);
        Company **compCheck;
        compCheck = this->companies->findData(companyToFind);
        if (compCheck == nullptr) {
            delete empToFind;
            delete companyToFind;
            return FAILURE;
        }

        //find the old comp and remove from it
        Company *compToFind = new Company(oldCompId, 0);
        if (compToFind == nullptr) {
            return ALLOCATION_ERROR;
        }
        Company **compRes;
        compRes = this->companiesWithEmployees->findData(compToFind);
        (*compRes)->numOfEmployees--;
        (*compRes)->employeesInCompanyBySalaries->remove(emp_pointer);
        (*compRes)->employeesInCompanyById->remove(emp_pointer);
        if ((*compRes)->numOfEmployees == 0) { //it is the only emp in the company
            this->companiesWithEmployees->remove(*compRes);
        }

        //remove from employeesTree
        this->employeesSortedBySalaries->remove(emp_pointer);
        this->employeesSortedByIDs->remove(emp_pointer);

        /** ADD **/

        Employee *employee = new Employee(EmployeeID, empSalary, empGrade, NewCompanyID);
        if (employee == nullptr) {
            return ALLOCATION_ERROR;
        }

        //check if the employee already exists
        try {
            this->employeesSortedBySalaries->insert(employee);
        }
        catch (NodeAlreadyExists &e) {
            delete employee;
            delete empToFind;
            delete companyToFind;
            return FAILURE;
        }
        try {
            this->employeesSortedByIDs->insert(employee);
        }
        catch (NodeAlreadyExists &e) {
            this->employeesSortedBySalaries->remove(employee);
            delete employee;
            delete empToFind;
            delete companyToFind;
            return FAILURE;
        }

        (*compCheck)->employeesInCompanyBySalaries->insert(employee);
        (*compCheck)->employeesInCompanyById->insert(employee);
        (*compCheck)->numOfEmployees++;

        //adds to companiesWithEmployees
        Company **compResWithEmployees;
        compResWithEmployees = this->companiesWithEmployees->findData(*compCheck);
        if (compResWithEmployees == nullptr) {
            this->companiesWithEmployees->insert(*compCheck);
        }

        delete companyToFind;
        delete empToFind;
        delete compToFind;
        delete (emp_pointer);

        return SUCCESS;
    }


    StatusType AcquireCompany(int AcquirerID, int TargetID, double Factor) {
        if (AcquirerID <= 0 || TargetID <= 0 || AcquirerID == TargetID || Factor < 1) {
            return INVALID_INPUT;
        }
        Company *compToFind = new Company(TargetID, EMPTY);
        Company **compToDelete;
        compToDelete = this->companies->findData(compToFind);
        if (compToDelete == nullptr) {
            delete compToFind;
            return FAILURE;
        }
        Company *compToDeletePointer = *compToDelete;

        //check if the second comp exists
        Company *compToFind2 = new Company(AcquirerID, EMPTY);
        Company **compToAdd;
        compToAdd = this->companies->findData(compToFind2);
        if (compToAdd == nullptr) {
            delete compToFind;
            delete compToFind2;
            return FAILURE;
        }
        if ((*compToAdd)->value < (*compToDelete)->value * 10) {
            delete compToFind;
            delete compToFind2;
            return FAILURE;
        }
        (*compToAdd)->value = ((*compToAdd)->value + (*compToDelete)->value) * Factor;

        //check if the first comp has emp
        Company **compCheck;
        compCheck = this->companiesWithEmployees->findData(*compToDelete);
        if (compCheck == nullptr) {
            this->companies->remove(compToDeletePointer);
            delete compToFind;
            delete compToFind2;
            delete compToDeletePointer;
            return SUCCESS;
        }

        int n_delete = (*compToDelete)->employeesInCompanyBySalaries->num_of_nodes;
        Employee **firstCompToArrayBySalary = new Employee *[n_delete];
        (*compToDelete)->employeesInCompanyBySalaries->storeInOrderToArray(firstCompToArrayBySalary);
        // we added by id tree
        Employee **firstCompToArrayById = new Employee *[n_delete];
        (*compToDelete)->employeesInCompanyById->storeInOrderToArray(firstCompToArrayById);

        int n_add = (*compToAdd)->employeesInCompanyBySalaries->num_of_nodes;

        //if the sec company has no players
        if (n_add == 0) {
            for (int i = 0; i < n_delete; i++) {
                firstCompToArrayBySalary[i]->companyId = AcquirerID;
            }
            delete ((*compToAdd)->employeesInCompanyBySalaries);
            (*compToAdd)->employeesInCompanyBySalaries = new AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees>(
                    firstCompToArrayBySalary, n_delete);

            // by id tree
            for (int i = 0; i < n_delete; i++) {
                firstCompToArrayById[i]->companyId = AcquirerID;
            }
            delete ((*compToAdd)->employeesInCompanyById);
            (*compToAdd)->employeesInCompanyById = new AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees>(
                    firstCompToArrayById,
                    n_delete);
            // add to the comp with emp
            this->companiesWithEmployees->insert(*compToAdd);
        } else {
            Employee **secondCompToArrayBySalary = new Employee *[n_add];
            (*compToAdd)->employeesInCompanyBySalaries->storeInOrderToArray(secondCompToArrayBySalary);
            Employee **merged_salaries = this->mergeArraysBySal(firstCompToArrayBySalary, secondCompToArrayBySalary,
                                                                n_delete, n_add);
            delete (*compToAdd)->employeesInCompanyBySalaries;
            for (int i = 0; i < n_delete + n_add; i++) {
                merged_salaries[i]->companyId = AcquirerID;
            }
            (*compToAdd)->employeesInCompanyBySalaries = new AVLtree<Employee *, CompareEmployeeBySalary, IsEqualEmployees>
                    (merged_salaries, n_delete + n_add);
            // add the id tree
            Employee **secondCompToArrayByID = new Employee *[n_add];
            (*compToAdd)->employeesInCompanyById->storeInOrderToArray(secondCompToArrayByID);
            Employee **merged_ID = this->mergeArraysByID(firstCompToArrayById, secondCompToArrayByID, n_delete,
                                                         n_add);
            delete (*compToAdd)->employeesInCompanyById;

            for (int i = 0; i < n_delete + n_add; i++) {
                merged_ID[i]->companyId = AcquirerID;
            }
            (*compToAdd)->employeesInCompanyById = new AVLtree<Employee *, CompareEmployeeByID, IsEqualEmployees>
                    (merged_ID, n_delete + n_add);

            delete[] secondCompToArrayBySalary;
            delete[] merged_salaries;
            delete[] secondCompToArrayByID;
            delete[] merged_ID;
        }

        (*compToAdd)->numOfEmployees = n_delete + n_add;
        this->companies->remove(compToDeletePointer);
        this->companiesWithEmployees->remove(compToDeletePointer);
        delete[] firstCompToArrayBySalary;
        delete[] firstCompToArrayById;
        delete compToDeletePointer;
        delete compToFind;
        delete compToFind2;
        return SUCCESS;
    }


    StatusType GetHighestEarner(int CompanyID, int *EmployeeID) {
        if (CompanyID == 0 || EmployeeID == nullptr) {
            return INVALID_INPUT;
        }
        if (CompanyID < 0) {
            if (this->employeesSortedBySalaries->maxData() == nullptr) {

                return FAILURE;
            }
            int empId = (*(this->employeesSortedBySalaries->maxData()))->employeeId;
            *EmployeeID = empId;
        } else {
            Company *compToFind = new Company(CompanyID, EMPTY);
            Company **compRes1, **compRes2;
            compRes1 = this->companies->findData(compToFind);
            if (compRes1 == nullptr) {
                delete compToFind;
                return FAILURE;
            }
            compRes2 = this->companiesWithEmployees->findData(compToFind);
            if (compRes2 == nullptr) {
                delete compToFind;
                return FAILURE;
            }
            int empId = (*(*compRes2)->employeesInCompanyBySalaries->maxData())->employeeId;

            *EmployeeID = empId;
            delete compToFind;
        }
        return SUCCESS;
    }


    StatusType GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees) {
        if (CompanyID == 0 || Employees == nullptr || NumOfEmployees == nullptr) {
            return INVALID_INPUT;
        }

        if (CompanyID < 0) {
            int num_of_emp = this->employeesSortedBySalaries->num_of_nodes;
            if (num_of_emp == 0) {
                return FAILURE;
            }
            Employee **employeesData = new Employee *[num_of_emp];

            this->employeesSortedBySalaries->avlToSortedArrayBackwards(employeesData, num_of_emp - 1);
            int *arr = (int *) malloc(sizeof(int) * num_of_emp);
            for (int i = 0; i < num_of_emp; i++) {
                arr[i] = employeesData[i]->employeeId;
            }
            *Employees = arr;
            *NumOfEmployees = num_of_emp;

            for (int i = 0; i < num_of_emp; i++) {
                employeesData[i] = nullptr;
            }
            delete[] employeesData;
        } else {
            Company *compToFind = new Company(CompanyID, EMPTY);
            Company **compRes1, **compRes2;
            compRes1 = this->companies->findData(compToFind);
            if (compRes1 == nullptr) {
                delete compToFind;
                return FAILURE;
            }
            compRes2 = this->companiesWithEmployees->findData(compToFind);
            if (compRes2 == nullptr) {
                delete compToFind;
                return FAILURE;
            }
            int num_of_emp = (*compRes2)->employeesInCompanyBySalaries->num_of_nodes;
            Employee **employeesData = new Employee *[num_of_emp];
            // it was compRes1 need to check !
            (*compRes2)->employeesInCompanyBySalaries->avlToSortedArrayBackwards(employeesData, num_of_emp - 1);

            int *arr = (int *) malloc(sizeof(int) * num_of_emp);
            for (int i = 0; i < num_of_emp; i++) {
                arr[i] = employeesData[i]->employeeId;
            }
            *Employees = arr;
            *NumOfEmployees = num_of_emp;
            for (int i = 0; i < num_of_emp; i++) {
                employeesData[i] = nullptr;
            }
            delete compToFind;
            delete[] employeesData;
        }
        return SUCCESS;
    }


    StatusType GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees) {
        if (Employees == nullptr || NumOfCompanies < 1) {
            return INVALID_INPUT;
        }
        if (NumOfCompanies > this->companiesWithEmployees->num_of_nodes) {
            return FAILURE;
        }
        Company **compsData = new Company *[NumOfCompanies];

        this->companiesWithEmployees->storeInOrderToArray2(compsData, NumOfCompanies);

        int *arr = (int *) malloc(NumOfCompanies * sizeof(int));
        if (!arr) {
            return ALLOCATION_ERROR;
        }
        for (int i = 0; i < NumOfCompanies; i++) {
            Employee *emp = *(compsData[i]->employeesInCompanyBySalaries->maxData());
            arr[i] = emp->employeeId;
        }

        *(Employees) = arr;

        for (int i = 0; i < NumOfCompanies; i++) {
            compsData[i] = nullptr;
        }
        delete[] compsData;
        return SUCCESS;
    }


    StatusType GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeId,
                                       int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees) {
        if (TotalNumOfEmployees == nullptr || NumOfEmployees == nullptr || CompanyID == 0 || MinEmployeeID < 0 ||
            MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || MinEmployeeID > MaxEmployeeId) {
            return INVALID_INPUT;
        }
        if (CompanyID < 0) {
            if (this->employeesSortedByIDs->num_of_nodes == 0) {
                return FAILURE;
            }
            Employee *em_min = new Employee(MinEmployeeID, EMPTY, EMPTY, EMPTY);
            if (em_min == nullptr) {
                return ALLOCATION_ERROR;
            }
            Employee *em_max = new Employee(MaxEmployeeId, EMPTY, EMPTY, EMPTY);
            if (em_max == nullptr) {
                delete em_min;
                return ALLOCATION_ERROR;
            }

            int size = this->employeesSortedByIDs->getNodesNumInRange(employeesSortedByIDs->root, em_min, em_max);
            // mekre 1
            *TotalNumOfEmployees = size;
            Employee **arr = new Employee *[size];
            if (arr == nullptr) {
                delete em_min;
                delete em_max;
                return ALLOCATION_ERROR;
            }
            this->employeesSortedByIDs->getArr(em_min, em_max, arr);
            int num_of_emp = 0;
            for (int i = 0; i < size; i++) {
                if (arr[i]->salary >= MinSalary && arr[i]->grade >= MinGrade) {
                    num_of_emp++;
                }
            }
            *NumOfEmployees = num_of_emp;

            delete[] arr;
            delete em_min;
            delete em_max;
        } else {
            Company *compToFind = new Company(CompanyID, EMPTY);
            Company **compRes;
            compRes = this->companiesWithEmployees->findData(compToFind);
            if (compRes == nullptr) {
                delete compToFind;
                return FAILURE;
            }
            Employee *em_min = new Employee(MinEmployeeID, EMPTY, EMPTY, EMPTY);
            if (em_min == nullptr) {
                delete compToFind;
                return ALLOCATION_ERROR;
            }
            Employee *em_max = new Employee(MaxEmployeeId, EMPTY, EMPTY, EMPTY);
            if (em_max == nullptr) {
                delete compToFind;
                delete em_min;
                return ALLOCATION_ERROR;
            }

            int size = (*compRes)->employeesInCompanyById->getNodesNumInRange(
                    (*compRes)->employeesInCompanyById->root,
                    em_min, em_max);
            // mekre 1
            *TotalNumOfEmployees = size;
            Employee **arr = new Employee *[size];
            if (arr == nullptr) {
                delete em_min;
                delete em_max;
                return ALLOCATION_ERROR;
            }
            (*compRes)->employeesInCompanyById->getArr(em_min, em_max, arr);
            int num_of_emp = 0;
            for (int i = 0; i < size; i++) {
                if (arr[i]->salary >= MinSalary && arr[i]->grade >= MinGrade) {
                    num_of_emp++;
                }
            }
            *NumOfEmployees = num_of_emp;

            delete[] arr;
            delete compToFind;
            delete em_min;
            delete em_max;
        }
        return SUCCESS;
    }

};


#endif