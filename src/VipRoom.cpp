#include "VipRoom.h"
using namespace std;
ull VipRoom::stdRentPrice = 3000000;

VipRoom::VipRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{
    this->rentPrice = VipRoom::stdRentPrice;
}

VipRoom::~VipRoom(){}

std::string VipRoom::GetRoomType() const 
{
    return "Vip";
}
RoomType VipRoom::Type() const
{
    return RoomType::Vip;
}
ull VipRoom::calcRent() const
{
    if (this->GetTenantCount() == 1) return rentPrice;
    return ull((double) rentPrice * (1 + 0.1 * (this->GetTenantCount() - 1))); 
}
ostream& operator<<(ostream& o, const VipRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}