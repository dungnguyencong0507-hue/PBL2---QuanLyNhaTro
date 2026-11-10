#include <Tenant.h>
using namespace std;

string Tenant::currID = "KH0000";
string operator++(string& id)
{
    for (int i = static_cast<int>(id.size()) - 1; i >= 0; i--) {
        if (isdigit(id[i])) {
            if (id[i] == '9') {
                id[i] = '0';
                if (i > 0 && isdigit(id[i-1])) id[i-1]++;
            } else {
                id[i]++;
                break;
            }
        }
    }
    return id;
}

Tenant::Tenant(const std::string& id, const string& name, const Gender gend, const string& phone, const string& cccd, const string& home)
    : id(id), name(name), gender(gend), phone(phone), cccdID(cccd), hometown(home)
{
    this->room = nullptr;
    this->isTemporaryResidence = false;
    currID = max(currID, id);
}

Tenant::Tenant(const string& name, const Gender gend, const string& phone, const string& cccd, const string& home)
    : Tenant(++Tenant::currID, name, gend, phone, cccd, home)
{}

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

bool Tenant::IsTemporaryResidence() const 
{
    return this->isTemporaryResidence;
}

bool Tenant::IsLeader() const
{
    if (this->GetRoom() == nullptr) return false;
    return this == this->GetRoom()->GetRoomLeader();
}

void Tenant::SetTemporaryResidence(bool tmpRsd)
{
    this->isTemporaryResidence = tmpRsd;
}

void Tenant::GetDiscount()
{}

// ostream& operator<<(ostream& o, const Tenant& tenant)
// {
//     o << std::left << BORDER_V
//         << setw(TENANT_ID_W)        << tenant.id.substr(0, TENANT_ID_W)         << BORDER_V
//         << setw(TENANT_NAME_W)      << tenant.name.substr(0, TENANT_NAME_W)     << BORDER_V
//         << setw(TENANT_CCCD_W)      << tenant.cccdID.substr(0, TENANT_CCCD_W)   << BORDER_V
//         << setw(TENANT_PHONE_W)     << tenant.phone.substr(0, TENANT_PHONE_W)   << BORDER_V
//         << setw(TENANT_GENDER_W)    << (tenant.gender == Male ? "Nam" : "Nu")   << BORDER_V
//         << setw(TENANT_ROOM_W)      << (tenant.IsRenting()
//             ? tenant.room->GetName().substr(0, TENANT_ROOM_W) : "Khong")        << BORDER_V
//         << setw(TENANT_TMP_RSD_W) << (tenant.isTemporaryResidence ? "Roi" : "Chua") << BORDER_V
//         << setw(TENANT_NOTE_W) << (tenant.IsLeader() ? "TRUONG PHONG" : "") << BORDER_V;
//     return o;
// }