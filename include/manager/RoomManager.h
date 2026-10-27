#pragma once
#include <iostream>
#include <Nhatro.h>
#include <Room.h>

void LoadRoomData(const std::string&, Nhatro&);
void SaveRoomData(const std::string&, Nhatro&);

void ManageRooms(Nhatro&);
void FindRooms(Nhatro&);
void ShowRoomDetails(Nhatro&);
void ShowAvailableRooms(Nhatro&);
void ShowRoomsByType(Nhatro&);
void AddRoom(Nhatro&);
void DeleteRoom(Nhatro&);
void UpdateRoom(Nhatro&);
