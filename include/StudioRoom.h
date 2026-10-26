#pragma once
#include "Room.h"

class StudioRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    StudioRoom(const std::string& id = "102", const std::string& name = "Phong 2", const int& capacity = 5, const int& floor = 1);
    ~StudioRoom();

    ull calcRent() const override;
    std::string GetRoomType() const override;

    friend std::ostream& operator<<(std::ostream&, const StudioRoom&);
};