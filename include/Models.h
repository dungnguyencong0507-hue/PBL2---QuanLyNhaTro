#pragma once
#include <string>
using ull = unsigned long long;


struct Month
{
    int month;
    int year;
};

struct Date : public Month
{
    int day;
};

struct MonthlyBill
{
    Month month;
    Date paymentDate;
    ull rentCost;
    ull serviceCost;
    ull damageCost;

    ull oldElectricIndex;
    ull newElectricIndex;
    ull electricCost;

    ull oldWaterIndex;
    ull newWaterIndex;
    ull waterCost;
    
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

enum class RoomType{
    Basic,
    Vip,
    Studio,
};