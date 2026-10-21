#pragma once
#include "Room.h"

class VipRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    VipRoom(const std::string& id = "103", const std::string& name = "Phong 3", const int& capacity = 5, const int& floor = 1);
    ~VipRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() const override;
    ull calcRent() const override;
    std::string GetRoomType() const override;

    friend std::ostream& operator<<(std::ostream&, const VipRoom&);
};