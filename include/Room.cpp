#include "Room.h"
#include <algorithm>
#include "header.h"
#include <iomanip>
using namespace std;
ull Room::electricUnitPrice = 3500;
ull Room::waterUnitPrice = 8000;


Room::Room(const string& id,const string& name,const int& capacity,const int& floor) 
    : id(id), name(name) , capacity(capacity) , floor(floor)
{} 
Room::~Room(){}
bool Room::IsRented() const
{
    return !tenants.empty();
}
bool Room::IsFull() const
{
    return GetTenantCount() >= this->capacity;
}
int Room::GetTenantCount() const
{
    return tenants.size();
}


const std::string& Room::GetId() const
{
    return this->id;
}

const std::string& Room::GetName() const
{
    return this->name;
}
int Room::GetCapacity() const
{
    return this->capacity;
}
int Room::GetFloor() const
{
    return this->floor;
}
Tenant* Room::GetRoomLeader() const
{
    return this->roomLeader;
}


void Room::SetCapacity(int capacity)
{
    if(capacity>0 && capacity >=this->GetTenantCount())
    { 
        this->capacity = capacity;
    }
}
void Room::SetPrepaid(const ull& prepaid)
{
    this->prepaid = prepaid;
}
ull Room::GetPrepaid() const
{
    return this->prepaid;
}
void Room::SetServicePrice(const ull& s)
{
    this->servicePrice=s;
}
ull Room::GetServicePrice() const
{
    return this->servicePrice;
}
void Room::AddFurnishing(const Furnishing& f)
{
    this->furnishings.push_back(f);
}


bool Room::AddTenant(Tenant* t)
{
    if(t==nullptr) return false;

    if(this->IsFull()) return false;

    if(find(this->tenants.begin(),this->tenants.end(),t) != this->tenants.end()) return false;

    this->tenants.push_back(t);
    if(this->roomLeader==nullptr) this->roomLeader =  t;
    
    return true;
}
bool Room::RemoveTenant(Tenant* t)
{
    auto i = find(this->tenants.begin(),this->tenants.end(),t);
    if(i == this->tenants.end()) return false;

    this->tenants.erase(i);

    if(this->roomLeader == t)
    {
        if(tenants.empty()) 
            this->roomLeader = nullptr;
        else
            this->roomLeader = tenants.front();
    }

    return true;
}
void Room::SetRoomLeader(Tenant* t)
{
    if(t == nullptr)
    {
        this->roomLeader = nullptr;
        return;
    }

    if(find(this->tenants.begin(),this->tenants.end(),t) != this->tenants.end())
    {
        this->roomLeader = t;
    }
}


void Room::SetElectricIndex(const ull& e)
{
    if(!this->electricIndex.empty() && e < this->electricIndex.back())
        return;
    
    this->electricIndex.push_back(e);
}
ull Room::GetElectricIndex() const
{
    if(this->electricIndex.empty()) return 0;

    return this->electricIndex.back();
}
void Room::SetWaterIndex(const ull& w)
{
    if(!this->waterIndex.empty() && w < this->waterIndex.back())
        return;
    
    this->waterIndex.push_back(w);
}
ull Room::GetWaterIndex() const
{
    if(this->waterIndex.empty()) return 0;

    return this->waterIndex.back();
}

bool Same_month(const Month& a,const Month& b)
{
    return a.month == b.month && a.year == b.year;
}

bool Room::CreateBill(const Month& month)
{
    for(const auto& b : this->monthlyBills)
        if(Same_month(b.month,month)) return false;
    
    MonthlyBill bill{};
    bill.month = month;
    bill.rentCost = this->calcRent();
    bill.serviceCost = servicePrice;

    int n = this -> electricIndex.size();
    if(n >= 2)
    {
        bill.electricCost = (this->electricIndex[n-1] - this->electricIndex[n-2])*electricUnitPrice;
    }
    else if (n == 1)
    {
        bill.electricCost = this->electricIndex[0] * electricUnitPrice;
    }

    int m = this->waterIndex.size();
    if(m >= 2)
    {
        bill.waterCost = (this->waterIndex[m-1] - this->waterIndex[m-2])*waterUnitPrice;
    }
    else if (m == 1)
    {
        bill.waterCost = this->waterIndex[0] * waterUnitPrice;
    }

    bill.totalCost = bill.rentCost + bill.serviceCost + bill.trashCost + bill.electricCost 
                    + bill.waterCost  + bill.internetCost + bill.damageCost;
    
    this->monthlyBills.push_back(bill);
    return true;
}
ull Room::GetBill(const Month& m)
{
    for(const auto& b : monthlyBills)
        if(Same_month(b.month,m))
            return b.totalCost;
            
    return 0;
}
void Room::PayBill(const Month& month, const Date& paymentDate)
{
    for(auto& b : this->monthlyBills)
    {
        if(Same_month(b.month,month)) 
        {
            b.isPaid = true;
            b.paymentDate = paymentDate;
            return;
        }
    }
}
ull Room::GetDebt() const
{
    ull debt = 0;
    for(const auto& b : monthlyBills)
    {
        if(!b.isPaid) debt += b.totalCost;  
    }
    return debt;
}
void Room::AddDamage(const Damage& d)
{
    this->damages.push_back(d);
}
ostream& operator<<(ostream& o, const Room& room)
{
    o << std::left 
       << std::setw(ROOMIDW) << room.id
       << std::setw(ROOMNAMEW) << room.name
       << std::setw(ROOMCAPW) << room.capacity
       << std::setw(ROOMFLOORW) << room.floor
       << std::setw(ROOMPRICEW) << room.rentPrice
       << std::setw(ROOMAVAILABLEW) << (room.IsFull() ? "Full" : "Available");
    return o;
}