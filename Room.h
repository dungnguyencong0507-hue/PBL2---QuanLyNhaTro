#pragma once
#include <string>
#include <iostream>
#include <vector>
using ull = unsigned long long;

struct MonthlyBill
{
    std::string month;
    std::string paymentDate;
    ull electricAmount;
    ull waterAmount;
    ull serviceAmount;
    ull rentAmount;
    ull totalAmount;
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
    std::string date;
    Furnishing* furnishing;
    std::string description;
    std::string resolution;
    bool isResolved;
};

class Tenant;
class Room
{
    public:
        static int stdPrice[10][5];             //gia thue theo loai phong va so nguoi o
        static ull electricUnitPrice;           //gia dien theo don vi (VND/kWh)
        static ull waterUnitPrice;              //gia nuoc theo don vi (VND/m3)
    private:
        std::string id;                         //ma phong
        std::string name;                       //ten phong
        Tenant* roomLeader;                     //nguoi dai dien
        int capacity;                           //so luong 
        int floor;                              //tang
        int type;                               //loai phong
        std::vector<Furnishing> furnishings;   //tien ich
        std::vector<Tenant*> tenants;           //danh sach nguoi o
        ull prepaid;                            //tien coc (VND)
        ull rentPrice;                          //gia thue (VND)
        ull servicePrice;                       //gia dich vu (VND)    
        std::vector<ull> electricMeter;         //chi so dien
        std::vector<ull> waterMeter;            //chi so nuoc
        std::vector<MonthlyBill> monthlyBills;  //danh sach hoa don hang thang
        std::vector<Damage> damages;            //danh sach bao hong
    
    public:
        Room();
        ~Room();

        bool IsRented() const;
        bool IsFull() const;
        bool AddTenant(Tenant*);
        bool RemoveTenant(Tenant*);
        int GetTenantCount();
        void SetRoomLeader(Tenant*);
        void SetRentPrice(const ull&);
        ull GetRentPrice();
        void SetServicePrice(const ull&);
        ull GetServicePrice();
        void SetElectricMeter(const ull&);
        ull GetElectricMeter();
        void SetWaterMeter(const ull&);
        ull GetWaterMeter();
        void PayBill(const std::string& month, const std::string& paymentDate);
        ull GetBill(const std::string& month);
        ull GetDebt();
        ull GetPrepaid();
        void AddDamage(const Damage&);
};