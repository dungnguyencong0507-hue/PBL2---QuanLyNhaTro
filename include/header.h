#pragma once

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

namespace Box {
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
