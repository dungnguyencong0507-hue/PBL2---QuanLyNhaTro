#include <Nhatro.h>
#include <header.h>
#include <iomanip>
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

bool Nhatro::AddRoom(unique_ptr<Room> newRoom)
{
    if (newRoom == nullptr) return false;
    if (find(this->rooms.begin(), this->rooms.end(), newRoom) != this->rooms.end()) return false;
    this->rooms.push_back(newRoom);
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

bool Nhatro::AddTenant(unique_ptr<Tenant> newTenant, Room* choiceRoom)
{
    if (newTenant == nullptr  || choiceRoom == nullptr) return false;
    for(const auto& t : this->tenants)
        if(t.get() == newTenant.get()) return false;
    
    if(choiceRoom->IsFull()) return false; 
    if(newTenant->IsRenting()) return false;
    if(!choiceRoom->AddTenant(newTenant.get())) return false;
    newTenant->SetRoom(choiceRoom);
    this->tenants.push_back(move(newTenant));
    return true;
}

void RoomTableInit()
{
    cout << left 
        << setw(ROOM_TYPE_W) << "Loai phong"
        << setw(ROOM_ID_W) << "Ma phong"
        << setw(ROOM_NAME_W) << "Phong"
        << setw(ROOM_CAP_W) << "So luong"
        << setw(ROOM_FLOOR_W) << "Tang"
        << setw(ROOM_PRICE_W) << "Gia thue"
        << setw(ROOM_AVAILABLE_W) << "Trang thai" << endl;
}

void Nhatro::ShowRooms() const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        cout << it->get();
        cout << endl;
    }
}

void Nhatro::ShowRoomsOfType(RoomType rt) const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++)
    if (it->get()->Type() == rt){
        cout << it->get();
        cout << endl;
    }
}

void Nhatro::ShowAvailableRooms() const
{
    RoomTableInit();
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        if (!(it->get()->IsFull())){
            cout << it->get();
            cout << endl;
        }
    }
}

void TenantTableInit()
{
    cout << left 
        << setw(TENANT_ID_W) << "Ma khach"
        << setw(TENANT_CCCD_W) << "CCCD"
        << setw(TENANT_PHONE_W) << "SDT"
        << setw(TENANT_GENDER_W) << "Gioi tinh"
        << setw(TENANT_ROOM_W) << "Da thue phong"
        << setw(TENANT_TMP_RSD_W) << "Dang ki tam tru" << endl;
}

void Nhatro::ShowTenants() const
{
    TenantTableInit();
    for (auto it=this->tenants.begin(); it!=this->tenants.end(); it++){
        cout << it->get();
        cout << endl;
    }
}

ostream& operator<<(ostream& o, const Nhatro& nhatro)
{
    o << "Thong tin " << nhatro.name << ":\n"
        << "Dia chi: " << nhatro.address << endl
        << "Lien he: " << nhatro.phone << endl;
    return o;
}