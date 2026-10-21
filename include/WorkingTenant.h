#pragma once
#include <Tenant.h>

class WorkingTenant : public Tenant
{
private:
    // std::string numberID;
    std::string company;
public:
    WorkingTenant(/* args */);
    ~WorkingTenant();

    void GetDiscount() override;
};
