#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Room.h"
class RoomManager
{
private:
    std::vector<std::unique_ptr<Room>> rooms;
public:
bool AddRoom(std::unique_ptr<Room>);    //them phong
bool RemoveRoom(Room*);                 //xoa phong (neu phong dang co nguoi o thi khong xoa duoc)
Room* FindRoomById(const std::string&); //tim phong theo ma phong
bool LoadRoomData(std::string&);
void SaveRoomData(std::string&);
void ShowRoomDetail(Room*);
};