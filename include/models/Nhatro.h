#pragma once
#include <header.h>
#include <memory>
#include <fstream>
#include <Room.h>
#include <BasicRoom.h>
#include <VipRoom.h>
#include <StudioRoom.h>
#include <Tenant.h>
#include <Models.h>
#include <algorithm>
#include <List.h>
#include <string>

using std::ostream;
using std::string;
using std::unique_ptr;

class Nhatro
{
public:
    static Date today;
    
private:
    List<unique_ptr<Room>> rooms;
    List<unique_ptr<Tenant>> tenants;
    List<unique_ptr<MonthlyBill>> bills;

    Nhatro();

public:
    static Nhatro& GetInstance();
    ~Nhatro();

    int GetNumRoom() const;
    int GetNumTenant() const;
    const List<unique_ptr<Room>>& GetRooms();
    const List<unique_ptr<Tenant>>& GetTenants();

    void AddRoom(unique_ptr<Room>);
    void RemoveRoom(Room*);

    void AddTenant(unique_ptr<Tenant>);
    void TenantCheckInRoom(Tenant*, Room*);
    void TenantCheckOutRoom(Tenant*, Room*);

    void SortRoomsById();
    int FindRoom(const string&);
    int FindTenant(const string&);
    friend ostream& operator<<(ostream&, const Nhatro&);

    void LoadGeneralData(const string&);
    void SaveGeneralData(const string&) const;

    void LoadRoomData(const string&, const string&, const string&, const string&);
    void SaveRoomData(const string&, const string&, const string&, const string&);

    void LoadTenantData(const string&);
    void SaveTenantData(const string&) const;
};