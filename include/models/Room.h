#pragma once
#include <header.h>
#include <vector>
#include <Models.h>
#include <string>
class Tenant;

enum RoomType{
    Basic,
    Vip,
    Studio,
};

class Room
{
    protected:
        std::string id;                         //ma phong
        std::string name;                       //ten phong
        Tenant* roomLeader= nullptr;            //nguoi dai dien
        int capacity;                           //so luong 
        int floor;                              //tang
        std::vector<Furnishing> furnishings;    //tien ich
        std::vector<Tenant*> tenants;           //danh sach nguoi o
        ull prepaid = 0;                        //tien coc (VND)
        ull rentPrice;                          //gia thue (VND) 
        ull oldElectricIndex = 0;               //chi so dien cu
        ull newElectricIndex = 0;               //chi so dien moi
        ull oldWaterIndex = 0;                  //chi so nuoc cu
        ull newWaterIndex = 0;                  //chi so dien moi
        std::vector<MonthlyBill> monthlyBills;  //danh sach hoa don hang thang
        std::vector<Damage> damages;            //danh sach bao hong
    
    public:
        static ull electricUnitPrice;           //gia dien theo don vi (VND/kWh)
        static ull waterUnitPrice;              //gia nuoc theo don vi (VND/m3)
        static ull internetCost;                //gia internet (VND/phong)
        static ull trashCost;                   //tien thu rac (VND/nguoi)
        static ull serviceCost;                 //tien dich vu (VND/nguoi)
    public:
        Room(const std::string& ="100",const std::string& ="Phong 1",int=2,int=1);
        virtual ~Room();

    void SetCapacity(int);
    void SetId(const std::string&);
    void SetName(const std::string&);
    void SetFloor(int);
    void SetPrepaid(const ull&);

    const std::string& GetId() const;
    const std::string& GetName() const;
    int GetCapacity() const;
    int GetFloor() const;
    Tenant* GetRoomLeader() const;
    const std::vector<Furnishing>& GetFurnishings() const;
    const std::vector<Tenant*>& GetTenants() const;
    const std::vector<MonthlyBill>& GetMonthlyBills() const;
    const std::vector<Damage>& GetDamages() const;

    void AddFurnishing(const Furnishing&);

    bool IsRented() const;
    bool IsFull() const;
    bool AddTenant(Tenant*);
    bool RemoveTenant(Tenant*);
    void SetRoomLeader(Tenant*);
    
    static void SetElectricUnitPrice(const ull&);
    static void SetWaterUnitPrice(const ull&);
    static void SetInternetCost(const ull&);
    static void SetTrashCost(const ull&);
    static void SetServiceCost(const ull&);
    void SetRentPrice(const ull&);
    void SetServicePrice(const ull&);
    bool SetElectricIndex(const ull&);
    bool SetWaterIndex(const ull&);

    int GetTenantCount() const;
    ull GetRentPrice() const;
    ull GetOldElectricIndex() const;
    ull GetNewElectricIndex() const;
    ull GetOldWaterIndex() const;
    ull GetNewWaterIndex() const;
    ull GetDebt() const;
    ull GetPrepaid() const;
    MonthlyBill* GetBill(const Month& month);
    virtual std::string GetRoomType() const = 0;
    virtual RoomType Type() const = 0;
    
    bool CreateBill(const Month&);
    bool AddBill(const MonthlyBill&);
    void PayBill(const Month& month, const Date& paymentDate);
    virtual ull calcRent() const = 0;
    
    void AddDamage(const Damage&);

    friend std::ostream& operator<<(std::ostream&, const Room&);
};