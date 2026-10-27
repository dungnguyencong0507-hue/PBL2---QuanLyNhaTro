#include <Tenant.h>
#include <iomanip>
#include <header.h>
using namespace std;

Tenant::Tenant(const std::string& id, const string& name, const string& phone, const Gender gend, const string& cccd, const string& home)
    : id(id), name(name), phone(phone), gender(gend), cccdID(cccd), hometown(home)
{
    this->room = nullptr;
    this->isTemporaryResidence = false;
}  

Tenant::~Tenant()
{}

bool Tenant::IsRenting() const
{
    if (this->room != nullptr) return true;
    return false;
}

string Tenant::GetId() const
{
    return this->id;
}

string Tenant::GetName() const
{
    return this->name;
}

string Tenant::GetPhone() const
{
    return this->phone;
}

string Tenant::GetCCCD() const
{
    return this->cccdID;
}

string Tenant::GetHometown() const
{
    return this->hometown;
}

Gender Tenant::GetGender() const
{
    return this->gender;
}

Room* Tenant::GetRoom() const
{
    return this->room;
}

void Tenant::SetName(const string& name)
{
    this->name = name;
}

void Tenant::SetId(const string& id)
{
    this->id = id;
}

void Tenant::SetPhone(const string& phone)
{
    this->phone = phone;
}

void Tenant::SetCCCD(const string& cccd)
{
    this->cccdID = cccd;
}

void Tenant::SetHometown(const string& hometown)
{
    this->hometown = hometown;
}

void Tenant::SetGender(const Gender& gender)
{
    this->gender = gender;
}

void Tenant::SetRoom(Room* room)
{
    this->room = room;
}

bool Tenant::GetTemporaryResidence() const 
{
    return this->isTemporaryResidence;
}

void Tenant::SetTemporaryResidence(bool tmpRsd)
{
    this->isTemporaryResidence = tmpRsd;
}

void Tenant::GetDiscount()
{}

ostream& operator<<(ostream& o, const Tenant& tenant)
{
    o << std::left << border::V
        << setw(TENANT_ID_W) << tenant.id << border::V
        << setw(TENANT_CCCD_W) << tenant.cccdID << border::V
        << setw(TENANT_PHONE_W) << tenant.phone << border::V
        << setw(TENANT_GENDER_W) << (tenant.gender == Male ? "Nam" : "Nu") << border::V
        << setw(TENANT_ROOM_W) << (tenant.IsRenting() ? tenant.room->GetName() : "Khong") << border::V
        << setw(TENANT_TMP_RSD_W) << (tenant.isTemporaryResidence ? "Roi" : "Chua") << border::V;
    return o;
}