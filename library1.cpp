#include"library1.h"
#include"EmployeeManager.h"


void *Init()
{
    EmployeeManager *DS = new EmployeeManager();
    if(DS==nullptr)
    {
        return nullptr;
    }
    return (void*)DS;
}

StatusType AddCompany(void *DS, int CompanyID, int Value)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->AddCompany(CompanyID,Value);
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->AddEmployee(EmployeeID,CompanyID,Salary, Grade);

}

StatusType RemoveCompany(void *DS, int CompanyID)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->RemoveCompany(CompanyID);
}

StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->RemoveEmployee(EmployeeID);

}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetCompanyInfo(CompanyID, Value, NumEmployees);
    
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetEmployeeInfo(EmployeeID,EmployerID, Salary,Grade);
    
}

StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->IncreaseCompanyValue(CompanyID,ValueIncrease);
    
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->PromoteEmployee(EmployeeID, SalaryIncrease,  BumpGrade);
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->HireEmployee(EmployeeID,NewCompanyID);
    
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->AcquireCompany(AcquirerID,TargetID,Factor);

}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID){
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetHighestEarner(CompanyID, EmployeeID);
}

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees){
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetAllEmployeesBySalary(CompanyID, Employees,NumOfEmployees);
}

StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees){
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetHighestEarnerInEachCompany(NumOfCompanies,Employees);
}

StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
            int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees){
    if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return ((EmployeeManager*) DS)->GetNumEmployeesMatching(CompanyID,MinEmployeeID,MaxEmployeeId,
    MinSalary,MinGrade,TotalNumOfEmployees,NumOfEmployees);
}

void Quit(void** DS)
{
    delete (EmployeeManager*)*DS;
    *DS=nullptr;
}