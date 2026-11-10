#pragma once
#include <header.h>
#include <List.h>
#include <Models.h>
#include <string>
#include <ConsoleUI.h>
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
    List<Furnishing> furnishings;           //tien ich
    List<Tenant*> tenants;                  //danh sach nguoi o
    ull prepaid = 0;                        //tien coc (VND)
    ull rentPrice;                          //gia thue (VND) 
    ull oldElectricIndex = 0;               //chi so dien cu
    ull newElectricIndex = 0;               //chi so dien moi
    ull oldWaterIndex = 0;                  //chi so nuoc cu
    ull newWaterIndex = 0;                  //chi so dien moi
    List<MonthlyBill> monthlyBills;           //danh sach hoa don hang thang
    List<Damage> damages;                   //danh sach bao hong
    Date rentedDate;                        //ngay bat dau thue

public:
    static ull electricUnitPrice;           //gia dien theo don vi (VND/kWh)
    static ull waterUnitPrice;              //gia nuoc theo don vi (VND/m3)
    static ull internetCost;                //gia internet (VND/phong)
    static ull trashCost;                   //tien thu rac (VND/nguoi)
    static ull serviceCost;                 //tien dich vu (VND/nguoi)
public:
    Room(const std::string& ="100",const std::string& ="Phong 1",int=2,int=1);
    virtual ~Room();

    bool IsRented() const;
    bool IsFull() const;

    const std::string& GetId() const;
    const std::string& GetName() const;
    int GetCapacity() const;
    int GetFloor() const;
    Date GetDate() const;
    Tenant* GetRoomLeader() const;
    const Furnishing* GetFurnishing(const std::string&) const;
    const List<Furnishing>& GetFurnishings() const;
    const List<Tenant*>& GetTenants() const;
    const List<MonthlyBill>& GetMonthlyBills() const;
    const List<Damage>& GetDamages() const;
    int GetTenantCount() const;
    ull GetRentPrice() const;
    ull GetOldElectricIndex() const;
    ull GetNewElectricIndex() const;
    ull GetOldWaterIndex() const;
    ull GetNewWaterIndex() const;
    ull GetDebt() const;
    ull GetPrepaid() const;
    virtual ull GetStdPrice() const = 0;
    MonthlyBill* GetBill(const Month& month) const;
    int GetBillIndex(const Month& month) const;
    virtual std::string GetRoomType() const = 0;
    virtual RoomType Type() const = 0;
    
    void SetCapacity(int);
    void SetId(const std::string&);
    void SetName(const std::string&);
    void SetFloor(int);
    void SetPrepaid(const ull&);
    void SetDate(const Date&);
    void SetRoomLeader(Tenant*);
    static void SetElectricUnitPrice(const ull&);
    static void SetWaterUnitPrice(const ull&);
    static void SetInternetCost(const ull&);
    static void SetTrashCost(const ull&);
    static void SetServiceCost(const ull&);
    void SetRentPrice(const ull&);
    void SetElectricIndex(const ull&);
    void SetWaterIndex(const ull&);
    void EditElectricIndex(const ull&);
    void EditWaterIndex(const ull&);

    void AddTenant(Tenant*);
    void RemoveTenant(Tenant*);
    
    void AddFurnishing(Furnishing);
    void RemoveFurnishing(Furnishing*);

    void AddDamage(const Damage&);
    void RemoveDamage(Damage*);

    void CreateBill(const Month&);
    void AddBill(MonthlyBill);
    void RemoveBill(const Month&);
    void UpdateBill(const MonthlyBill&);
    void PayBill(const Month& month, const Date& paymentDate);
    virtual ull calcRent() const = 0;
    
    // friend std::ostream& operator<<(std::ostream&, const Room&);
};