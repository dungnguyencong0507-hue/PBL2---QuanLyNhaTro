#pragma once
#include <header.h>
#include <memory>
#include <Room.h>
#include <Tenant.h>
#include <Models.h>

class Nhatro
{
private:
    std::string name;
    std::string address;
    std::string phone;
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::unique_ptr<Tenant>> tenants;

public:
    Nhatro(const std::string& ="Nha tro Aura", const std::string& ="54 Nguyen Luong Bang", const std::string& ="0123456789");
    ~Nhatro();

    void SetAddress(const std::string&);
    void SetPhone(const std::string&);

    const std::string& GetName() const;
    const std::string& GetAddress() const;
    const std::string& GetPhone() const;
    int GetNumRoom() const;
    int GetNumTenant() const;
    Room* GetRoom(int);
    Tenant* GetTenant(int);

    bool AddRoom(std::unique_ptr<Room>);
    bool RemoveRoom(Room*);

    bool AddTenant(std::unique_ptr<Tenant>);

    void ShowRooms() const;
    void ShowRoomsByType(RoomType) const;
    void ShowTenants() const;
    void ShowAvailableRooms() const;

    void SortRoomsByPrice(bool);
    void SortRoomsByAvailableSlots(bool);

    Room* FindRoom(const std::string&);
    Tenant* FindTenant(const std::string&);
    friend std::ostream& operator<<(std::ostream&, const Nhatro&);
};