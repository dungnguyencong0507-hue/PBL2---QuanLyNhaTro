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
    string name;
    string address;
    string phone;
    List<unique_ptr<Room>> rooms;
    List<unique_ptr<Tenant>> tenants;
    List<unique_ptr<MonthlyBill>> bills;

    Nhatro(const string&, const string&, const string&);

public:
    static Nhatro& GetInstance(const string& ="Nha tro KHA BI", 
        const string& ="54 Nguyen Luong Bang", const string& ="0123456789");
    ~Nhatro();

    void SetName(const string&);
    void SetAddress(const string&);
    void SetPhone(const string&);
    void SetDate(const Date&);

    const string& GetName() const;
    const string& GetAddress() const;
    const string& GetPhone() const;
    const Date& GetDate() const;
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

    void LoadRoomData(const string&, const string&, const string&, const string&);
    void SaveRoomData(const string&, const string&, const string&, const string&);

    void LoadTenantData(const string&);
    void SaveTenantData(const string&) const;
};