#include <Room.h>
#include <algorithm>
#include <header.h>
#include <iomanip>
using namespace std;
ull Room::electricUnitPrice = 3500;
ull Room::waterUnitPrice = 8000;
ull Room::internetCost = 50000;
ull Room::trashCost = 15000;
ull Room::serviceCost = 50000;

Room::Room(const string& id,const string& name,int capacity,int floor) 
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
ull Room::GetRentPrice()  const
{
    return this->rentPrice;
}
void Room::SetRentPrice(const ull& newPrice)
{
    this->rentPrice = newPrice;
}


void Room::AddFurnishing(const Furnishing& f)
{
    this->furnishings.push_back(f);
}
bool Room::AddTenant(Tenant* t)
{
    if(t == nullptr) return false;

    if(this->IsFull()) return false;

    if(find(this->tenants.begin(),this->tenants.end(),t) != this->tenants.end()) return false;

    this->tenants.push_back(t);
    if(this->roomLeader==nullptr) this->roomLeader = t;
    
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

void Room::SetElectricUnitPrice(const ull& p)
{
    Room::electricUnitPrice = p;
}
void Room::SetWaterUnitPrice(const ull& p)
{
    Room::waterUnitPrice = p;
}
void Room::SetInternetCost(const ull& p)
{
    Room::internetCost = p;
}
void Room::SetTrashCost(const ull& p)
{
    Room::trashCost = p;
}
void Room::SetServiceCost(const ull& p)
{
    Room::serviceCost = p;
}
bool Room::SetElectricIndex(const ull& e)
{
    if(e < this->newElectricIndex) return false;
    
    this->oldElectricIndex = this->newElectricIndex;
    this->newElectricIndex = e;
    return true;
}
ull Room::GetElectricIndex() const
{
    return this->newElectricIndex;
}
bool Room::SetWaterIndex(const ull& w)
{
    if(w < this->newWaterIndex) return false;
    
    this->oldWaterIndex = this->newWaterIndex;
    this->newWaterIndex = w;
    return true;
}
ull Room::GetWaterIndex() const
{
    return this->newWaterIndex;
}

static bool Same_month(const Month& a,const Month& b)
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
    int n = this->GetTenantCount();
    bill.serviceCost = Room::serviceCost * n + Room::trashCost * n + Room::internetCost;

    bill.damageCost = 0;
    for(const auto& d : this->damages)
        if(Same_month(Month{d.date.month, d.date.year}, month)) bill.damageCost += d.furnishing->price;

    bill.oldElectricIndex = this->oldElectricIndex;
    bill.newElectricIndex = this->newElectricIndex;
    bill.electricCost = (bill.newElectricIndex - bill.oldElectricIndex) * Room::electricUnitPrice;

    bill.oldWaterIndex = this->oldWaterIndex;
    bill.newWaterIndex = this->newWaterIndex;
    bill.waterCost = (bill.newWaterIndex - bill.oldWaterIndex) * Room::waterUnitPrice;

    bill.totalCost = bill.rentCost + bill.serviceCost + bill.electricCost 
                    + bill.waterCost + bill.damageCost;
    
    bill.isPaid = false;
    this->monthlyBills.push_back(bill);
    return true;
}
MonthlyBill* Room::GetBill(const Month& m)
{
    for(auto& b : monthlyBills)
        if(Same_month(b.month, m))
            return &b;

    return nullptr;
}
void Room::PayBill(const Month& month, const Date& paymentDate)
{
    for(auto& b : this->monthlyBills)
    {
        if(Same_month(b.month,month) && !b.isPaid) 
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
       << std::setw(ROOM_ID_W) << room.id
       << std::setw(ROOM_TYPE_W) << room.GetRoomType() 
       << std::setw(ROOM_NAME_W) << room.name
       << std::setw(ROOM_CAP_W) << room.capacity
       << std::setw(ROOM_FLOOR_W) << room.floor
       << std::setw(ROOM_PRICE_W) << room.GetRentPrice()
       << std::setw(ROOM_AVAILABLE_W) << (room.IsFull() ? "HET" : "CON");
    return o;
}