#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>

#define ROOM_ID_W           7
#define ROOM_NAME_W         10
#define ROOM_CAP_W          5
#define ROOM_FLOOR_W        5
#define ROOM_PRICE_W        12
#define ROOM_TYPE_W         12
#define ROOM_AVAILABLE_W    5

#define TENANT_ID_W         5
#define TENANT_NAME_W       15
#define TENANT_GENDER_W     7
#define TENANT_PHONE_W      12
#define TENANT_CCCD_W       15
#define TENANT_ROOM_W       7
#define TENANT_TMP_RSD_W    5

namespace border {
    constexpr char V      = (char)179; // │
    constexpr char H      = (char)196; // ─
    constexpr char TL     = (char)218; // ┌
    constexpr char TR     = (char)191; // ┐
    constexpr char BL     = (char)192; // └
    constexpr char BR     = (char)217; // ┘
    constexpr char DT     = (char)194; // ┬
    constexpr char UT     = (char)193; // ┴
    constexpr char RT     = (char)195; // ├
    constexpr char LT     = (char)180; // ┤
    constexpr char CROSS  = (char)197; // ┼

    constexpr char V2     = (char)186; // ║
    constexpr char H2     = (char)205; // ═
    constexpr char TL2    = (char)201; // ╔
    constexpr char TR2    = (char)187; // ╗
    constexpr char BL2    = (char)200; // ╚
    constexpr char BR2    = (char)188; // ╝
    constexpr char DT2    = (char)203; // ╦
    constexpr char UT2    = (char)202; // ╩
    constexpr char RT2    = (char)204; // ╠
    constexpr char LT2    = (char)185; // ╣
    constexpr char CROSS2 = (char)206; // ╬
}

inline void RoomTableInit()
{
    std::cout << border::TL 
        << std::string(border::H, ROOM_TYPE_W)      << border::DT
        << std::string(border::H, ROOM_ID_W)        << border::DT
        << std::string(border::H, ROOM_NAME_W)      << border::DT
        << std::string(border::H, ROOM_CAP_W)       << border::DT
        << std::string(border::H, ROOM_FLOOR_W)     << border::DT
        << std::string(border::H, ROOM_PRICE_W)     << border::DT
        << std::string(border::H, ROOM_AVAILABLE_W) << border::DT
        << border::TR << std::endl;

    std::cout << std::left
        << std::setw(ROOM_TYPE_W)    << "Loai phong"     << border::V
        << std::setw(ROOM_ID_W)      << "Ma phong"       << border::V
        << std::setw(ROOM_NAME_W)    << "Phong"          << border::V
        << std::setw(ROOM_CAP_W)     << "So luong"       << border::V
        << std::setw(ROOM_FLOOR_W)   << "Tang"           << border::V
        << std::setw(ROOM_PRICE_W)   << "Gia thue"       << border::V
        << std::setw(ROOM_AVAILABLE_W) << "Trang thai"   << border::V
        << std::endl;
}

inline void RoomTableEnd()
{
    std::cout << border::BL 
        << std::string(border::H, ROOM_TYPE_W)      << border::UT
        << std::string(border::H, ROOM_ID_W)        << border::UT
        << std::string(border::H, ROOM_NAME_W)      << border::UT
        << std::string(border::H, ROOM_CAP_W)       << border::UT
        << std::string(border::H, ROOM_FLOOR_W)     << border::UT
        << std::string(border::H, ROOM_PRICE_W)     << border::UT
        << std::string(border::H, ROOM_AVAILABLE_W) << border::UT
        << border::BR << std::endl;
}

inline void TenantTableInit()
{
    std::cout << border::TL
        << std::string(border::H, TENANT_ID_W)      << border::DT
        << std::string(border::H, TENANT_CCCD_W)    << border::DT
        << std::string(border::H, TENANT_PHONE_W)   << border::DT
        << std::string(border::H, TENANT_GENDER_W)  << border::DT
        << std::string(border::H, TENANT_ROOM_W)    << border::DT
        << std::string(border::H, TENANT_TMP_RSD_W) << border::DT
        << border::TR << std::endl;
    std::cout << std::left 
        << std::setw(TENANT_ID_W)        << "Ma khach"       << border::V
        << std::setw(TENANT_CCCD_W)      << "CCCD"           << border::V
        << std::setw(TENANT_PHONE_W)     << "SDT"            << border::V
        << std::setw(TENANT_GENDER_W)    << "Gioi tinh"      << border::V
        << std::setw(TENANT_ROOM_W)      << "Da thue phong"  << border::V
        << std::setw(TENANT_TMP_RSD_W)   << "Dang ki tam tru"<< border::V 
        << std::endl;
}

inline void TenantTableEnd()
{
    std::cout << border::BL
        << std::string(border::H, TENANT_ID_W)      << border::UT
        << std::string(border::H, TENANT_CCCD_W)    << border::UT
        << std::string(border::H, TENANT_PHONE_W)   << border::UT
        << std::string(border::H, TENANT_GENDER_W)  << border::UT
        << std::string(border::H, TENANT_ROOM_W)    << border::UT
        << std::string(border::H, TENANT_TMP_RSD_W) << border::UT
        << border::BR << std::endl;
}

inline bool isNum(std::string& s)
{
    for (unsigned char c : s){
        if (c < '0' || c > '9') return false;
    }
    return !s.empty();
}

inline bool isName(const std::string& s)
{
    for (int i=0; i<s.size(); i++){
        if (isalpha(s[i])) continue;
        if (s[i] == ' ' && i > 0 && s[i-1] != ' ') continue;
        return false;
    }
    return !s.empty() && s.length() < 100;
}