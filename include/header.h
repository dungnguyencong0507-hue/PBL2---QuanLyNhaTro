#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>

#define ROOM_ID_W           8
#define ROOM_NAME_W         12
#define ROOM_CAP_W          9
#define ROOM_FLOOR_W        5
#define ROOM_PRICE_W        12
#define ROOM_TYPE_W         12
#define ROOM_AVAILABLE_W    10

#define TENANT_ID_W         8
#define TENANT_NAME_W       15
#define TENANT_GENDER_W     9
#define TENANT_PHONE_W      12
#define TENANT_CCCD_W       15
#define TENANT_ROOM_W       14
#define TENANT_TMP_RSD_W    15

#define FIELD_W 15
#define COST_W 10

#define F_ID_W 8
#define F_NAME_W 15
#define F_PRICE_W 15

#define DAM_DATE_W 10
#define DAM_FURN_W 15
#define DAM_DESCRIPTION_W 50
#define DAM_RESOLUTION_W 30

namespace border {
    inline constexpr char V[] = "│";
    inline constexpr char H[] = "─";
    inline constexpr char TL[] = "┌";
    inline constexpr char TR[] = "┐";
    inline constexpr char BL[] = "└";
    inline constexpr char BR[] = "┘";
    inline constexpr char DT[] = "┬";
    inline constexpr char UT[] = "┴";
    inline constexpr char RT[] = "├";
    inline constexpr char LT[] = "┤";
    inline constexpr char CROSS[] = "┼";

    inline constexpr char V2[] = "║";
    inline constexpr char H2[] = "═";
    inline constexpr char TL2[] = "╔";
    inline constexpr char TR2[] = "╗";
    inline constexpr char BL2[] = "╚";
    inline constexpr char BR2[] = "╝";
    inline constexpr char DT2[] = "╦";
    inline constexpr char UT2[] = "╩";
    inline constexpr char RT2[] = "╠";
    inline constexpr char LT2[] = "╣";
    inline constexpr char CROSS2[] = "╬";

    inline std::string Repeat(int count, const char* symbol)
    {
        std::string result;
        for (int i = 0; i < count; ++i)
            result += symbol;
        return result;
    }
}

inline void RoomTableInit()
{
    std::cout << border::TL 
        << border::Repeat(ROOM_TYPE_W, border::H)      << border::DT
        << border::Repeat(ROOM_ID_W, border::H)        << border::DT
        << border::Repeat(ROOM_NAME_W, border::H)      << border::DT
        << border::Repeat(ROOM_CAP_W, border::H)       << border::DT
        << border::Repeat(ROOM_FLOOR_W, border::H)     << border::DT
        << border::Repeat(ROOM_PRICE_W, border::H)     << border::DT
        << border::Repeat(ROOM_AVAILABLE_W, border::H)
        << border::TR << std::endl;

    std::cout << std::left << border::V
        << std::setw(ROOM_TYPE_W)    << "Loai phong"     << border::V
        << std::setw(ROOM_ID_W)      << "Ma phong"       << border::V
        << std::setw(ROOM_NAME_W)    << "Phong"          << border::V
        << std::setw(ROOM_CAP_W)     << "So luong"       << border::V
        << std::setw(ROOM_FLOOR_W)   << "Tang"           << border::V
        << std::setw(ROOM_PRICE_W)   << "Gia thue"       << border::V
        << std::setw(ROOM_AVAILABLE_W) << "Trang thai"   << border::V
        << std::endl;

    std::cout << border::RT
        << border::Repeat(ROOM_TYPE_W, border::H)      << border::CROSS
        << border::Repeat(ROOM_ID_W, border::H)        << border::CROSS
        << border::Repeat(ROOM_NAME_W, border::H)      << border::CROSS
        << border::Repeat(ROOM_CAP_W, border::H)       << border::CROSS
        << border::Repeat(ROOM_FLOOR_W, border::H)     << border::CROSS
        << border::Repeat(ROOM_PRICE_W, border::H)     << border::CROSS
        << border::Repeat(ROOM_AVAILABLE_W, border::H)
        << border::LT << std::endl;
}

inline void RoomTableEnd()
{
    std::cout << border::BL 
        << border::Repeat(ROOM_TYPE_W, border::H)      << border::UT
        << border::Repeat(ROOM_ID_W, border::H)        << border::UT
        << border::Repeat(ROOM_NAME_W, border::H)      << border::UT
        << border::Repeat(ROOM_CAP_W, border::H)       << border::UT
        << border::Repeat(ROOM_FLOOR_W, border::H)     << border::UT
        << border::Repeat(ROOM_PRICE_W, border::H)     << border::UT
        << border::Repeat(ROOM_AVAILABLE_W, border::H)
        << border::BR << std::endl;
}

inline void TenantTableInit()
{
    std::cout << border::TL
        << border::Repeat(TENANT_ID_W, border::H)      << border::DT
        << border::Repeat(TENANT_CCCD_W, border::H)    << border::DT
        << border::Repeat(TENANT_PHONE_W, border::H)   << border::DT
        << border::Repeat(TENANT_GENDER_W, border::H)  << border::DT
        << border::Repeat(TENANT_ROOM_W, border::H)    << border::DT
        << border::Repeat(TENANT_TMP_RSD_W, border::H)
        << border::TR << std::endl;

    std::cout << std::left << border::V
        << std::setw(TENANT_ID_W)        << "Ma khach"       << border::V
        << std::setw(TENANT_CCCD_W)      << "CCCD"           << border::V
        << std::setw(TENANT_PHONE_W)     << "SDT"            << border::V
        << std::setw(TENANT_GENDER_W)    << "Gioi tinh"      << border::V
        << std::setw(TENANT_ROOM_W)      << "Da thue phong"  << border::V
        << std::setw(TENANT_TMP_RSD_W)   << "Dang ki tam tru"<< border::V 
        << std::endl;

    std::cout << border::RT
        << border::Repeat(TENANT_ID_W, border::H)      << border::CROSS
        << border::Repeat(TENANT_CCCD_W, border::H)    << border::CROSS
        << border::Repeat(TENANT_PHONE_W, border::H)   << border::CROSS
        << border::Repeat(TENANT_GENDER_W, border::H)  << border::CROSS
        << border::Repeat(TENANT_ROOM_W, border::H)    << border::CROSS
        << border::Repeat(TENANT_TMP_RSD_W, border::H)
        << border::LT << std::endl;
}

inline void TenantTableEnd()
{
    std::cout << border::BL
        << border::Repeat(TENANT_ID_W, border::H)      << border::UT
        << border::Repeat(TENANT_CCCD_W, border::H)    << border::UT
        << border::Repeat(TENANT_PHONE_W, border::H)   << border::UT
        << border::Repeat(TENANT_GENDER_W, border::H)  << border::UT
        << border::Repeat(TENANT_ROOM_W, border::H)    << border::UT
        << border::Repeat(TENANT_TMP_RSD_W, border::H)
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