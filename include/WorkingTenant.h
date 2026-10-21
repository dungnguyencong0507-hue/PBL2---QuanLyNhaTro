#pragma once
#include <Tenant.h>

class WorkingTenant : public Tenant
{
private:
    std::string empID;
    std::string company;
public:
    WorkingTenant(/* args */);
    ~WorkingTenant();

    void GetDiscount() override;

    friend std::ostream& operator<<(std::ostream&, const WorkingTenant&);
};
