#pragma once
#include "Room.h"

class BasicRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    BasicRoom(/* args */);
    ~BasicRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() const override;
    ull calcRent() const override;

    friend std::ostream& operator<<(std::ostream&, const BasicRoom&);
};
