
#include <string>
using ull = unsigned long long;

struct Month
{
    __int8 month;
    int year;
};

struct Date : public Month
{
    __int8 day;
};

struct MonthlyBill
{
    Month month;
    Date paymentDate;
    ull rentCost;
    ull electricCost;
    ull waterCost;
    ull serviceCost;
    ull internetCost;
    ull trashCost;
    ull damageCost;
    ull totalCost;
    bool isPaid;
};

struct Furnishing
{
    std::string id;
    std::string name;
    ull price;
};

struct Damage
{
    Date date;
    Furnishing* furnishing;
    std::string description;
    std::string resolution;
    bool isResolved;
};