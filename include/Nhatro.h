#pragma once
#include <iostream>
#include <memory>
#include <Room.h>
#include <Tenant.h>

class Nhatro
{
private:
    std::string name;
    std::string address;
    std::string phone;
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::unique_ptr<Tenant>> tenants;

public:
    Nhatro(const std::string& = "Nha tro DHBK", const std::string& = "54 Nguyen Luong Bang", const std::string& = "0123456789");
    ~Nhatro();

    void SetAddress(const std::string&);
    void SetPhone(const std::string&);

    const std::string& GetName() const;
    const std::string& GetAddress() const;
    const std::string& GetPhone() const;
    
    int GetNumRoom() const;
    bool AddRoom(std::unique_ptr<Room>);
    bool RemoveRoom(Room*);

    bool AddTenant(std::unique_ptr<Tenant>);

    void ShowRooms() const;
    void ShowTenants() const;
    void ShowAvailableRooms() const;
    friend std::ostream& operator<<(std::ostream&, const Nhatro&);
};