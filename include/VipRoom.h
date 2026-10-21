#pragma once
#include <Room.h>

class VipRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    VipRoom(/* args */);
    ~VipRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() override;
    ull calcRent() override;
};