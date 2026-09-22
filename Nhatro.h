#include <string.h>
#include <iostream>
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Room.h"

class Nhatro
{
    private:
        std::string address;
        std::string phone;
        int numFloor = 0;
        std::vector<std::unique_ptr<Room>> rooms;

    public:
        Nhatro();
        ~Nhatro();

        void SetAddress(const std::string& newAddress);
        void SetPhone(const std::string& newPhone);
        void SetNumFloor(int newNumFloor);

        const std::string& GetAddress() const;
        const std::string& GetPhone() const;
        int GetNumFloor() const;
        int GetNumRoom() const;

        Room* AddRoom(std::unique_ptr<Room> room);
        bool RemoveRoom(Room* room);

        int AvailableRoom() const;
};