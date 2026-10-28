#include <Nhatro.h>
#include <algorithm>
using namespace std;

Nhatro::Nhatro(const string& name, const string& address, const string& phone)
    : name(name), address(address), phone(phone)
{}

Nhatro::~Nhatro()
{}

void Nhatro::SetAddress(const string& newAddress)
{
    this->address = newAddress;
}

void Nhatro::SetPhone(const string& newPhone)
{
    this->phone = newPhone;
}

const string& Nhatro::GetName() const
{
    return this->name;
}

const string& Nhatro::GetAddress() const
{
    return this->address;
}

const string& Nhatro::GetPhone() const
{
    return this->phone;
}

int Nhatro::GetNumRoom() const
{
    return this->rooms.size();
}

int Nhatro::GetNumTenant() const
{
    return this->tenants.size();
}

Room* Nhatro::GetRoom(int i)
{
    return this->rooms.at(i).get();
}

Tenant* Nhatro::GetTenant(int i)
{
    return this->tenants.at(i).get();
}

bool Nhatro::AddRoom(unique_ptr<Room> newRoom)
{
    if (!newRoom) return false;
    this->rooms.push_back(move(newRoom));
    return true;
}

bool Nhatro::RemoveRoom(Room* targetRoom)
{
    if (targetRoom->GetTenantCount() > 0) return false;
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        if (it->get() == targetRoom){
            this->rooms.erase(it);
            return true;
        }
    }
    return false;
}

bool Nhatro::AddTenant(unique_ptr<Tenant> newTenant)
{
    if (newTenant == nullptr) return false;
    if (find(this->tenants.begin(), this->tenants.end(), newTenant) != this->tenants.end()) return false;
    this->tenants.push_back(std::move(newTenant));
    return true;
}

bool Nhatro::TenantCheckInRoom(Tenant* tenant, Room* room)
{
    if (tenant == nullptr || room == nullptr) throw runtime_error("Khong ton tai du lieu");
    return room->AddTenant(tenant);
}
bool Nhatro::TenantCheckOutRoom(Tenant* tenant, Room* room)
{
    if (tenant == nullptr || room == nullptr) throw runtime_error("Khong ton tai du lieu");
    return room->RemoveTenant(tenant);
}

void Nhatro::ShowRooms() const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        cout << *it->get();
        cout << endl;
    }
    RoomTableEnd();
}

void Nhatro::ShowRoomsByType(RoomType rt) const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++)
    if (it->get()->Type() == rt){
        cout << *it->get();
        cout << endl;
    }
    RoomTableEnd();
}

void Nhatro::ShowAvailableRooms() const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        if (!(it->get()->IsFull())){
            cout << *it->get();
            cout << endl;
        }
    }
    RoomTableEnd();
}

void Nhatro::ShowTenants() const
{
    TenantTableInit();
    for (auto it=this->tenants.begin(); it!=this->tenants.end(); it++){
        cout << *it->get();
        cout << endl;
    }
    TenantTableEnd();
}

void Nhatro::SortRoomsByPrice(bool ascending)
{
    std::stable_sort(rooms.begin(), rooms.end(),
        [ascending](const std::unique_ptr<Room>& a,
                    const std::unique_ptr<Room>& b)
        {
            if (a->GetRentPrice() == b->GetRentPrice())
                return a->GetId() < b->GetId();

            return ascending
                ? a->GetRentPrice() < b->GetRentPrice()
                : a->GetRentPrice() > b->GetRentPrice();
        });
}
void Nhatro::SortRoomsById(bool tangdan)
{
    std::stable_sort(rooms.begin(), rooms.end(),
        [tangdan](const std::unique_ptr<Room>& a,
                  const std::unique_ptr<Room>& b)
        {
            const string IDa = a->GetId();
            const string IDb = b->GetId();

            return tangdan
                ? IDa < IDb
                : IDa > IDb;
        });
}

ostream& operator<<(ostream& o, const Nhatro& nhatro)
{
    o << "Thong tin " << nhatro.name << ":\n"
        << "Dia chi: " << nhatro.address << endl
        << "Lien he: " << nhatro.phone << endl;
    return o;
}

Room* Nhatro::FindRoom(const std::string& id)
{
    int left = 0, right = this->rooms.size() - 1;
    while (left <= right) { 
        int mid = left + (right - left) / 2;
        if (this->rooms[mid]->GetId() == id) {
            return this->rooms[mid].get();
        }
        else if (this->rooms[mid]->GetId() < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr;
}
    
Tenant* Nhatro::FindTenant(const std::string& id)
{
    int left = 0, right = this->tenants.size() - 1;
    while (left <= right) { 
        int mid = left + (right - left) / 2;
        if (this->tenants[mid]->GetId() == id) {
            return this->tenants[mid].get();
        }
        else if (this->tenants[mid]->GetId() < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr;
}