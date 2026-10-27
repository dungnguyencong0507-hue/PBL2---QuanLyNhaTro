#include "BasicRoom.h"
using namespace std;
ull BasicRoom::stdRentPrice = 2000000;

BasicRoom::BasicRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{
    this->rentPrice = BasicRoom::stdRentPrice;
}

BasicRoom::~BasicRoom(){}

std::string BasicRoom::GetRoomType() const 
{
    return "Basic";
}
ull BasicRoom::calcRent() const
{
    // if(this->GetTenantCount() == 0) return 0;
    if (this->GetTenantCount() == 1) return rentPrice;
    return ull((double) rentPrice * (1 + 0.1 * (this->GetTenantCount() - 1))); 
}
RoomType BasicRoom::Type() const
{
    return RoomType::Basic;
}
ostream& operator<<(ostream& o, const BasicRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}