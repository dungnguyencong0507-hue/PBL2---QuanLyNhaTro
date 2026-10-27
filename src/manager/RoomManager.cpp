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
    FormBox form(2, 1, 78, title);
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
}

void PrintRoomDetails(const Room& room)
{
    cout << "\n========== CHI TIET PHONG ==========\n"
        << "Ma phong: " << room.GetId() << '\n'
        << "Ten phong: " << room.GetName() << '\n'
        << "Loai phong: " << room.GetRoomType() << '\n'
        << "Tang: " << room.GetFloor() << '\n'
        << "Suc chua: " << room.GetTenantCount() << "/" << room.GetCapacity() << '\n'
        << "Trang thai: " << (room.IsFull() ? "Da day" : "Con cho") << '\n'
        << "Gia thue: " << room.GetRentPrice() << " VND\n"
        << "Tien thue hien tai: " << room.calcRent() << " VND\n"
        << "Tien coc: " << room.GetPrepaid() << " VND\n"
        << "Chi so dien: " << room.GetOldElectricIndex() << " -> "
        << room.GetNewElectricIndex() << '\n'
        << "Chi so nuoc: " << room.GetOldWaterIndex() << " -> "
        << room.GetNewWaterIndex() << '\n'
        << "Cong no: " << room.GetDebt() << " VND\n";

    Tenant* leader = room.GetRoomLeader();
    cout << "Nguoi dai dien: ";
    if (leader) cout << leader->GetName() << " (" << leader->GetId() << ")\n";
    else cout << "Chua co\n";

    cout << "\n--- Danh sach nguoi thue ---\n";
    if (room.GetTenants().empty()) cout << "Khong co\n";
    for (Tenant* tenant : room.GetTenants()){
        if (tenant)
            cout << "- " << tenant->GetName() << " | Ma: " << tenant->GetId()
                 << " | SDT: " << tenant->GetPhone() << '\n';
    }

    cout << "\n--- Tien nghi ---\n";
    if (room.GetFurnishings().empty()) cout << "Khong co\n";
    for (const Furnishing& furnishing : room.GetFurnishings())
        cout << "- " << furnishing.name << " | Ma: " << furnishing.id
             << " | Gia tri: " << furnishing.price << " VND\n";

    cout << "\n--- Hoa don hang thang ---\n";
    if (room.GetMonthlyBills().empty()) cout << "Khong co\n";
    for (const MonthlyBill& bill : room.GetMonthlyBills()){
        cout << "- " << bill.month.month << "/" << bill.month.year
             << " | Thue: " << bill.rentCost
             << " | Dich vu: " << bill.serviceCost
             << " | Dien: " << bill.electricCost
             << " | Nuoc: " << bill.waterCost
             << " | Hu hong: " << bill.damageCost
             << " | Tong: " << bill.totalCost
             << " | " << (bill.isPaid ? "Da thanh toan" : "Chua thanh toan");
        if (bill.isPaid)
            cout << " ngay " << bill.paymentDate.day << "/"
                 << bill.paymentDate.month << "/" << bill.paymentDate.year;
        cout << '\n';
    }

    cout << "\n--- Bao hong ---\n";
    if (room.GetDamages().empty()) cout << "Khong co\n";
    for (const Damage& damage : room.GetDamages()){
        cout << "- " << damage.date.day << "/" << damage.date.month << "/"
             << damage.date.year << " | "
             << (damage.furnishing ? damage.furnishing->name : "Khong ro vat dung")
             << " | " << damage.description
             << " | Xu ly: " << damage.resolution
             << " | " << (damage.isResolved ? "Da xu ly" : "Chua xu ly") << '\n';
    }
    cout << "====================================\n";
}

bool HasDuplicateRoomId(Nhatro& nhatro, const string& id, const Room* except = nullptr)
{
    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        if (room != except && room->GetId() == id) return true;
    }
    return false;
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
    MenuBox menu(2, 1, 38, "QUAN LY PHONG");
    menu.AddOption("Hien thi danh sach phong");
    menu.AddOption("Tim phong theo ma / ten");
    menu.AddOption("Xem chi tiet phong");
    menu.AddOption("Hien thi phong con cho");
    menu.AddOption("Loc phong theo loai");
    menu.AddOption("Them phong");
    menu.AddOption("Xoa phong");
    menu.AddOption("Cap nhat phong");

    int choice;
    while ((choice = menu.Exec()) != -1){
        switch (choice){
            case 0:
                nhatro.ShowRooms();
                Pause();
                break;
            case 1:
                FindRooms(nhatro);
                break;
            case 2:
                ShowRoomDetails(nhatro);
                break;
            case 3:
                ShowAvailableRooms(nhatro);
                break;
            case 4:
                ShowRoomsByType(nhatro);
                break;
            case 5:
                AddRoom(nhatro);
                break;
            case 6:
                DeleteRoom(nhatro);
                break;
            case 7:
                UpdateRoom(nhatro);
                break;
        }
    }
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