#pragma once
#include "Room.h"

class VipRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    VipRoom(/* args */);
    ~VipRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() const override;
    ull calcRent() const override;

    friend std::ostream& operator<<(std::ostream&, const VipRoom&);
};