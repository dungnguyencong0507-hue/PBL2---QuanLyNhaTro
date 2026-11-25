#include <RoomManager.h>
using namespace std;

void ManageRooms(Nhatro& nhatro)
{
    auto& rooms = nhatro.GetRooms();
    Manager<Room> roomManager("PHONG", nhatro, rooms, RoomTableWidth(), RoomTableInit, RoomTableEnd, RoomTableShow,
                            SearchRooms, AddRoom, UpdateRoom, RemoveRoom);
    roomManager.AddFilter("Phong con suat", FilterAvailableRooms);
    roomManager.AddFilter("Phong da co nguoi thue", FilterRentedRooms);
    roomManager.AddFilter("Phong day", FilterFullRooms);
    roomManager.AddFilter("Phong co ban", FilterBasicRooms);
    roomManager.AddFilter("Phong VIP", FilterVipRooms);
    roomManager.AddFilter("Phong Studio", FilterStudioRooms);
    roomManager.AddSortComparison("Gia thue tang dan", CompareRoomsPrice);
    roomManager.AddSortComparison("Gia thue giam dan", CompareRoomsPrice1);
    roomManager.AddSortComparison("Suc chua tang dan", CompareRoomsCapacity);
    roomManager.AddSortComparison("Suc chua giam dan", CompareRoomsCapacity1);
    roomManager.AddSortComparison("Suat con lai tang dan", CompareRoomsAvailability);
    roomManager.AddSortComparison("Suat con lai giam dan", CompareRoomsAvailability1);
    
    roomManager.Draw();
}

bool SearchRooms(Room* room, const string& query) // Theo ten phong
{
    if (room == nullptr) return false;
    if (query == room->GetId()) return true;
    string roomName = room->GetName();
    transform(roomName.begin(), roomName.end(), roomName.begin(), ::tolower);
    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    return roomName.find(lowerQuery) != string::npos;
}
bool FilterAvailableRooms(Room* room)
{
    return (!room->IsRented() || !room->IsFull());
}
bool FilterRentedRooms(Room* room)
{
    return room->IsRented();
}
bool FilterFullRooms(Room* room)
{
    return room->IsFull();
}
bool FilterBasicRooms(Room* room)
{
    return room->Type() == RoomType::Basic;
}
bool FilterVipRooms(Room* room)
{
    return room->Type() == RoomType::Vip;
}
bool FilterStudioRooms(Room* room)
{
    return room->Type() == RoomType::Studio;
}
bool CompareRoomsPrice(Room* room1, Room* room2)
{
    return room1->GetRentPrice() < room2->GetRentPrice();
}
bool CompareRoomsPrice1(Room* room1, Room* room2)
{
    return room1->GetRentPrice() > room2->GetRentPrice();
}
bool CompareRoomsCapacity(Room* room1, Room* room2)
{
    return room1->GetCapacity() < room2->GetCapacity();
}
bool CompareRoomsCapacity1(Room* room1, Room* room2)
{
    return room1->GetCapacity() > room2->GetCapacity();
}
bool CompareRoomsAvailability(Room* room1, Room* room2)
{
    return (room1->GetCapacity() - room1->GetTenantCount()) < (room2->GetCapacity() - room2->GetTenantCount());
}
bool CompareRoomsAvailability1(Room* room1, Room* room2)
{
    return (room1->GetCapacity() - room1->GetTenantCount()) > (room2->GetCapacity() - room2->GetTenantCount());
}

FormBox GetRoomDetails(Room* room, int x, int y, bool draw)
{
    if (room == nullptr) throw runtime_error("Khong ton tai du lieu phong");

    FormBox roomDetailsBox(x, y, 70, 0.5, "THONG TIN PHONG");
    string roomType = (room->Type() == RoomType::Basic ? "[CO BAN]" : (room->Type() == RoomType::Vip ? "[VIP]" : "[STUDIO]"));
    roomDetailsBox.AddConstField("Loai phong", roomType);
    roomDetailsBox.AddField("Ma phong", room->GetId());
    roomDetailsBox.AddField("Ten phong", room->GetName());
    roomDetailsBox.AddField("So nguoi toi da", to_string(room->GetCapacity()), to_string(room->GetTenantCount()) + "/");
    roomDetailsBox.AddField("Tang", to_string(room->GetFloor()));
    roomDetailsBox.AddField("Gia thue", to_string(room->GetRentPrice()), "", " VND");
    roomDetailsBox.AddField("Chi so dien", to_string(room->GetNewElectricIndex()), to_string(room->GetOldElectricIndex()) + " -> ", " kWh");
    roomDetailsBox.AddField("Chi so nuoc", to_string(room->GetNewWaterIndex()), to_string(room->GetOldWaterIndex()) + " -> ", " m3");
    roomDetailsBox.AddField("Tien coc", to_string(room->GetPrepaid()), "", " VND");
    roomDetailsBox.AddConstField("Trang thai", (room->IsRented() ? "Da duoc thue tu ngay " + to_string(room->GetDate()) : "Con trong"));
    
    if (draw) roomDetailsBox.Draw();
    return roomDetailsBox;
}

bool GetRoomForm(int x, int y, Nhatro& nhatro, unique_ptr<Room>& newRoom, Room* room = nullptr)
{
    FormBox roomForm(x, y, 80, 0.5, "THEM PHONG MOI");
    if (room) {
        roomForm = GetRoomDetails(room, x, y);
    }
    else {
        roomForm.AddChosenField("Loai phong *", {"CO BAN", "VIP", "STUDIO"});
        roomForm.AddField("Ma phong *");
        roomForm.AddField("Ten phong *");
        roomForm.AddField("So nguoi toi da *", "", "", " nguoi");
        roomForm.AddField("Tang *");
        roomForm.AddField("Gia thue (Bo qua neu lay gia chuan)", "", "", " VND");
        roomForm.AddField("Chi so dien *", "0", "", " kWh");
        roomForm.AddField("Chi so nuoc *", "0", "", " m3");
        roomForm.GoToCenter(x, y);
    }

    string roomId, roomName, capacityStr, floorStr, rentPriceStr, electricIndexStr, waterIndexStr, prepaidStr;
    int capacity = 0, floor = 0;
    ull rentPrice = 0, electricIndex = 0, waterIndex = 0, prepaid;
    Console::Key key;
    bool confirm;
    string error;

    roomForm.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, x, roomForm.GetY() + roomForm.GetH());
        error.clear();
    }
    try {
        confirm = roomForm(key);
        if (key == Console::ESC) return false;

        if (confirm) {
            roomId = roomForm.GetValue(1);
            roomName = roomForm.GetValue(2);
            capacityStr = roomForm.GetValue(3);
            floorStr = roomForm.GetValue(4);
            rentPriceStr = roomForm.GetValue(5);
            electricIndexStr = roomForm.GetValue(6);
            waterIndexStr = roomForm.GetValue(7);
            if (room) prepaidStr = roomForm.GetValue(8);

            CheckId(roomId, 8);

            CheckRoomName(roomName);

            if (room == nullptr && nhatro.FindRoom(roomId) != -1)
                throw invalid_argument("Ma phong da ton tai!");
            
            capacity = SToI(capacityStr);

            floor = SToI(floorStr);
            if (capacity <= 0) throw invalid_argument("Suc chua phai lon hon 0");
            if (room && capacity < room->GetTenantCount())
                throw invalid_argument("Suc chua khong du so nguoi dang thue!");
            if (floor <= 0) throw invalid_argument("Tang phai lon hon 0");
            
            if (room == nullptr && rentPriceStr.empty()) rentPriceStr = "0";
            rentPrice = SToULL(rentPriceStr);

            electricIndex = SToULL(electricIndexStr);

            waterIndex = SToULL(waterIndexStr);

            if (room) prepaid = SToULL(prepaidStr);

            if (room == nullptr) {
                RoomType roomType = static_cast<RoomType>(roomForm.GetChosenValue(0));
                if (roomType == RoomType::Basic) {
                    newRoom = make_unique<BasicRoom>(roomId, roomName, capacity, floor);
                } else if (roomType == RoomType::Vip) {
                    newRoom = make_unique<VipRoom>(roomId, roomName, capacity, floor);
                } else if (roomType == RoomType::Studio) {
                    newRoom = make_unique<StudioRoom>(roomId, roomName, capacity, floor);
                }

                newRoom->SetRentPrice(rentPrice ? rentPrice : newRoom->GetStdPrice());
                newRoom->SetElectricIndex(electricIndex);
                newRoom->SetWaterIndex(waterIndex);
            }
            else {
                room->SetId(roomId);
                room->SetName(roomName);
                room->SetCapacity(capacity);
                room->SetFloor(floor);
                room->SetRentPrice(rentPrice);
                if (electricIndex != room->GetNewElectricIndex()) room->EditElectricIndex(electricIndex);
                if (waterIndex != room->GetNewWaterIndex()) room->EditWaterIndex(waterIndex);
                room->SetPrepaid(prepaid);
            }

            return true;
        }   
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    catch (const string& e) {
        error = e;
    }
    catch (...) {
        error = "Loi khong xac dinh";
    }
    }
}

void AddRoom(Nhatro& nhatro)
{
    system("cls");
    unique_ptr<Room> newRoom;
    bool success = GetRoomForm(3, 3, nhatro, newRoom);

    if (success) nhatro.AddRoom(move(newRoom));
}

void RemoveRoom(Nhatro& nhatro, Room* room)
{
    nhatro.RemoveRoom(room);
}

void AddTenantToRoom(Nhatro& nhatro, Room* room)
{
    if (room == nullptr) throw runtime_error("Khong ton tai du lieu phong");
    int padding = 3;

    string query = "";
    SingleLineBox searchTenantBox(padding, padding, 50, "TIM KIEM", query);

    List<Tenant*> freeTenants, results;
    auto& tenants = nhatro.GetTenants();
    for (const auto& per : tenants) {
        if (!per.get()->IsRenting()) freeTenants.push_back(per.get());
    }
    results = freeTenants;
    TableBox<Tenant*> freeTenantsBox(searchTenantBox.GetX(), searchTenantBox.GetY() + searchTenantBox.GetH(), TenantTableWidth(), results, TenantTableInit, TenantTableEnd, TenantTableShow);

    bool search = true;
    bool redraw = true;
    string error;
    Console::Key key;
    while (true) {
    try {
        if (redraw) {
            system("cls");
            Console::SetColor(Console::YELLOW);
            Console::Goto(padding, 1);
            cout << "CHECK IN CHO KHACH VAO PHONG " << room->GetName();
            searchTenantBox.Draw();
            freeTenantsBox.Draw();
            redraw = false;
        }
        if (!error.empty()) {
            Console::DrawManagerError(error, searchTenantBox.GetX(), searchTenantBox.GetY() - 1);
            error.clear();
        }

        if (search) {
            searchTenantBox(key);
            if (key == Console::DOWN) search = false;

            if (key == Console::ENTER) {
                results.clear();
                int id;
                if ((id = nhatro.FindTenant(query)) != -1) {
                    if (!tenants[id]->IsRenting()) results.push_back(tenants[id].get());
                }
                else {
                    for (const auto& per : freeTenants) {
                        if (SearchTenant(per, query)) results.push_back(per);
                    }
                }

                freeTenantsBox.Draw();
            }
        }
        else {
            int choice = freeTenantsBox(key, true);

            if (choice != -1) {
                Gender currGend;
                bool singleGender = room->IsSingleGender(currGend);
                if (singleGender && currGend != results[choice]->GetGender()) {
                    bool confirm = WarningBox::Warning(30, "CANH BAO", 
                        "Co xac nhan them nguoi khac gioi vao phong khong?");
                    redraw = true;
                    if (!confirm) continue;
                }
                if (!room->IsRented()) room->SetDate(Nhatro::today);
                room->AddTenant(results[choice]);
                break;
            }
            else search = true;
            key = Console::OTHER;
        }

        if (key == Console::ESC) return;
        if (key == Console::F5) redraw = true;
        continue;
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void AddFurnToRoom(Room* room)
{
    if (room == nullptr) throw runtime_error("Khong ton tai du lieu phong");

    system("cls");
    int padding = 3;
    FormBox addFurnBox(padding, padding, 60, 0.75, "THEM DO DUNG MOI");
    addFurnBox.AddField("Ten do dung *");
    addFurnBox.AddField("Gia tien *", "0", "", " VND");

    string furnName, furnCostStr;
    ull furnCost;
    Console::Key key;
    bool confirm;
    string error;

    addFurnBox.GoToCenter();
    addFurnBox.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, padding, addFurnBox.GetY() + addFurnBox.GetH());
        error.clear();
    }
    try {
        confirm = addFurnBox(key);
        if (key == Console::ESC) return;

        if (confirm) {
            furnName = addFurnBox.GetValue(0);
            furnCostStr = addFurnBox.GetValue(1);

            CheckRoomName(furnName);
            furnCost = SToULL(furnCostStr);

            Furnishing newFurn("", furnName, furnCost);
            room->AddFurnishing(newFurn);
            return;
        }
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void UpdateFurn(Furnishing* furn)
{
    if (furn == nullptr) throw runtime_error("Khong ton tai du lieu do dung");
    
    system("cls");
    int padding = 3;
    FormBox updateFurnBox(padding, padding, 60, 0.75, "CHINH SUA DO DUNG");
    updateFurnBox.AddField("Ten do dung", furn->name);
    updateFurnBox.AddField("Gia tien", to_string(furn->price), "", " VND");

    string furnName, furnCostStr;
    ull furnCost;
    Console::Key key;
    bool confirm;
    string error;

    updateFurnBox.GoToCenter();
    updateFurnBox.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, padding, updateFurnBox.GetY() + updateFurnBox.GetH());
        error.clear();
    }
    try {
        confirm = updateFurnBox(key);
        if (key == Console::ESC) return;

        if (confirm) {
            furnName = updateFurnBox.GetValue(0);
            furnCostStr = updateFurnBox.GetValue(1);

            CheckRoomName(furnName);
            furnCost = SToULL(furnCostStr);

            furn->name = furnName;
            furn->price = furnCost;
            return;
        }
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void AddDamageToRoom(Room* room, Furnishing* furn)
{
    if (furn == nullptr) throw runtime_error("Khong ton tai du lieu do dung");
    if (room == nullptr) throw runtime_error("Khong ton tai du lieu phong");
    
    system("cls");
    int padding = 3;
    FormBox addDamageBox(padding, padding, 100, 0.8, "BAO HONG: " + furn->name);
    addDamageBox.AddField("Ngay bao *", to_string(Nhatro::today));
    addDamageBox.AddField("Mo ta *");
    addDamageBox.AddField("Boi thuong *", to_string(furn->price/2), "", " VND");

    string damDateStr, damDescr, damCostStr;
    ull damCost;
    Date damDate;
    Console::Key key;
    bool confirm;
    string error;

    addDamageBox.GoToCenter();
    addDamageBox.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, padding, addDamageBox.GetY() + addDamageBox.GetH());
        error.clear();
    }
    try {
        confirm = addDamageBox(key);
        if (key == Console::ESC) return;

        if (confirm) {
            damDateStr = addDamageBox.GetValue(0);
            damDescr = addDamageBox.GetValue(1);
            damCostStr = addDamageBox.GetValue(2);

            damDate = SToDate(damDateStr);
            damCost = SToULL(damCostStr);

            Damage newDam(damDate, furn, damDescr, damCost);
            room->AddDamage(newDam);
            return;
        }
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void UpdateDamage(Damage* dam)
{
    if (dam == nullptr) throw runtime_error("Khong ton tai du lieu bao hong");
    if (dam->furnishing == nullptr) throw runtime_error("Do dung bi hu hong khong ton tai");
    
    system("cls");
    int padding = 3;
    FormBox updateDamageBox(padding, padding, 100, 0.8, "CHINH SUA BAO HONG: " + dam->furnishing->name);
    updateDamageBox.AddField("Ngay bao", to_string(dam->date));
    updateDamageBox.AddField("Mo ta", dam->description);
    updateDamageBox.AddField("Boi thuong", to_string(dam->compen), "", " VND");

    string damDateStr, damDescr, damCostStr;
    ull damCost;
    Date damDate;
    Console::Key key;
    bool confirm;
    string error;

    updateDamageBox.GoToCenter();
    updateDamageBox.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, padding, updateDamageBox.GetY() + updateDamageBox.GetH());
        error.clear();
    }
    try {
        confirm = updateDamageBox(key);
        if (key == Console::ESC) return;

        if (confirm) {
            damDateStr = updateDamageBox.GetValue(0);
            damDescr = updateDamageBox.GetValue(1);
            damCostStr = updateDamageBox.GetValue(2);

            damDate = SToDate(damDateStr);
            damCost = SToULL(damCostStr);

            dam->date = damDate;
            dam->description = damDescr;
            dam->compen = damCost;
            return;
        }
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void UpdateRoom(Nhatro& nhatro, Room* room)
{
    NavigationBox navBox(36, 2);
    navBox.AddItem("XEM CHI TIET PHONG");
    navBox.AddItem("XEM NGUOI THUE PHONG");
    navBox.AddItem("DO DUNG-TIEN ICH");

    int startX = navBox.GetX(), startY = navBox.GetY() + 4;

    ButtonBox addTenantBtn(startX, startY, 20, "THEM NGUOI");
    ButtonBox removeTenantBtn(addTenantBtn.GetX() + addTenantBtn.GetW(), startY, 20, "XOA NGUOI");
    ButtonBox clearTenantBtn(removeTenantBtn.GetX() + removeTenantBtn.GetW(), startY, 20, "XOA TOAN BO");
    ButtonBox setLeaderBtn(clearTenantBtn.GetX() + clearTenantBtn.GetW(), startY, 20, "DOI NGUOI DAI DIEN");
    List<Tenant*> roomTenants;
    TableBox<Tenant*> roomTenantsBox(startX, addTenantBtn.GetY() + addTenantBtn.GetH(),
        TenantTableWidth(), roomTenants, TenantTableInit, TenantTableEnd, TenantTableShow);

    ButtonBox addFurnBox(startX, startY, 16, "THEM DO DUNG");
    ButtonBox removeFurnBox(addFurnBox.GetX() + addFurnBox.GetW(), startY, 16, "XOA DO DUNG");
    ButtonBox updateFurnBox(removeFurnBox.GetX() + removeFurnBox.GetW(), startY, 16, "SUA DO DUNG");
    List<Furnishing*> roomFurnishings;
    TableBox<Furnishing*> roomFurnBox(startX, addFurnBox.GetY() + addFurnBox.GetH(), FurnTableWidth(), 
        roomFurnishings, FurnTableInit, FurnTableEnd, FurnTableShow);

    ButtonBox addDamageBox(0, 0, 16, "THEM HU HONG");
    ButtonBox removeDamageBox(0, 0, 16, "XOA HU HONG");
    ButtonBox updateDamageBox(0, 0, 16, "SUA HU HONG");
    List<Damage*> roomDamages;
    TableBox<Damage*> roomDamageBox(0, addDamageBox.GetY() + addDamageBox.GetH(), DamageTableWidth(), 
                            roomDamages, DamageTableInit, DamageTableEnd, DamageTableShow);

    enum Action {
        NONE,
        NAVIGATION,
        UPDATE,
        ADD_TENANT,
        REMOVE_TENANT,
        CLEAR_TENANT,
        SET_LEADER,
        ADD_FURN,
        REMOVE_FURN,
        UPDATE_FURN,
        ADD_DAMAGE,
        REMOVE_DAMAGE,
        UPDATE_DAMAGE
    };
    InputHandler<Action> btnLink(NONE, UPDATE_DAMAGE);
    btnLink.AddRelation(ADD_TENANT,     Console::UP,    NAVIGATION);
    btnLink.AddRelation(REMOVE_TENANT,  Console::UP,    NAVIGATION);
    btnLink.AddRelation(CLEAR_TENANT,   Console::UP,    NAVIGATION);
    btnLink.AddRelation(SET_LEADER,     Console::UP,    NAVIGATION);
    btnLink.AddRelation(ADD_FURN,       Console::UP,    NAVIGATION);
    btnLink.AddRelation(REMOVE_FURN,    Console::UP,    NAVIGATION);
    btnLink.AddRelation(UPDATE_FURN,    Console::UP,    NAVIGATION);
    btnLink.AddRelation(ADD_TENANT,     Console::RIGHT, REMOVE_TENANT);
    btnLink.AddRelation(REMOVE_TENANT,  Console::RIGHT, CLEAR_TENANT);
    btnLink.AddRelation(CLEAR_TENANT,   Console::RIGHT, SET_LEADER);
    btnLink.AddRelation(REMOVE_TENANT,  Console::LEFT,  ADD_TENANT);
    btnLink.AddRelation(CLEAR_TENANT,   Console::LEFT,  REMOVE_TENANT);
    btnLink.AddRelation(SET_LEADER,     Console::LEFT,  CLEAR_TENANT);
    btnLink.AddRelation(ADD_FURN,       Console::RIGHT, REMOVE_FURN);
    btnLink.AddRelation(REMOVE_FURN,    Console::RIGHT, UPDATE_FURN);
    btnLink.AddRelation(ADD_DAMAGE,     Console::RIGHT, REMOVE_DAMAGE);
    btnLink.AddRelation(REMOVE_DAMAGE,  Console::RIGHT, UPDATE_DAMAGE);
    btnLink.AddRelation(REMOVE_FURN,    Console::LEFT,  ADD_FURN);
    btnLink.AddRelation(UPDATE_FURN,    Console::LEFT,  REMOVE_FURN);
    btnLink.AddRelation(REMOVE_DAMAGE,  Console::LEFT,  ADD_DAMAGE);
    btnLink.AddRelation(UPDATE_DAMAGE,  Console::LEFT,  REMOVE_DAMAGE);
    btnLink.AddRelation(ADD_FURN,       Console::DOWN,  ADD_DAMAGE);
    btnLink.AddRelation(REMOVE_FURN,    Console::DOWN,  REMOVE_DAMAGE);
    btnLink.AddRelation(UPDATE_FURN,    Console::DOWN,  UPDATE_DAMAGE);
    btnLink.AddRelation(ADD_DAMAGE,     Console::UP,    ADD_FURN);
    btnLink.AddRelation(REMOVE_DAMAGE,  Console::UP,    REMOVE_FURN);
    btnLink.AddRelation(UPDATE_DAMAGE,  Console::UP,    UPDATE_FURN);

    Console::Key key;
    bool redraw = true;
    bool isClick;
    int choice;
    int page = 0, currPage;
    Action action = NAVIGATION;
    string error;

    while (true) {
    try {
        if (redraw) {
            system("cls");
            navBox.Draw();
        }
        if (!error.empty()) {
            Console::DrawManagerError(error, startX, startY - 1);
            error.clear();
        }

        if (page == 0) {
            if (redraw) {
                GetRoomDetails(room, startX, startY, true);
                redraw = false;
            }

            if (action == UPDATE) {
                unique_ptr<Room> tmpRoom = nullptr;
                if (GetRoomForm(startX, startY, nhatro, tmpRoom, room))
                    nhatro.SortRoomsById();
                action = NAVIGATION;
                redraw = true;
            }
        }
        else if (page == 1) {
            if (redraw) {
                addTenantBtn.Draw();
                removeTenantBtn.Draw();
                clearTenantBtn.Draw();
                setLeaderBtn.Draw();
                roomTenants = room->GetTenants();
                roomTenantsBox.Draw();
                redraw = false;
            }

            switch (action) {
                case ADD_TENANT:
                    isClick = addTenantBtn(key);
                    if (isClick) {
                        AddTenantToRoom(nhatro, room);
                        redraw = true;
                    }
                    break;
                case REMOVE_TENANT:
                    isClick = removeTenantBtn(key);
                    if (isClick) {
                        int choice = roomTenantsBox(key, true);
                        if (choice != -1) {
                            bool rmv = true;
                            if (roomTenants[choice]->IsLeader()) {
                                rmv = WarningBox::Warning(30, "CANH BAO", 
                                    "Xac nhan xoa nguoi dai dien phong", "XAC NHAN", "HUY");
                            }
                            if (rmv) room->RemoveTenant(roomTenants[choice]);
                            redraw = true;
                        }
                    }
                    break;
                case CLEAR_TENANT:
                    isClick = clearTenantBtn(key);
                    if (isClick) {
                        bool clr = true;
                        if (room->IsRented()) clr = WarningBox::Warning(30, "CANH BAO", 
                            "Xac nhan xoa toan bo nguoi thue", "XAC NHAN", "HUY");
                        if (clr) for (const auto& per : roomTenants) {
                            room->RemoveTenant(per);
                        }
                        redraw = true;
                    }
                    break;
                case SET_LEADER:
                    isClick = setLeaderBtn(key);
                    if (isClick) {    
                        choice = roomTenantsBox(key);
                        if (choice != -1) {
                            bool confirm = false;
                            if (!roomTenants[choice]->IsLeader()) confirm = WarningBox::Warning(30, "CANH BAO", 
                                "Xac nhan thay doi nguoi dai dien phong", "XAC NHAN", "HUY");
                            if (confirm) room->SetRoomLeader(roomTenants[choice]);
                            redraw = true;
                        }
                    }
                    break;
                default: break;
            }
        }
        else if (page == 2) {
            if (redraw) {
                addFurnBox.Draw();
                removeFurnBox.Draw();
                updateFurnBox.Draw();
                auto& furn = room->GetFurnishings();
                roomFurnishings.clear();
                for (const auto& it : furn) roomFurnishings.push_back(const_cast<Furnishing*>(&it));
                roomFurnBox.Draw();

                addDamageBox.ResetPosition(startX, roomFurnBox.GetY() + roomFurnBox.GetH());
                addDamageBox.Draw();
                removeDamageBox.ResetPosition(addDamageBox.GetX() + addDamageBox.GetW(), addDamageBox.GetY());
                removeDamageBox.Draw();
                updateDamageBox.ResetPosition(removeDamageBox.GetX() + removeDamageBox.GetW(), addDamageBox.GetY());
                updateDamageBox.Draw();
                auto& dam = room->GetDamages();
                roomDamages.clear();
                for (const auto& it : dam) roomDamages.push_back(const_cast<Damage*>(&it));
                roomDamageBox.ResetPosition(startX, addDamageBox.GetY() + addDamageBox.GetH());
                roomDamageBox.Draw();
                redraw = false;
            }

            switch (action) {
                case ADD_FURN:
                    isClick = addFurnBox(key);
                    if (isClick) {
                        AddFurnToRoom(room);
                        redraw = true;
                    }
                    break;
                case REMOVE_FURN:
                    isClick = removeFurnBox(key);
                    if (isClick) {
                        int choice = roomFurnBox(key, true);
                        if (choice != -1) {
                            room->RemoveFurnishing(roomFurnishings[choice]);
                            redraw = true;
                        }
                    }
                    break;
                case UPDATE_FURN:
                    isClick = updateFurnBox(key);
                    if (isClick) {
                        int choice = roomFurnBox(key, true);
                        if (choice != -1) {
                            UpdateFurn(roomFurnishings[choice]);
                            redraw = true;
                        }
                    }
                    break;
                case ADD_DAMAGE:
                    isClick = addDamageBox(key);
                    if (isClick) {
                        int choice = roomFurnBox(key);
                        if (choice != -1) {
                            AddDamageToRoom(room, roomFurnishings[choice]);
                            redraw = true;
                        }
                    }
                    break;
                case REMOVE_DAMAGE:
                    isClick = removeDamageBox(key);
                    if (isClick) {
                        int choice = roomDamageBox(key, true);
                        if (choice != -1) {
                            room->RemoveDamage(roomDamages[choice]);
                            redraw = true;
                        }
                    }
                    break;
                case UPDATE_DAMAGE:
                    isClick = updateDamageBox(key);
                    if (isClick) {
                        int choice = roomDamageBox(key, true);
                        if (choice != -1) {
                            UpdateDamage(roomDamages[choice]);
                            redraw = true;
                        }
                    }
                    break;
                default: break;
            }
        }

        action = btnLink.GetDest(action, key);

        currPage = page;
        if (action == NAVIGATION) page = navBox(key);
        if (page != currPage) redraw = true;

        if (action == NAVIGATION && (key == Console::DOWN || key == Console::ENTER)) {
            switch (page)
            {
            case 0:
                action = UPDATE;
                break;
            case 1:
                action = ADD_TENANT;
                break;
            case 2:
                action = ADD_FURN;
                break;
            default:
                break;
            }
        }

        if (key == Console::ESC) return;
        if (key == Console::F5) redraw = true;
    }
    catch (const exception& e) {
        error = e.what();
    }
    catch (const char* e) {
        error = e;
    }
    }
}

void RoomTableInit(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_TL 
        << Repeat(ROOM_TYPE_W, BORDER_H)      << BORDER_DT
        << Repeat(ROOM_ID_W, BORDER_H)        << BORDER_DT
        << Repeat(ROOM_NAME_W, BORDER_H)      << BORDER_DT
        << Repeat(ROOM_CAP_W, BORDER_H)       << BORDER_DT
        << Repeat(ROOM_FLOOR_W, BORDER_H)     << BORDER_DT
        << Repeat(ROOM_PRICE_W, BORDER_H)     << BORDER_DT
        << Repeat(ROOM_AVAILABLE_W, BORDER_H)
        << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(ROOM_TYPE_W)    << "Loai phong"   << BORDER_V
        << setw(ROOM_ID_W)      << "Ma phong"     << BORDER_V
        << setw(ROOM_NAME_W)    << "Ten phong"    << BORDER_V
        << setw(ROOM_CAP_W)     << "So luong"     << BORDER_V
        << setw(ROOM_FLOOR_W)   << "Tang"         << BORDER_V
        << setw(ROOM_PRICE_W)   << "Gia thue"     << BORDER_V
        << setw(ROOM_AVAILABLE_W) << "Trang thai" << BORDER_V;

    Console::Goto(x, y+2);
    cout << BORDER_RT
        << Repeat(ROOM_TYPE_W, BORDER_H)      << BORDER_CROSS
        << Repeat(ROOM_ID_W, BORDER_H)        << BORDER_CROSS
        << Repeat(ROOM_NAME_W, BORDER_H)      << BORDER_CROSS
        << Repeat(ROOM_CAP_W, BORDER_H)       << BORDER_CROSS
        << Repeat(ROOM_FLOOR_W, BORDER_H)     << BORDER_CROSS
        << Repeat(ROOM_PRICE_W, BORDER_H)     << BORDER_CROSS
        << Repeat(ROOM_AVAILABLE_W, BORDER_H)
        << BORDER_LT;
}
void RoomTableEnd(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_BL 
        << Repeat(ROOM_TYPE_W, BORDER_H)      << BORDER_UT
        << Repeat(ROOM_ID_W, BORDER_H)        << BORDER_UT
        << Repeat(ROOM_NAME_W, BORDER_H)      << BORDER_UT
        << Repeat(ROOM_CAP_W, BORDER_H)       << BORDER_UT
        << Repeat(ROOM_FLOOR_W, BORDER_H)     << BORDER_UT
        << Repeat(ROOM_PRICE_W, BORDER_H)     << BORDER_UT
        << Repeat(ROOM_AVAILABLE_W, BORDER_H)
        << BORDER_BR;
}
int RoomTableWidth()
{
    return ROOM_TYPE_W + ROOM_ID_W + ROOM_NAME_W + ROOM_CAP_W 
            + ROOM_FLOOR_W + ROOM_PRICE_W + ROOM_AVAILABLE_W + 8;
}
void RoomTableShow(Room* const& room)
{
    if (room == nullptr) {
        cout << "Khong ton tai du lieu";
        return;
    }
    cout << std::left << BORDER_V
       << std::setw(ROOM_TYPE_W) << room->GetRoomType() << BORDER_V
       << std::setw(ROOM_ID_W) << room->GetId() << BORDER_V
       << std::setw(ROOM_NAME_W) << postsub(ROOM_NAME_W, room->GetName()) << BORDER_V
       << std::setw(ROOM_CAP_W) << std::to_string(room->GetTenantCount()) + "/" + std::to_string(room->GetCapacity()) << BORDER_V
       << std::setw(ROOM_FLOOR_W) << room->GetFloor() << BORDER_V
       << std::setw(ROOM_PRICE_W) << std::to_string(room->GetRentPrice()).substr(0, ROOM_PRICE_W) << BORDER_V
       << std::setw(ROOM_AVAILABLE_W) << (room->IsRented() ? (room->IsFull() ? "HET" : "CON") : "TRONG") << BORDER_V;
}

void FurnTableInit(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_TL
        << Repeat(F_ID_W, BORDER_H)     << BORDER_DT
        << Repeat(F_NAME_W, BORDER_H)   << BORDER_DT
        << Repeat(F_PRICE_W, BORDER_H)  
        << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(F_ID_W) << "Ma so" << BORDER_V
        << setw(F_NAME_W) << "Tien ich" << BORDER_V
        << setw(F_PRICE_W) << "Gia tri" << BORDER_V;

    Console::Goto(x, y+2);
    cout << BORDER_RT
        << Repeat(F_ID_W, BORDER_H)     << BORDER_CROSS
        << Repeat(F_NAME_W, BORDER_H)   << BORDER_CROSS
        << Repeat(F_PRICE_W, BORDER_H)  
        << BORDER_LT;
}
void FurnTableEnd(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_BL
        << Repeat(F_ID_W, BORDER_H)     << BORDER_UT
        << Repeat(F_NAME_W, BORDER_H)   << BORDER_UT
        << Repeat(F_PRICE_W, BORDER_H)  
        << BORDER_BR;
}
int FurnTableWidth()
{
    return F_ID_W + F_NAME_W + F_PRICE_W + 4;
}
void FurnTableShow(Furnishing* const& furn)
{
    if (furn == nullptr) {
        cout << "Khong ton tai du lieu";
        return;
    }
    cout << left << BORDER_V
        << setw(F_ID_W) << furn->id << BORDER_V
        << setw(F_NAME_W) << presub(F_NAME_W, furn->name) << BORDER_V
        << setw(F_PRICE_W) << furn->price << BORDER_V;
}

void DamageTableInit(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_TL
        << Repeat(DAM_DATE_W, BORDER_H)         << BORDER_DT
        << Repeat(DAM_FURN_W, BORDER_H)         << BORDER_DT
        << Repeat(DAM_DESCRIPTION_W, BORDER_H)  << BORDER_DT
        << Repeat(DAM_COMPEN_W, BORDER_H)     
        << BORDER_TR;

    Console::Goto(x, y+1);
    cout << BORDER_V
        << setw(DAM_DATE_W)         << "Ngay"    << BORDER_V
        << setw(DAM_FURN_W)         << "Do dung" << BORDER_V
        << setw(DAM_DESCRIPTION_W)  << "Mo ta"   << BORDER_V
        << setw(DAM_COMPEN_W)       << "Den bu"  << BORDER_V;

    Console::Goto(x, y+2);
    cout << BORDER_RT
        << Repeat(DAM_DATE_W, BORDER_H)         << BORDER_CROSS
        << Repeat(DAM_FURN_W, BORDER_H)         << BORDER_CROSS
        << Repeat(DAM_DESCRIPTION_W, BORDER_H)  << BORDER_CROSS
        << Repeat(DAM_COMPEN_W, BORDER_H)   
        << BORDER_LT;
}
void DamageTableEnd(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_BL
        << Repeat(DAM_DATE_W, BORDER_H)         << BORDER_UT
        << Repeat(DAM_FURN_W, BORDER_H)         << BORDER_UT
        << Repeat(DAM_DESCRIPTION_W, BORDER_H)  << BORDER_UT
        << Repeat(DAM_COMPEN_W, BORDER_H) 
        << BORDER_BR;
}
int DamageTableWidth()
{
    return DAM_DATE_W + DAM_FURN_W + DAM_DESCRIPTION_W + DAM_COMPEN_W + 5;
}
void DamageTableShow(Damage* const& damn)
{
    if (damn == nullptr) {
        cout << "Khong ton tai du lieu";
        return;
    }
    cout << left << BORDER_V
        << setw(DAM_DATE_W) << damn->date << BORDER_V
        << setw(DAM_FURN_W) << (damn->furnishing ? presub(DAM_FURN_W, damn->furnishing->name) 
            : "Khong ro") << BORDER_V
        << setw(DAM_DESCRIPTION_W) << presub(DAM_DESCRIPTION_W, damn->description) << BORDER_V
        << setw(DAM_COMPEN_W) << damn->compen << BORDER_V
        << endl;
}
