#pragma once
#include <Tenant.h>

class StudentTenant : public Tenant
{
private:
    std::string studentID;
    std::string university;
public:
    StudentTenant(/* args */);
    ~StudentTenant();

    void GetDiscount() override;

    friend std::ostream& operator<<(std::ostream&, const StudentTenant&);
};
