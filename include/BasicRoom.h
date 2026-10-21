#pragma once
#include <Room.h>

class BasicRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    BasicRoom(/* args */);
    ~BasicRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() override;
    ull calcRent() override;
};
