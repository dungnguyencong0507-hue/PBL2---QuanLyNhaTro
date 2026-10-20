#include <string.h>
#include <iostream>
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Room.h"

class Nhatro
{
    public:
        static ull electricUnitPrice;           //gia dien theo don vi (VND/kWh)
        static ull waterUnitPrice;              //gia nuoc theo don vi (VND/m3)
        
    private:
        std::string address;
        std::string phone;
        std::vector<std::unique_ptr<Room>> rooms;

    public:
        Nhatro();
        ~Nhatro();

        void SetAddress(const std::string& newAddress);
        void SetPhone(const std::string& newPhone);

        const std::string& GetAddress() const;
        const std::string& GetPhone() const;
        int GetNumRoom() const;

        Room* AddRoom(std::unique_ptr<Room> room);
        bool RemoveRoom(Room* room);

        void AvailableRoom() const;
};