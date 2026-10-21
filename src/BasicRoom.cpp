#include "BasicRoom.h"
using namespace std;
ull BasicRoom::stdRentPrice = 2000000;

BasicRoom::BasicRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{}

BasicRoom::~BasicRoom(){}

std::string BasicRoom::GetRoomType() const 
{
    return "Basic";
}

ull BasicRoom::GetRentPrice()  const
{
    return BasicRoom::stdRentPrice;
}
void BasicRoom::SetRentPrice(const ull& newPrice)
{
    BasicRoom::stdRentPrice = newPrice;
}
ull BasicRoom::calcRent() const
{
    return BasicRoom::stdRentPrice; 
}
ostream& operator<<(ostream& o, const BasicRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}