#pragma once
#include <iostream>
#include <vector>
#include "Models.h"
#include <string>
class Tenant;
class Room
{
    protected:
        std::string id;                         //ma phong
        std::string name;                       //ten phong
        Tenant* roomLeader= nullptr;            //nguoi dai dien
        int capacity ;                          //so luong 
        int floor ;                              //tang
        std::vector<Furnishing> furnishings;    //tien ich
        std::vector<Tenant*> tenants;           //danh sach nguoi o
        ull prepaid = 0;                        //tien coc (VND)
        // ull rentPrice = 0;                     //gia thue (VND)
        ull servicePrice = 0;                   //gia dich vu (VND)    
        std::vector<ull> electricIndex;         //chi so dien
        std::vector<ull> waterIndex;            //chi so nuoc
        std::vector<MonthlyBill> monthlyBills;  //danh sach hoa don hang thang
        std::vector<Damage> damages;            //danh sach bao hong
    
    public:
        static ull electricUnitPrice;           //gia dien theo don vi (VND/kWh)
        static ull waterUnitPrice;              //gia nuoc theo don vi (VND/m3)
    public:
        Room(const std::string& ="100",const std::string& ="Phong 1",const int& =5,const int& =2);
        virtual ~Room();

    void SetCapacity(int);
    void SetPrepaid(const ull&);

    const std::string& GetId() const;
    const std::string& GetName() const;
    int GetCapacity() const;
    int GetFloor() const;
    Tenant* GetRoomLeader() const;

    void AddFurnishing(const Furnishing&);

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
        virtual ull GetRentPrice() const = 0;
        ull GetServicePrice() const;
        ull GetElectricIndex() const;
        ull GetWaterIndex() const;
        ull GetDebt() const;
        ull GetPrepaid() const;
        ull GetBill(const Month& month);
        
        bool CreateBill(const Month&);
        void PayBill(const Month& month, const Date& paymentDate);
        virtual ull calcRent() const = 0;
        
        void AddDamage(const Damage&);

    friend std::ostream& operator<<(std::ostream&, const Room&);
};