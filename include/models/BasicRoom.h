#pragma once
#include <Room.h>

class BasicRoom : public Room
{
public:
    static ull stdRentPrice;
public:
    BasicRoom(const std::string& id = "100", const std::string& name = "Phong 1", const int& capacity = 5, const int& floor = 1);
    ~BasicRoom();

    ull calcRent() const override;
    std::string GetRoomType() const override;
    RoomType Type() const override;
    ull GetStdPrice() const override;

    // friend std::ostream& operator<<(std::ostream&, const BasicRoom&);
};
