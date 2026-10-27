#include <Tenant.h>
#include <iomanip>
#include <header.h>
using namespace std;

Tenant::Tenant(const string& id, const string& name, const string& phone, const Gender gend, const string& cccd, const string& home)
    : name(name), phone(phone), gender(gend), cccdID(cccd), hometown(home)
{
    this->id = id;
    this->room = nullptr;
    this->isTemporaryResidence = false;
}  

bool Tenant::IsRenting() const
{
    if (this->room != nullptr) return true;
    return false;
}

Room* Tenant::GetRoom() const
{
    return this->room;
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

ostream& operator<<(ostream& o, const Tenant& tenant)
{
    o << left
        << setw(TENANT_ID_W) << tenant.id
        << setw(TENANT_CCCD_W) << tenant.cccdID
        << setw(TENANT_PHONE_W) << tenant.phone
        << setw(TENANT_GENDER_W) << (tenant.gender == Male ? "Nam" : "Nu")
        << setw(TENANT_ROOM_W) << (tenant.IsRenting() ? tenant.room->GetName() : "Khong")
        << setw(TENANT_TMP_RSD_W) << (tenant.isTemporaryResidence ? "Roi" : "Chua");
    return o;
}