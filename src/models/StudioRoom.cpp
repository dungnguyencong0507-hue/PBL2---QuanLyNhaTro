#include "StudioRoom.h"
using namespace std;
ull StudioRoom::stdRentPrice = 2500000;

StudioRoom::StudioRoom(const string& id, const string& name, const int& capacity, const int& floor)
    : Room(id, name, capacity, floor)
{
    this->rentPrice = StudioRoom::stdRentPrice;
}

StudioRoom::~StudioRoom(){}

std::string StudioRoom::GetRoomType() const 
{
    return "Studio";
}
RoomType StudioRoom::Type() const
{
    return RoomType::Studio;
}

ull StudioRoom::calcRent() const
{
    return rentPrice; 
}
ostream& operator<<(ostream& o, const StudioRoom& room)
{
    o << static_cast<const Room&>(room);
    return o;
}