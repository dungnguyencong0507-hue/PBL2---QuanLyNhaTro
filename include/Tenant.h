#pragma once
#include <string>
#include <iostream>
#include <Room.h>

enum Gender
{
    Male,
    Female
};

class Tenant
{
protected:
    std::string id;
    std::string name;
    Gender gender;
    std::string phone;
    std::string cccdID;
    std::string hometown;
    Room* room;
    bool isTemporaryResidence;

public:
    Tenant(const std::string&, const std::string&, const Gender, const std::string&, const std::string&);
    virtual ~Tenant();

    bool IsRenting() const;

    Room* GetRoom() const;
    void SetRoom(Room*);

    bool GetTemporaryResidence() const;
    void SetTemporaryResidence(bool);
    
    virtual void GetDiscount();

    friend std::ostream& operator<<(std::ostream&, const Tenant&);
};