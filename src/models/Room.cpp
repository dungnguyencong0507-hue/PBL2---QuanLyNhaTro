#include <Nhatro.h>
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

// ===============================
//             GETTER
// ===============================

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
Date Room::GetDate() const
{
    return this->rentedDate;
}
Tenant* Room::GetRoomLeader() const
{
    return this->roomLeader;
}
const Furnishing* Room::GetFurnishing(const string& id) const
{
    for (const auto& item : this->furnishings) {
        if (id == item.id) return const_cast<Furnishing*>(&item);
    }
    return nullptr;
}
const List<Furnishing>& Room::GetFurnishings() const
{
    return this->furnishings;
}
const List<Tenant*>& Room::GetTenants() const
{
    return this->tenants;
}
const List<MonthlyBill>& Room::GetMonthlyBills() const
{
    return this->monthlyBills;
}
const List<Damage>& Room::GetDamages() const
{
    return this->damages;
}
ull Room::GetPrepaid() const
{
    return this->prepaid;
}
ull Room::GetRentPrice()  const
{
    return this->rentPrice;
}
ull Room::GetOldElectricIndex() const
{
    return this->oldElectricIndex;
}
ull Room::GetNewElectricIndex() const
{
    return this->newElectricIndex;
}
ull Room::GetOldWaterIndex() const
{
    return this->oldWaterIndex;
}
ull Room::GetNewWaterIndex() const
{
    return this->newWaterIndex;
}

// ===============================
//             SETTER
// ===============================

void Room::SetRentPrice(const ull& newPrice)
{
    this->rentPrice = newPrice;
}
void Room::SetCapacity(int capacity)
{
    if(capacity>0 && capacity >=this->GetTenantCount())
    { 
        this->capacity = capacity;
    }
}
void Room::SetId(const string& id)
{
    this->id = id;
}
void Room::SetName(const string& name)
{
    this->name = name;
}
void Room::SetFloor(int floor)
{
    if (floor > 0) this->floor = floor;
}
void Room::SetPrepaid(const ull& prepaid)
{
    this->prepaid = prepaid;
}
void Room::SetDate(const Date& d)
{
    this->rentedDate = d;
}
void Room::SetRoomLeader(Tenant* t)
{
    if(t == nullptr)
    {
        this->roomLeader = nullptr;
        throw runtime_error("Khong ton tai du lieu nguoi dai dien!");
    }

    if (this->roomLeader == t) return;

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
void Room::SetElectricIndex(const ull& index)
{
    if (index < this->newElectricIndex) 
        throw runtime_error("Chi so dien moi phai lon hon chi so dien cu: " + this->newElectricIndex);
    this->oldElectricIndex = this->newElectricIndex;
    this->newElectricIndex = index;
}
void Room::SetWaterIndex(const ull& index)
{
    if (index < this->newWaterIndex) 
        throw runtime_error("Chi so nuoc moi phai lon hon chi so nuoc cu: " + this->newWaterIndex);
    this->oldWaterIndex = this->newWaterIndex;
    this->newWaterIndex = index;
}
void Room::EditElectricIndex(const ull& index)
{
    if (index < this->oldElectricIndex) 
        throw runtime_error("Chi so dien moi phai lon hon chi so dien cu: " + this->oldElectricIndex);
    this->newElectricIndex = index;
}
void Room::EditWaterIndex(const ull& index)
{
    if (index < this->oldWaterIndex) 
        throw runtime_error("Chi so nuoc moi phai lon hon chi so nuoc cu: " + this->oldWaterIndex);
    this->newWaterIndex = index;
}

// ===============================
//       ADD-REMOVE-UPDATE
// ===============================


void Room::AddTenant(Tenant* t)
{
    if(t == nullptr) throw runtime_error("Khong ton tai du lieu nguoi thue!");

    if(this->IsFull()) throw runtime_error("Phong da day!");

    if(find(this->tenants.begin(),this->tenants.end(),t) != this->tenants.end()) 
        throw runtime_error("Nguoi thue nay da co trong phong!");

    this->tenants.push_back(t);
    if(this->roomLeader == nullptr) this->roomLeader = t;
    t->SetRoom(this);
}
void Room::RemoveTenant(Tenant* t)
{
    auto i = find(this->tenants.begin(), this->tenants.end(), t);
    if(i == this->tenants.end()) throw runtime_error("Nguoi thue nay khong co trong phong!");

    this->tenants.erase(i);

    if(this->roomLeader == t)
    {
        if(tenants.empty()) 
            this->roomLeader = nullptr;
        else
            this->roomLeader = tenants.front();
    }

    t->SetRoom(nullptr);
    
    if (!this->IsRented()) this->rentedDate = Nhatro::today;
}

void Room::AddFurnishing(Furnishing f)
{
    if (f.id.empty()) {
        f.id = this->id;
        if (this->furnishings.empty()) f.id += "001";
        else {
            string currId = postsub(3, this->furnishings.back().id);
            f.id += currId;
        }
    }
    this->furnishings.push_back(f);
}
void Room::RemoveFurnishing(Furnishing* f)
{
    if (f == nullptr) throw runtime_error("Khong ton tai du lieu do dung!");
    bool belongsToRoom = false;
    for (const auto& furnishing : this->furnishings) {
        if (&furnishing == f) {
            belongsToRoom = true;
            break;
        }
    }
    if (!belongsToRoom) throw runtime_error("Do dung khong thuoc phong nay!");

    for (const auto& damage : this->damages) {
        if (damage.furnishing == f)
            throw runtime_error("Do dung co bao hong, khong the xoa!");
    }
    this->furnishings.erase(f);
}

void Room::AddDamage(const Damage& dam)
{
    if (dam.furnishing == nullptr)
        throw runtime_error("Bao hong phai gan voi do dung!");
    for (const auto& furnishing : this->furnishings) {
        if (&furnishing == dam.furnishing) {
            this->damages.push_back(dam);
            return;
        }
    }
    throw runtime_error("Do dung khong thuoc phong nay!");
}
void Room::RemoveDamage(Damage* dam)
{
    this->damages.erase(dam);
}

// ===============================
//              BILL
// ===============================

void Room::CreateBill(const Month& month)
{
    if (!(this->monthlyBills.empty()) && month <= this->monthlyBills.back().month) return;

    MonthlyBill bill{};
    bill.room = this;
    bill.month = month;
    bill.rentCost = this->calcRent();
    int n = this->GetTenantCount();
    bill.trashCost = Room::trashCost * n;
    bill.wifiCost = Room::internetCost;
    bill.serviceCost = Room::serviceCost * n;

    bill.damageCost = 0;
    for(const auto& d : this->damages)
        if(d.furnishing && month == d.date)
            bill.damageCost += d.furnishing->price;

    if (this->monthlyBills.empty() || this->oldElectricIndex >= this->monthlyBills.back().newElectricIndex) {
        bill.oldElectricIndex = this->oldElectricIndex;
        bill.newElectricIndex = this->newElectricIndex;
        bill.electricCost = (bill.newElectricIndex - bill.oldElectricIndex) * Room::electricUnitPrice;
    } else {
        bill.oldElectricIndex = bill.newElectricIndex = this->monthlyBills.back().newElectricIndex;
        bill.electricCost = 0;
    }

    if (this->monthlyBills.empty() || this->oldWaterIndex >= this->monthlyBills.back().newWaterIndex) {
        bill.oldWaterIndex = this->oldWaterIndex;
        bill.newWaterIndex = this->newWaterIndex;
        bill.waterCost = (bill.newWaterIndex - bill.oldWaterIndex) * Room::waterUnitPrice;
    } else {
        bill.oldWaterIndex = bill.newWaterIndex = this->monthlyBills.back().newWaterIndex;
        bill.waterCost = 0;
    }
    

    bill.totalCost = bill.rentCost + bill.serviceCost + bill.trashCost + bill.wifiCost
                    + bill.electricCost + bill.waterCost + bill.damageCost;
    
    bill.isPaid = false;
    this->monthlyBills.push_back(bill);
}
void Room::AddBill(MonthlyBill bill)
{
    if (this->GetBill(bill.month) != nullptr) 
        throw runtime_error("Hoa don thang "+to_string(bill.month)+" da ton tai");

    bill.room = this;
    if (this->monthlyBills.empty() || bill.month > this->monthlyBills.back().month) {
        this->monthlyBills.push_back(bill);
        return;
    }

    int left = 0, right = int(this->monthlyBills.size()-1), mid;
    while (left < right) {
        mid = left + (right - left) / 2;
        if (this->monthlyBills[mid].month < bill.month) left = mid + 1;
        else right = mid;
    }

    int index = left;
    if ((index > 0 && this->monthlyBills[index-1].newElectricIndex > bill.oldElectricIndex) || 
    (index+1 < this->monthlyBills.size() && this->monthlyBills[index+1].oldElectricIndex < bill.newElectricIndex)) {
        WarningBox warn(50, 30, 40, "CANH BAO SAI LECH DU LIEU", 
            "Chi so dien thang nay khong khop. Co muon luu hay khong?");
        bool confirm = warn();
        if (!confirm) return;
    }
    if ((index > 0 && this->monthlyBills[index-1].newWaterIndex > bill.oldWaterIndex) || 
    (index+1 < this->monthlyBills.size() && this->monthlyBills[index+1].oldWaterIndex < bill.newWaterIndex)) {
        WarningBox warn(50, 30, 40, "CANH BAO SAI LECH DU LIEU", 
            "Chi so nuoc thang nay khong khop. Co muon luu hay khong?");
        bool confirm = warn();
        if (!confirm) return;
    }

    this->monthlyBills.insert(left, bill);
}
void Room::RemoveBill(const Month& month)
{
    int index = this->GetBillIndex(month);
    if (index == -1) throw runtime_error("Khong ton tai hoa don thang " + to_string(month));
    this->monthlyBills.erase(this->monthlyBills.begin() + index);
}
void Room::UpdateBill(const MonthlyBill& bill)
{
    int index = this->GetBillIndex(bill.month);
    if (index == -1) throw runtime_error("Khong ton tai hoa don thang " + to_string(bill.month));

    if ((index > 0 && this->monthlyBills[index-1].newElectricIndex > bill.oldElectricIndex) || 
    (index+1 < this->monthlyBills.size() && this->monthlyBills[index+1].oldElectricIndex < bill.newElectricIndex)) {
        WarningBox warn(50, 30, 40, "CANH BAO SAI LECH DU LIEU", 
            "Chi so dien thang nay khong khop. Co muon luu hay khong?");
        bool confirm = warn();
        if (!confirm) return;
    }
    if ((index > 0 && this->monthlyBills[index-1].newWaterIndex > bill.oldWaterIndex) || 
    (index+1 < this->monthlyBills.size() && this->monthlyBills[index+1].oldWaterIndex < bill.newWaterIndex)) {
        WarningBox warn(50, 30, 40, "CANH BAO SAI LECH DU LIEU", 
            "Chi so nuoc thang nay khong khop. Co muon luu hay khong?");
        bool confirm = warn();
        if (!confirm) return;
    }

    this->monthlyBills[index] = bill;
    this->monthlyBills[index].room = this;
}
MonthlyBill* Room::GetBill(const Month& m) const
{
    int index = this->GetBillIndex(m);
    if (index == -1) return nullptr;
    return const_cast<MonthlyBill*>(&this->monthlyBills[index]);
}
int Room::GetBillIndex(const Month& m) const
{
    if (this->monthlyBills.empty()) return -1;

    int left = 0, right = this->monthlyBills.size() - 1;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (this->monthlyBills[mid].month == m) return mid;
        else if (this->monthlyBills[mid].month < m) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
void Room::PayBill(const Month& month, const Date& paymentDate)
{
    MonthlyBill* bill = this->GetBill(month);
    if (bill == nullptr) throw runtime_error("Khong ton tai hoa don thang " + to_string(month.month) + "/" + to_string(month.year));
    if (bill->isPaid) throw runtime_error("Hoa don thang " + to_string(month.month) + "/" + to_string(month.year) + " da duoc thanh toan");
    
    bill->isPaid = true;
    bill->paymentDate = paymentDate;
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
// ostream& operator<<(ostream& o, const Room& room)
// {
//     o << std::left << BORDER_V
//        << std::setw(ROOM_TYPE_W) << room.GetRoomType().substr(0, ROOM_TYPE_W) << BORDER_V
//        << std::setw(ROOM_ID_W) << room.id.substr(0, ROOM_ID_W) << BORDER_V
//        << std::setw(ROOM_NAME_W) << room.name.substr(0, ROOM_NAME_W) << BORDER_V
//        << std::setw(ROOM_CAP_W) << std::to_string(room.GetTenantCount()) + "/" + std::to_string(room.capacity) << BORDER_V
//        << std::setw(ROOM_FLOOR_W) << room.floor << BORDER_V
//        << std::setw(ROOM_PRICE_W) << std::to_string(room.GetRentPrice()).substr(0, ROOM_PRICE_W) << BORDER_V
//        << std::setw(ROOM_AVAILABLE_W) << (room.IsRented() ? (room.IsFull() ? "HET" : "CON") : "TRONG") << BORDER_V;
//     return o;
// }