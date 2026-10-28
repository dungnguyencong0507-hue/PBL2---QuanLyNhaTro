#pragma once
#include <string>
#include <header.h>
#include <iostream>
using std::endl;
using std::left;
using std::ostream;
using std::string;
using std::setw;
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

inline ostream& operator<<(ostream& o, const Month& d)
{
    o << d.month << "/" << d.year;
    return o;
}

inline ostream& operator<<(ostream& o, const Date& d)
{
    o << d.day << "/" << d.month << "/" << d.year;
    return o;
}

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

inline ostream& operator<<(ostream& o, const MonthlyBill& bill)
{
    o << "\nHoa don thang " << bill.month << endl;
    o << border::TL << string(FIELD_W, border::H) 
        << border::DT << string(COST_W, border::H) << border::TR;
        
    o << left 
        << border::V << setw(FIELD_W) << "Tien thue" << border::V 
        << setw(COST_W) << bill.rentCost << border::V << endl
        << border::V << setw(FIELD_W) << "Tien dien" << border::V 
        << setw(COST_W) << bill.electricCost << border::V << endl
        << border::V << setw(FIELD_W) << "Tien nuoc" << border::V 
        << setw(COST_W) << bill.waterCost << border::V << endl
        << border::V << setw(FIELD_W) << "Phi dich vu" << border::V 
        << setw(COST_W) << bill.serviceCost << border::V << endl
        << border::V << setw(FIELD_W) << "Phi hu hong" << border::V 
        << setw(COST_W) << bill.damageCost << border::V << endl
        << border::V << setw(FIELD_W) << "Thanh tien" << border::V 
        << setw(COST_W) << bill.totalCost << border::V << endl;
    
    o << border::BL << string(FIELD_W, border::H) 
        << border::UT << string(COST_W, border::H) << border::BR;
    o << "\nTrang thai: ";
    if (bill.isPaid) o << "Da thanh toan | ngay" << bill.paymentDate;
    else o << "Chua thanh toan";
    o << endl;
    return o;
}

struct Furnishing
{
    std::string id;
    std::string name;
    ull price;
};

inline ostream& operator<<(ostream& o, const Furnishing& furn)
{
    o << left << border::V
        << setw(F_ID_W) << furn.id << border::V
        << setw(F_NAME_W) << furn.name << border::V
        << setw(F_PRICE_W) << furn.price << border::V;
    return o;
}

struct Damage
{
    Date date;
    Furnishing* furnishing;
    std::string description;
    std::string resolution;
    bool isResolved;
};

inline ostream& operator<<(ostream& o, const Damage& damn)
{
    o << left << border::V
        << setw(DAM_DATE_W) << damn.date << border::V
        << setw(DAM_FURN_W) << damn.furnishing->name << border::V
        << setw(DAM_DESCRIPTION_W) << damn.description << border::V
        << setw(DAM_RESOLUTION_W) << damn.resolution << border::V;
    return o;
}