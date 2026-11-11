#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::string;

using ull = unsigned long long;

#define ROOM_TYPE_W         12
#define ROOM_ID_W           8
#define ROOM_NAME_W         25
#define ROOM_CAP_W          8
#define ROOM_FLOOR_W        5
#define ROOM_PRICE_W        15
#define ROOM_AVAILABLE_W    10

#define TENANT_ID_W         8
#define TENANT_NAME_W       25
#define TENANT_GENDER_W     9
#define TENANT_PHONE_W      12
#define TENANT_CCCD_W       13
#define TENANT_ROOM_W       22
#define TENANT_TMP_RSD_W    15
#define TENANT_NOTE_W       15

#define FIELD_W 15
#define COST_W 10

#define F_ID_W 8
#define F_NAME_W 15
#define F_PRICE_W 15

#define DAM_DATE_W 10
#define DAM_FURN_W 15
#define DAM_DESCRIPTION_W 50
#define DAM_COMPEN_W 10

#define BORDER_V "│"
#define BORDER_H "─"
#define BORDER_TL "┌"
#define BORDER_TR "┐"
#define BORDER_BL "└"
#define BORDER_BR "┘"
#define BORDER_DT "┬"
#define BORDER_UT "┴"
#define BORDER_RT "├"
#define BORDER_LT "┤"
#define BORDER_CROSS "┼"

#define BORDER_V2 "║"
#define BORDER_H2 "═"
#define BORDER_TL2 "╔"
#define BORDER_TR2 "╗"
#define BORDER_BL2 "╚"
#define BORDER_BR2 "╝"
#define BORDER_DT2 "╦"
#define BORDER_UT2 "╩"
#define BORDER_RT2 "╠"
#define BORDER_LT2 "╣"
#define BORDER_CROSS2 "╬"

inline string Repeat(int count, const char* symbol)
{
    string result;
    for (int i = 0; i < count; ++i)
        result += symbol;
    return result;
}

inline string presub(int width, const string& s)
{
    if (int(s.size()) < width) return s;
    return s.substr(0, width);
}

inline string postsub(int width, const string& s)
{
    if (int(s.size()) < width) return s;
    return s.substr(s.size() - width, width);
}

inline string center(const string& text, int width) {
    int len = text.size();
    if (len >= width) return text;
    int pad = (width - len) / 2;
    return string(pad, ' ') + text + string(width - len - pad, ' ');
}

string operator++(string& id);
