#include <Room.h>

class StudioRoom : public Room
{
private:
    static ull stdRentPrice;
public:
    StudioRoom(/* args */);
    ~StudioRoom();

    void SetRentPrice(const ull&) override;
    ull GetRentPrice() override;
    ull calcRent() override;
};