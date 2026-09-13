#include <string.h>
#include <iostream>
#include "Phong.h"

class Nhatro
{
    private:
        std::string address;
        std::string phone;
        int numRoom;
        int numFloor;

    public:
        Nhatro();
        ~Nhatro();

        int AvailableRoom();
};