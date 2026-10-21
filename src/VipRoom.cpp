#include "VipRoom.h"
using namespace std;
ull VipRoom::stdRentPrice = 3000000;

VipRoom::VipRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{}

VipRoom::~VipRoom(){}

std::string VipRoom::GetRoomType() const 
{
    return "Vip";
}

ull VipRoom::GetRentPrice()  const
{
    return VipRoom::stdRentPrice;
}
void VipRoom::SetRentPrice(const ull& newPrice)
{
    VipRoom::stdRentPrice = newPrice;
}
ull VipRoom::calcRent() const
{
    return VipRoom::stdRentPrice; 
}
ostream& operator<<(ostream& o, const VipRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}