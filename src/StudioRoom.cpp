#include "StudioRoom.h"
using namespace std;
ull StudioRoom::stdRentPrice = 2500000;

StudioRoom::StudioRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{}

StudioRoom::~StudioRoom(){}

std::string StudioRoom::GetRoomType() const 
{
    return "Studio";
}

ull StudioRoom::GetRentPrice()  const
{
    return StudioRoom::stdRentPrice;
}
void StudioRoom::SetRentPrice(const ull& newPrice)
{
    StudioRoom::stdRentPrice = newPrice;
}
ull StudioRoom::calcRent() const
{
    return StudioRoom::stdRentPrice; 
}
ostream& operator<<(ostream& o, const StudioRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}