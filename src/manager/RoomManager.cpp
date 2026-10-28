#include <fstream>
#include <sstream>
#include <header.h>
#include <Nhatro.h>
#include <BasicRoom.h>
#include <VipRoom.h>
#include <StudioRoom.h>
#include <ConsoleUI.h>
#include <RoomManager.h>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <stdexcept>
using namespace std;

namespace {
const string ROOM_DATA_FILE = "Room.txt";

string ToLower(string value)
{
    transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });
    return value;
}

bool ReadForm(const string& title, const vector<pair<string, string>>& fields,
    vector<string>& values)
{
    FormBox form(2, 1, 78, 0.5, title);
    for (const auto& field : fields)
        form.AddField(field.first, field.second);

    if (!form.Exec()) return false;

    values.clear();
    for (const auto& field : form.GetFields())
        values.push_back(field.value);
    return true;
}

bool ParseUnsigned(const string& value, ull& number)
{
    if (value.empty()) return false;
    for (unsigned char c : value)
        if (!isdigit(c)) return false;

    istringstream input(value);
    input >> number;
    return input && input.eof();
}

bool ParsePositiveInt(const string& value, int& number)
{
    ull parsed;
    if (!ParseUnsigned(value, parsed) || parsed == 0
        || parsed > static_cast<ull>(numeric_limits<int>::max()))
        return false;
    number = static_cast<int>(parsed);
    return true;
}

void Pause()
{
    cout << "\nNhan mot phim bat ky de tiep tuc...";
    char c;
    Console::Read(c);
    //system("pause");
}

ostream& operator<<(ostream& o, const vector<Furnishing>& furn)
{
    o << border::TL
        << string(F_ID_W, border::H) << border::DT
        << string(F_NAME_W, border::H) << border::DT
        << string(F_PRICE_W, border::H) << border::TR
        << endl;
    for (auto it : furn){
        o << it << endl;
    }
    o << border::BL
        << string(F_ID_W, border::H) << border::UT
        << string(F_NAME_W, border::H) << border::UT
        << string(F_PRICE_W, border::H) << border::BR
        << endl;
    return o;
}

ostream& operator<<(ostream& o, const vector<Damage>& damage)
{
    o << border::TL
        << string(DAM_DATE_W, border::H) << border::DT
        << string(DAM_FURN_W, border::H) << border::DT
        << string(DAM_DESCRIPTION_W, border::H) << border::DT
        << string(DAM_RESOLUTION_W, border::H) << border::TR
        << endl;
    for (int i = damage.size()-1; i>=0; i--){
        o << damage[i] << endl;
    }
    o << border::BL
        << string(DAM_DATE_W, border::H) << border::UT
        << string(DAM_FURN_W, border::H) << border::UT
        << string(DAM_DESCRIPTION_W, border::H) << border::UT
        << string(DAM_RESOLUTION_W, border::H) << border::BR
        << endl;
    return o;
}

void PrintRoomDetails(const Room& room)
{
    cout << "Ma phong   : " << room.GetId() << '\n'
        << "Ten phong  : " << room.GetName() << '\n'
        << "Loai phong : " << room.GetRoomType() << '\n'
        << "Tang       : " << room.GetFloor() << '\n'
        << "Suc chua   : " << room.GetTenantCount() << "/" << room.GetCapacity() << '\n'
        << "Trang thai : " << (room.IsFull() ? "Da day" : "Con cho") << '\n'
        << "Gia thue   : " << room.GetRentPrice() << " VND\n"
        << "Gia thue   : " << room.calcRent() << " VND\n"
        << "Tien coc   : " << room.GetPrepaid() << " VND\n"
        << "Chi so dien: " << room.GetOldElectricIndex() << " -> "
            << room.GetNewElectricIndex() << '\n'
        << "Chi so nuoc: " << room.GetOldWaterIndex() << " -> "
            << room.GetNewWaterIndex() << '\n'
        << "Tong no    : " << room.GetDebt() << " VND\n";

    cout << "\n--- Danh sach nguoi thue ---\n";
    if (room.GetTenants().empty()) cout << "Khong co\n";
    else {
        TenantTableInit();
        for (Tenant* tenant : room.GetTenants()){
            if (tenant)
                cout << tenant << '\n';
        }
        TenantTableEnd();
    }

    cout << "\n--- Tien nghi ---\n";
    if (room.GetFurnishings().empty()) cout << "Khong co\n";
    else cout << room.GetFurnishings();

    cout << "\n--- Bao hong ---\n";
    if (room.GetDamages().empty()) cout << "Khong co\n";
    else cout << room.GetDamages();
}

bool HasDuplicateRoomId(Nhatro& nhatro, const string& id, const Room* except = nullptr)
{
    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        if (room != except && room->GetId() == id) return true;
    }
    return false;
}

void DrawPanel(int x, int y, int width, int height, const string& title, bool focused)
{
    Console::SetColor(focused ? Console::CYAN : Console::WHITE);
    Console::Goto(x, y);
    cout << border::TL << string(width - 2, border::H) << border::TR;
    for (int row = 1; row < height - 1; row++){
        Console::Goto(x, y + row);
        cout << border::V;
        Console::Goto(x + width - 1, y + row);
        cout << border::V;
    }
    Console::Goto(x, y + height - 1);
    cout << border::BL << string(width - 2, border::H) << border::BR;

    Console::SetColor(Console::YELLOW);
    Console::Goto(x + 1, y + 1);
    cout << title.substr(0, width - 2);
    Console::SetColor(Console::WHITE);
}

void DrawRoomList(Nhatro& nhatro, int filter, const string& search)
{
    vector<Room*> rooms;
    string query = ToLower(search);
    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        if (filter == 1 && room->IsFull()) continue;
        if (filter >= 2 && static_cast<int>(room->Type()) != filter - 2) continue;

        if (!query.empty()
            && ToLower(room->GetId()).find(query) == string::npos
            && ToLower(room->GetName()).find(query) == string::npos)
            continue;
        rooms.push_back(room);
    }

    Console::SetColor(Console::WHITE);
    Console::Goto(0, 13);
    cout << "So phong: " << rooms.size() << "                                      ";
    Console::Goto(0, 14);
    RoomTableInit();
    for (size_t i = 0; i < rooms.size(); i++){
        Console::Goto(0, 16 + static_cast<int>(i));
        cout << *rooms[i];
    }
    Console::Goto(0, 16 + static_cast<int>(rooms.size()));
    RoomTableEnd();
}

void DrawManageRoomsScreen(Nhatro& nhatro, int focus, int filter,
    int selectedAction, const string& search)
{
    system("cls");
    Console::SetCursorVisible(false);
    Console::SetColor(Console::YELLOW);
    Console::Goto(0, 0);
    cout << center("QUAN LY PHONG", 80);

    DrawPanel(2, 2, 23, 10, "LOC PHONG", focus == 1);
    const vector<string> filters = {
        "Tat ca phong", "Con cho", "Phong Basic", "Phong Vip", "Phong Studio"
    };
    for (int i = 0; i < static_cast<int>(filters.size()); i++){
        Console::Goto(4, 4 + i);
        if (i == filter) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        cout << string(19, ' ');
        Console::Goto(4, 4 + i);
        cout << filters[i];
    }

    DrawPanel(26, 2, 52, 4, "TIM KIEM MA HOAC TEN", focus == 0);
    Console::Goto(28, 4);
    Console::SetColor(Console::WHITE);
    cout << "> " << search.substr(0, 46);

    const vector<string> actions = {
        "THEM [A]", "XOA [D]", "SUA [U]", "XEM [I]"
    };
    const vector<string> keyHints = {
        "Enter / A", "Enter / D", "Enter / U", "Enter / I"
    };
    for (int i = 0; i < static_cast<int>(actions.size()); i++){
        int x = 26 + i * 13;
        DrawPanel(x, 7, 12, 5, actions[i], focus == 2 && i == selectedAction);
        Console::Goto(x + 1, 9);
        Console::SetColor(focus == 2 && i == selectedAction
            ? Console::YELLOW : Console::WHITE);
        cout << keyHints[i];
    }

    Console::SetColor(Console::WHITE);
    Console::Goto(0, 12);
    cout << "TAB: chuyen muc | Tim: go chu, Backspace | Nut: trai/phai, Enter | ESC: thoat";
    DrawRoomList(nhatro, filter, search);
    Console::SetColor(Console::WHITE);
}
}

void LoadRoomData(const string& filename, Nhatro& nhatro)
{
    ifstream inp("data/" + filename);
    if (!inp) throw "Khong the tai file du lieu phong!";

    string line, id, type, name, cap, floor, rentPrice, prepaid, oldElec, newElec, oldWater, newWater;
    while (getline(inp, line)){
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        stringstream dt(line);
        getline(dt, id, '|');
        getline(dt, type, '|');
        getline(dt, name, '|');
        getline(dt, cap, '|');
        getline(dt, floor, '|');
        getline(dt, rentPrice, '|');
        getline(dt, prepaid, '|');
        getline(dt, oldElec, '|');
        getline(dt, newElec, '|');
        getline(dt, oldWater, '|');
        getline(dt, newWater);

        unique_ptr<Room> newRoom;
        RoomType rt = (RoomType) stoi(type);
        if (rt == RoomType::Basic){
            newRoom = make_unique<BasicRoom>(id, name, stoi(cap), stoi(floor));
        }
        if (rt == RoomType::Vip){
            newRoom = make_unique<VipRoom>(id, name, stoi(cap), stoi(floor));
        }
        if (rt == RoomType::Studio){
            newRoom = make_unique<StudioRoom>(id, name, stoi(cap), stoi(floor));
        }
        if (!newRoom) continue;
        newRoom->SetRentPrice(stoull(rentPrice));
        newRoom->SetPrepaid(stoull(prepaid));
        newRoom->SetElectricIndex(stoull(oldElec));
        newRoom->SetElectricIndex(stoull(newElec));
        newRoom->SetWaterIndex(stoull(oldWater));
        newRoom->SetWaterIndex(stoull(newWater));
        
        nhatro.AddRoom(move(newRoom));
    }
}

void SaveRoomData(const string& filename, Nhatro& nhatro)
{
    ofstream oup("data/" + filename);
    if (!oup) throw runtime_error("Khong the luu file du lieu phong!");
    
    for (int i=0; i<nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        oup << room->GetId()        << "|"
            << (int) room->Type()   << "|"
            << room->GetName()      << "|"
            << room->GetCapacity()  << "|"
            << room->GetFloor()     << "|"
            << room->GetRentPrice() << "|"
            << room->GetPrepaid()   << "|"
            << room->GetOldElectricIndex() << "|"
            << room->GetNewElectricIndex() << "|"
            << room->GetOldWaterIndex() << "|"
            << room->GetNewWaterIndex() << "\n";
    }
    if (!oup) throw runtime_error("Loi khi ghi file du lieu phong!");
}

void ManageRooms(Nhatro& nhatro)
{
    int focus = 2;
    int filter = 0;
    int selectedAction = 0;
    string search;
    auto executeAction = [&]() {
        switch (selectedAction){
            case 0: AddRoom(nhatro); break;
            case 1: DeleteRoom(nhatro); break;
            case 2: UpdateRoom(nhatro); break;
            case 3: ShowRoomDetails(nhatro); break;
        }
        focus = 2;
    };

    while (true){
        DrawManageRoomsScreen(nhatro, focus, filter, selectedAction, search);

        char pressed;
        Console::Key key = Console::Read(pressed);
        if (key == Console::ESC) break;

        if (key == Console::TAB){
            focus = (focus + 1) % 3;
        }
        else if (focus == 0){
            if (key == Console::BACKSPACE){
                if (!search.empty()) search.pop_back();
            }
            else if (pressed != '\0' && search.size() < 64){
                search += pressed;
            }
            else if (key == Console::ENTER){
                focus = 1;
            }
        }
        else if (focus == 1){
            if (key == Console::UP || key == Console::LEFT)
                filter = (filter + 4) % 5;
            else if (key == Console::DOWN || key == Console::RIGHT)
                filter = (filter + 1) % 5;
            else if (key == Console::ENTER)
                focus = 2;
        }
        else if (focus == 2){
            if (key == Console::LEFT)
                selectedAction = (selectedAction + 3) % 4;
            else if (key == Console::RIGHT)
                selectedAction = (selectedAction + 1) % 4;
            else if (key == Console::ENTER){
                executeAction();
            }
            else if (pressed != '\0'){
                switch (tolower(static_cast<unsigned char>(pressed))){
                    case 'a': selectedAction = 0; executeAction(); break;
                    case 'd': selectedAction = 1; executeAction(); break;
                    case 'u': selectedAction = 2; executeAction(); break;
                    case 'i': selectedAction = 3; executeAction(); break;
                }
            }
        }
    }
    Console::SetColor(Console::WHITE);
}

void FindRooms(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("TIM PHONG", {{"Nhap ma hoac ten", ""}}, values)) return;
    string key = ToLower(values[0]);
    bool found = false;

    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        if (ToLower(room->GetId()) == key || ToLower(room->GetName()) == key){
            PrintRoomDetails(*room);
            found = true;
        }
    }
    if (!found) cout << "Khong tim thay phong phu hop.\n";
    Pause();
}

void ShowRoomDetails(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("CHI TIET PHONG", {{"Ma phong", ""}}, values)) return;

    Room* room = nhatro.FindRoom(values[0]);
    if (!room) cout << "Khong tim thay phong co ma " << values[0] << ".\n";
    else PrintRoomDetails(*room);
    Pause();
}

void ShowAvailableRooms(Nhatro& nhatro)
{
    nhatro.ShowAvailableRooms();
    Pause();
}

void ShowRoomsByType(Nhatro& nhatro)
{
    MenuBox menu(2, 1, 30, "CHON LOAI PHONG");
    menu.AddOption("Basic");
    menu.AddOption("Vip");
    menu.AddOption("Studio");

    int choice = menu.Exec();
    if (choice == -1) return;
    nhatro.ShowRoomsByType(static_cast<RoomType>(choice));
    Pause();
}

void AddRoom(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("THEM PHONG", {
        {"Ma phong", ""},
        {"Ten phong", ""},
        {"Loai (0 Basic, 1 Vip, 2 Studio)", "0"},
        {"Suc chua", "2"},
        {"Tang", "1"},
        {"Gia thue", "0"},
        {"Tien coc", "0"},
        {"Chi so dien hien tai", "0"},
        {"Chi so nuoc hien tai", "0"}
    }, values)) return;

    ull roomType, rentPrice, prepaid, electricIndex, waterIndex;
    int capacity, floor;
    if (values[0].empty() || values[1].empty()
        || !ParseUnsigned(values[2], roomType) || roomType > Studio
        || !ParsePositiveInt(values[3], capacity)
        || !ParsePositiveInt(values[4], floor)
        || !ParseUnsigned(values[5], rentPrice) || rentPrice == 0
        || !ParseUnsigned(values[6], prepaid)
        || !ParseUnsigned(values[7], electricIndex)
        || !ParseUnsigned(values[8], waterIndex)){
        cout << "Du lieu khong hop le. Phong chua duoc them.\n";
        Pause();
        return;
    }
    if (HasDuplicateRoomId(nhatro, values[0])){
        cout << "Ma phong da ton tai.\n";
        Pause();
        return;
    }

    unique_ptr<Room> room;
    if (roomType == Basic)
        room = make_unique<BasicRoom>(values[0], values[1], capacity, floor);
    else if (roomType == Vip)
        room = make_unique<VipRoom>(values[0], values[1], capacity, floor);
    else
        room = make_unique<StudioRoom>(values[0], values[1], capacity, floor);

    room->SetRentPrice(rentPrice);
    room->SetPrepaid(prepaid);
    room->SetElectricIndex(electricIndex);
    room->SetWaterIndex(waterIndex);
    if (!nhatro.AddRoom(move(room))){
        cout << "Khong the them phong.\n";
        Pause();
        return;
    }

    SaveRoomData(ROOM_DATA_FILE, nhatro);
    cout << "Da them phong thanh cong.\n";
    Pause();
}

void DeleteRoom(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("XOA PHONG", {{"Ma phong", ""}}, values)) return;

    Room* room = nhatro.FindRoom(values[0]);
    if (!room){
        cout << "Khong tim thay phong.\n";
        Pause();
        return;
    }
    if (!nhatro.RemoveRoom(room)){
        cout << "Phong dang co nguoi thue, khong the xoa.\n";
        Pause();
        return;
    }

    SaveRoomData(ROOM_DATA_FILE, nhatro);
    cout << "Da xoa phong thanh cong.\n";
    Pause();
}

void UpdateRoom(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("CAP NHAT PHONG", {{"Ma phong can sua", ""}}, values)) return;
    Room* room = nhatro.FindRoom(values[0]);
    if (!room){
        cout << "Khong tim thay phong.\n";
        Pause();
        return;
    }

    if (!ReadForm("THONG TIN PHONG MOI", {
        {"Ma phong", room->GetId()},
        {"Ten phong", room->GetName()},
        {"Suc chua", to_string(room->GetCapacity())},
        {"Tang", to_string(room->GetFloor())},
        {"Gia thue", to_string(room->GetRentPrice())},
        {"Tien coc", to_string(room->GetPrepaid())},
        {"Chi so dien hien tai", to_string(room->GetNewElectricIndex())},
        {"Chi so nuoc hien tai", to_string(room->GetNewWaterIndex())}
    }, values)) return;

    int capacity, floor;
    ull rentPrice, prepaid, electricIndex, waterIndex;
    if (values[0].empty() || values[1].empty()
        || !ParsePositiveInt(values[2], capacity)
        || capacity < room->GetTenantCount()
        || !ParsePositiveInt(values[3], floor)
        || !ParseUnsigned(values[4], rentPrice) || rentPrice == 0
        || !ParseUnsigned(values[5], prepaid)
        || !ParseUnsigned(values[6], electricIndex)
        || electricIndex < room->GetNewElectricIndex()
        || !ParseUnsigned(values[7], waterIndex)
        || waterIndex < room->GetNewWaterIndex()
        || HasDuplicateRoomId(nhatro, values[0], room)){
        cout << "Du lieu khong hop le. Thong tin phong chua thay doi.\n";
        Pause();
        return;
    }

    room->SetId(values[0]);
    room->SetName(values[1]);
    room->SetCapacity(capacity);
    room->SetFloor(floor);
    room->SetRentPrice(rentPrice);
    room->SetPrepaid(prepaid);
    room->SetElectricIndex(electricIndex);
    room->SetWaterIndex(waterIndex);

    SaveRoomData(ROOM_DATA_FILE, nhatro);
    cout << "Da cap nhat phong thanh cong.\n";
    Pause();
}