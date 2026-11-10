#pragma once
#include <header.h>
#include <ConsoleUI.h>
#include <Nhatro.h>
#include <Manager.h>
#include <Room.h>
#include <Checker.h>
#include <algorithm>
#include <TenantManager.h>

void ManageRooms(Nhatro&);
void AddRoom(Nhatro&);
void RemoveRoom(Nhatro&, Room*);
void UpdateRoom(Nhatro&, Room*);
bool SearchRooms(Room*, const std::string&); // Theo ten phong
bool FilterAvailableRooms(Room*);
bool FilterRentedRooms(Room*);
bool FilterFullRooms(Room*);
bool FilterBasicRooms(Room*);
bool FilterVipRooms(Room*);
bool FilterStudioRooms(Room*);
bool CompareRoomsPrice(Room*, Room*);
bool CompareRoomsPrice1(Room*, Room*);
bool CompareRoomsCapacity(Room*, Room*);
bool CompareRoomsCapacity1(Room*, Room*);
bool CompareRoomsAvailability(Room*, Room*);
bool CompareRoomsAvailability1(Room*, Room*);

FormBox GetRoomDetails(Room*, int, int, bool = 0);

void RoomTableInit(int x, int y);
void RoomTableEnd(int x, int y);
int RoomTableWidth();
void RoomTableShow(Room* const&);

void FurnTableInit(int x, int y);
void FurnTableEnd(int x, int y);
int FurnTableWidth();
void FurnTableShow(Furnishing* const&);

void DamageTableInit(int x, int y);
void DamageTableEnd(int x, int y);
int DamageTableWidth();
void DamageTableShow(Damage* const&);
