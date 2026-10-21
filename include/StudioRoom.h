#pragma once
#include "Room.h"

class StudioRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    StudioRoom(/* args */);
    ~StudioRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() const override;
    ull calcRent() const override;

    friend std::ostream& operator<<(std::ostream&, const StudioRoom&);
};