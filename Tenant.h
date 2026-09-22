#pragma once
#include <string>
#include <iostream>
#include "Room.h"

enum Gender
{
    Male,
    Female
};

class Tenant
{
    private:
        std::string id;
        std::string name;
        Gender gender;
        std::string phone;
        std::string idCard;
        std::string hometown;
        Room* room = nullptr;
        bool isTemporaryResidence = false;

    public:
        Tenant();
        ~Tenant();

        bool Rent(Room&);
        bool StopHiring();
        bool IsRenting();
        void SetTemporaryResidence(bool);
        void ReportDamage(const Damage&);
        Room* GetRoom();

};