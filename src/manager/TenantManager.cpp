#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <header.h>
#include <Nhatro.h>
#include <Tenant.h>
#include <Room.h>
#include <TenantManager.h>
#include <ConsoleUI.h>
using namespace std;

namespace {
const string TENANT_DATA_FILE = "Tenant.txt";

string ToLower(string value)
{
    transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });
    return value;
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

bool HasDuplicateTenantId(Nhatro& nhatro, const string& id, const Tenant* except = nullptr)
{
    for (int i = 0; i < nhatro.GetNumTenant(); i++){
        Tenant* tenant = nhatro.GetTenant(i);
        if (tenant != except && tenant->GetId() == id) return true;
    }
    return false;
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

void Pause()
{
    cout << "\nNhan mot phim bat ky de tiep tuc...";
    char c;
    Console::Read(c);
}

void ShowTenantDetails(const Tenant& tenant)
{
    cout << "\n========== CHI TIET KHACH THUE ==========\n"
        << "Ma khach: " << tenant.GetId() << '\n'
        << "Ho ten: " << tenant.GetName() << '\n'
        << "Gioi tinh: " << (tenant.GetGender() == Male ? "Nam" : "Nu") << '\n'
        << "So dien thoai: " << tenant.GetPhone() << '\n'
        << "CCCD: " << tenant.GetCCCD() << '\n'
        << "Que quan: " << tenant.GetHometown() << '\n'
        << "Tam tru: " << (tenant.GetTemporaryResidence() ? "Da dang ky" : "Chua dang ky") << '\n';

    Room* room = tenant.GetRoom();
    if (room){
        cout << "Phong dang thue: " << room->GetName()
            << " (Ma: " << room->GetId() << ")\n"
            << "Loai phong: " << room->GetRoomType() << '\n'
            << "Tang: " << room->GetFloor() << '\n'
            << "Nguoi dai dien: "
            << (room->GetRoomLeader() == &tenant ? "Co" : "Khong") << '\n';
    }
    else cout << "Phong dang thue: Chua co\n";
    cout << "=========================================\n";
}
}

void LoadTenantData(const string& filename, Nhatro& nhatro)
{
    ifstream inp("data/" + filename);
    if (!inp) throw runtime_error("Khong the tai file du lieu khach hang!");

    string line, id, name, gender, cccd, sdt, home, roomID, isLeader, isTmpRsd;
    int lineNumber = 0;
    while (getline(inp, line)){
        lineNumber++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        stringstream dt(line);
        getline(dt, id, '|');
        getline(dt, name, '|');
        getline(dt, gender, '|');
        getline(dt, cccd, '|');
        getline(dt, sdt, '|');
        getline(dt, home, '|');
        getline(dt, roomID, '|');
        getline(dt, isLeader, '|');
        getline(dt, isTmpRsd);

        ull genderValue, temporaryResidence;
        if (id.empty() || name.empty()
            || !ParseUnsigned(gender, genderValue) || genderValue > Female
            || !ParseUnsigned(isTmpRsd, temporaryResidence) || temporaryResidence > 1)
            throw runtime_error("Du lieu khach thue khong hop le tai dong "
                + to_string(lineNumber) + ".");

        if (HasDuplicateTenantId(nhatro, id))
            throw runtime_error("Ma khach thue bi trung tai dong "
                + to_string(lineNumber) + ".");

        unique_ptr<Tenant> newTenant = make_unique<Tenant>(
            id, name, sdt, static_cast<Gender>(genderValue), cccd, home);
        newTenant->SetTemporaryResidence(temporaryResidence == 1);

        if (!roomID.empty()){
            Room* room = nhatro.FindRoom(roomID);
            if (!room)
                throw runtime_error("Khong tim thay phong " + roomID
                    + " tai dong " + to_string(lineNumber) + ".");
            if (!room->AddTenant(newTenant.get()))
                throw runtime_error("Khong the them khach vao phong " + roomID
                    + " tai dong " + to_string(lineNumber) + ".");
            newTenant->SetRoom(room);
            if (isLeader == "0") room->SetRoomLeader(newTenant.get());
        }

        if (!nhatro.AddTenant(move(newTenant)))
            throw runtime_error("Khong the them khach thue tai dong "
                + to_string(lineNumber) + ".");
    }
}

void SaveTenantData(const string& filename, Nhatro& nhatro)
{
    ofstream oup("data/" + filename);
    if (!oup) throw runtime_error("Khong the luu file du lieu khach hang!");

    for (int i = 0; i < nhatro.GetNumTenant(); i++){
        Tenant* tenant = nhatro.GetTenant(i);
        oup << tenant->GetId() << "|"
            << tenant->GetName() << "|"
            << tenant->GetGender() << "|"
            << tenant->GetCCCD() << "|"
            << tenant->GetPhone() << "|"
            << tenant->GetHometown() << "|"
            << (tenant->IsRenting() ? tenant->GetRoom()->GetId() : "") << "|"
            << ((tenant->IsRenting()
                && tenant->GetRoom()->GetRoomLeader() == tenant) ? 0 : 1) << "|"
            << tenant->GetTemporaryResidence() << "\n";
    }
    if (!oup) throw runtime_error("Loi khi ghi file du lieu khach hang!");
}

void ManageTenants(Nhatro& nhatro)
{
    MenuBox menu(2, 1, 38, "QUAN LY KHACH THUE");
    menu.AddOption("Hien thi danh sach khach thue");
    menu.AddOption("Tim khach theo ma / ten");
    menu.AddOption("Them khach thue");
    menu.AddOption("Cap nhat khach thue");

    int choice;
    while ((choice = menu.Exec()) != -1){
        switch (choice){
            case 0:
                ShowTenants(nhatro);
                break;
            case 1:
                FindTenant(nhatro);
                break;
            case 2:
                AddTenant(nhatro);
                break;
            case 3:
                UpdateTenant(nhatro);
                break;
        }
    }
}

void ShowTenants(Nhatro& nhatro)
{
    nhatro.ShowTenants();
    Pause();
}

void FindTenant(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("TIM KHACH THUE", {{"Ma khach hoac ho ten", ""}}, values)) return;

    string key = ToLower(values[0]);
    bool found = false;
    for (int i = 0; i < nhatro.GetNumTenant(); i++){
        Tenant* tenant = nhatro.GetTenant(i);
        string id = ToLower(tenant->GetId());
        string name = ToLower(tenant->GetName());
        if (id == key || name.find(key) != string::npos){
            ShowTenantDetails(*tenant);
            found = true;
        }
    }
    if (!found) cout << "Khong tim thay khach thue phu hop.\n";
    Pause();
}

void AddTenant(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("THEM KHACH THUE", {
        {"Ma khach", ""},
        {"Ho ten", ""},
        {"Gioi tinh (0 Nam, 1 Nu)", "0"},
        {"So dien thoai", ""},
        {"CCCD", ""},
        {"Que quan", ""},
        {"Ma phong (bo trong neu chua thue)", ""},
        {"Tam tru (0 Chua, 1 Da dang ky)", "0"}
    }, values)) return;

    ull genderValue, temporaryResidence;
    if (values[0].empty() || values[1].empty()
        || !ParseUnsigned(values[2], genderValue) || genderValue > Female
        || values[3].empty() || values[4].empty() || values[5].empty()
        || !ParseUnsigned(values[7], temporaryResidence) || temporaryResidence > 1){
        cout << "Du lieu khong hop le. Khach thue chua duoc them.\n";
        Pause();
        return;
    }
    if (HasDuplicateTenantId(nhatro, values[0])){
        cout << "Ma khach thue da ton tai.\n";
        Pause();
        return;
    }

    Room* room = nullptr;
    if (!values[6].empty()){
        room = nhatro.FindRoom(values[6]);
        if (!room){
            cout << "Khong tim thay phong co ma " << values[6] << ".\n";
            Pause();
            return;
        }
        if (room->IsFull()){
            cout << "Phong da day, khong the them khach thue.\n";
            Pause();
            return;
        }
    }

    unique_ptr<Tenant> tenant = make_unique<Tenant>(
        values[0], values[1], values[3], static_cast<Gender>(genderValue),
        values[4], values[5]);
    tenant->SetTemporaryResidence(temporaryResidence == 1);
    if (room){
        if (!room->AddTenant(tenant.get())){
            cout << "Khong the them khach vao phong.\n";
            Pause();
            return;
        }
        tenant->SetRoom(room);
    }
    Tenant* newTenant = tenant.get();
    if (!nhatro.AddTenant(move(tenant))){
        if (room) room->RemoveTenant(newTenant);
        cout << "Khong the them khach thue.\n";
        Pause();
        return;
    }

    SaveTenantData(TENANT_DATA_FILE, nhatro);
    cout << "Da them khach thue thanh cong.\n";
    Pause();
}

void UpdateTenant(Nhatro& nhatro)
{
    vector<string> values;
    if (!ReadForm("CAP NHAT KHACH THUE", {{"Ma khach can sua", ""}}, values)) return;

    Tenant* tenant = nhatro.FindTenant(values[0]);
    if (!tenant){
        cout << "Khong tim thay khach thue.\n";
        Pause();
        return;
    }

    Room* currentRoom = tenant->GetRoom();
    if (!ReadForm("THONG TIN KHACH THUE MOI", {
        {"Ma khach", tenant->GetId()},
        {"Ho ten", tenant->GetName()},
        {"Gioi tinh (0 Nam, 1 Nu)", to_string(tenant->GetGender())},
        {"So dien thoai", tenant->GetPhone()},
        {"CCCD", tenant->GetCCCD()},
        {"Que quan", tenant->GetHometown()},
        {"Ma phong (bo trong neu khong thue)", currentRoom ? currentRoom->GetId() : ""},
        {"Tam tru (0 Chua, 1 Da dang ky)", tenant->GetTemporaryResidence() ? "1" : "0"}
    }, values)) return;

    ull genderValue, temporaryResidence;
    if (values[0].empty() || values[1].empty()
        || !ParseUnsigned(values[2], genderValue) || genderValue > Female
        || values[3].empty() || values[4].empty() || values[5].empty()
        || !ParseUnsigned(values[7], temporaryResidence) || temporaryResidence > 1
        || HasDuplicateTenantId(nhatro, values[0], tenant)){
        cout << "Du lieu khong hop le. Thong tin chua thay doi.\n";
        Pause();
        return;
    }

    Room* newRoom = nullptr;
    if (!values[6].empty()){
        newRoom = nhatro.FindRoom(values[6]);
        if (!newRoom){
            cout << "Khong tim thay phong co ma " << values[6] << ".\n";
            Pause();
            return;
        }
        if (newRoom != currentRoom && newRoom->IsFull()){
            cout << "Phong moi da day, khong the chuyen khach.\n";
            Pause();
            return;
        }
    }

    if (newRoom != currentRoom){
        if (newRoom && !newRoom->AddTenant(tenant)){
            cout << "Khong the them khach vao phong moi.\n";
            Pause();
            return;
        }
        if (currentRoom) currentRoom->RemoveTenant(tenant);
        tenant->SetRoom(newRoom);
    }

    tenant->SetName(values[1]);
    tenant->SetGender(static_cast<Gender>(genderValue));
    tenant->SetPhone(values[3]);
    tenant->SetCCCD(values[4]);
    tenant->SetHometown(values[5]);
    tenant->SetTemporaryResidence(temporaryResidence == 1);
    tenant->SetRoom(newRoom);
    tenant->SetId(values[0]);

    SaveTenantData(TENANT_DATA_FILE, nhatro);
    cout << "Da cap nhat khach thue thanh cong.\n";
    Pause();
}
