#pragma once
#include <iostream>
#include <vector>
#include <Models.h>

class Tenant;
class Room
{
    protected:
        std::string id;                         //ma phong
        std::string name;                       //ten phong
        Tenant* roomLeader;                     //nguoi dai dien
        int capacity;                           //so luong 
        int floor;                              //tang
        std::vector<Furnishing> furnishings;    //tien ich
        std::vector<Tenant*> tenants;           //danh sach nguoi o
        ull prepaid;                            //tien coc (VND)
        ull rentPrice;                          //gia thue (VND)
        ull servicePrice;                       //gia dich vu (VND)    
        std::vector<ull> electricIndex;         //chi so dien
        std::vector<ull> waterIndex;            //chi so nuoc
        std::vector<MonthlyBill> monthlyBills;  //danh sach hoa don hang thang
        std::vector<Damage> damages;            //danh sach bao hong
    
    public:
        Room();
        virtual ~Room();

        bool IsRented() const;
        bool IsFull() const;
        bool AddTenant(Tenant*);
        bool RemoveTenant(Tenant*);
        void SetRoomLeader(Tenant*);
        
        virtual void SetRentPrice(const ull&) = 0;
        void SetServicePrice(const ull&);
        void SetElectricIndex(const ull&);
        void SetWaterIndex(const ull&);

        int GetTenantCount() const;
        virtual ull GetRentPrice() = 0;
        ull GetServicePrice() const;
        ull GetElectricIndex() const;
        ull GetWaterIndex() const;
        ull GetDebt() const;
        ull GetPrepaid() const;
        ull GetBill(const Month& month);
        
        void PayBill(const Month& month, const Date& paymentDate);
        virtual ull calcRent() = 0;
        
        void AddDamage(const Damage&);
};