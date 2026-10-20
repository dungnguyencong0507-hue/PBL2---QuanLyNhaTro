#include <Tenant.h>

class WorkingTenant : public Tenant
{
private:
    std::string studentID;
    std::string university;
public:
    WorkingTenant(/* args */);
    ~WorkingTenant();

    void GetDiscount() override;
};
