#pragma once
#include <string>
#include <header.h>
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

    static std::string currID;

public:
    Tenant(const std::string&, const std::string&, const Gender, const std::string&, const std::string&, const std::string&);
    Tenant(const std::string&, const Gender, const std::string&, const std::string&, const std::string&);
    virtual ~Tenant();

    bool IsRenting() const;
    bool IsTemporaryResidence() const;
    bool IsLeader() const;

    std::string GetId() const;
    std::string GetName() const;
    std::string GetPhone() const;
    std::string GetCCCD() const;
    std::string GetHometown() const;
    Gender GetGender() const;
    Room* GetRoom() const;
    
    void SetName(const std::string&);
    void SetId(const std::string&);
    void SetPhone(const std::string&);
    void SetCCCD(const std::string&);
    void SetHometown(const std::string&);
    void SetGender(const Gender&);
    void SetRoom(Room*);
    void SetTemporaryResidence(bool);
    
    virtual void GetDiscount();

    // friend std::ostream& operator<<(std::ostream&, const Tenant&);
};