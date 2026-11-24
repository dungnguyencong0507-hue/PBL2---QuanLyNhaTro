#include <RoomManager.h>
using namespace std;

void ManageTenants(Nhatro& nhatro)
{
    auto& tenants = nhatro.GetTenants();
    Manager<Tenant> tenantManager("KHACH THUE", nhatro, tenants, TenantTableWidth(), TenantTableInit, 
                            TenantTableEnd, TenantTableShow, SearchTenant, AddTenant, UpdateTenant);
    tenantManager.AddFilter("Khach dang thue", FilterRentingTenants);
    tenantManager.AddFilter("Khach chua thue", FilterNotRentingTenants);
    tenantManager.AddFilter("Nguoi dai dien", FilterRepresentativeTenants);
    tenantManager.AddFilter("Chua dang ki tam tru", FilterTenantsNotTmpRsd);
    tenantManager.AddFilter("Da dang ki tam tru", FilterTenantsGotTmpRsd);
    tenantManager.AddSortComparison("Theo ten", CompareTenantsName);
    tenantManager.AddSortComparison("Theo gioi tinh", CompareTenantsGender);
    tenantManager.AddSortComparison("Theo phong", CompareTenantsRoom);

    tenantManager.Draw();
}

bool SearchTenant(Tenant* tenant, const string& query) // Theo ten, cccd, sdt, que quan, phong
{
    if (tenant == nullptr) return false;
    if (query == tenant->GetId()) return true;

    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    string tenantInfo = tenant->GetName();
    transform(tenantInfo.begin(), tenantInfo.end(), tenantInfo.begin(), ::tolower);
    if (tenantInfo.find(lowerQuery) != string::npos) return true;

    tenantInfo = tenant->GetCCCD();
    if (tenantInfo.find(lowerQuery) != string::npos) return true;

    tenantInfo = tenant->GetPhone();
    if (tenantInfo.find(lowerQuery) != string::npos) return true;

    tenantInfo = tenant->GetHometown();
    transform(tenantInfo.begin(), tenantInfo.end(), tenantInfo.begin(), ::tolower);
    if (tenantInfo.find(lowerQuery) != string::npos) return true;

    if (SearchRooms(tenant->GetRoom(), query)) return true; 

    return false;
}
bool FilterRentingTenants(Tenant* tenant)
{
    return tenant->IsRenting();
}
bool FilterNotRentingTenants(Tenant* tenant)
{
    return !tenant->IsRenting();
}
bool FilterRepresentativeTenants(Tenant* tenant)
{
    return tenant->IsLeader();
}
bool FilterTenantsGotTmpRsd(Tenant* tenant)
{
    return tenant->IsTemporaryResidence();
}
bool FilterTenantsNotTmpRsd(Tenant* tenant){
    return !tenant->IsTemporaryResidence();
}
bool CompareTenantsName(Tenant* tenant1, Tenant* tenant2)
{
    string fullname1 = tenant1->GetName();
    int i = fullname1.rfind(' ', tenant1->GetName().size() - 1);
    string lastname1 = fullname1.substr(i + 1, fullname1.size() - i);
    
    string fullname2 = tenant2->GetName();
    int j = fullname2.rfind(' ', tenant2->GetName().size() - 1);
    string lastname2 = fullname2.substr(j + 1, fullname2.size() - i);

    if (lastname1 == lastname2) return fullname1 < fullname2;
    return lastname1 < lastname2;
}
bool CompareTenantsGender(Tenant* tenant1, Tenant* tenant2)
{
    return tenant1->GetGender() < tenant2->GetGender();
}
bool CompareTenantsRoom(Tenant* tenant1, Tenant* tenant2)
{
    if (tenant1->GetRoom() == tenant2->GetRoom()) return false;
    if (tenant2->GetRoom() == nullptr) return true;
    if (tenant1->GetRoom() == nullptr) return false;
    return tenant1->GetRoom()->GetId() < tenant2->GetRoom()->GetId();
}

FormBox GetTenantDetails(Tenant* tenant, int x, int y, bool draw = 0)
{
    if (tenant == nullptr) throw runtime_error("Khong ton tai du lieu khach thue");

    FormBox tenantDetailsBox(x, y, 70, 0.5, "THONG TIN KHACH THUE");
    tenantDetailsBox.AddConstField("Ma khach thue", tenant->GetId());
    tenantDetailsBox.AddField("Ho va ten", tenant->GetName());
    tenantDetailsBox.AddChosenField("Gioi tinh", {"NAM", "NU"}, (tenant->GetGender() == Gender::Male ? 0 : 1));
    tenantDetailsBox.AddField("So dien thoai", tenant->GetPhone());
    tenantDetailsBox.AddField("Can cuoc cong dan", tenant->GetCCCD());
    tenantDetailsBox.AddField("Que quan", tenant->GetHometown());
    tenantDetailsBox.AddChosenField("Dang ky tam tru", {"CHUA", "ROI"}, tenant->IsTemporaryResidence());
    string status = (tenant->IsRenting() ? (tenant->IsLeader() ? "Truong phong " + tenant->GetRoom()->GetId() 
                    : "Dang thue phong " + tenant->GetRoom()->GetId()) : "Chua thue phong");
    tenantDetailsBox.AddConstField("Trang thai", status);

    if (draw) tenantDetailsBox.Draw();
    return tenantDetailsBox;
}

void GetTenantForm(int x, int y, Nhatro& nhatro, Tenant* tenant = nullptr)
{
    FormBox tenantForm(x, y, 70, 0.5, "THEM KHACH THUE MOI");
    if (tenant) {
        tenantForm = GetTenantDetails(tenant, x, y);
    }
    else {
        tenantForm.AddField("Ho va ten *");
        tenantForm.AddChosenField("Gioi tinh *", {"NAM", "NU"});
        tenantForm.AddField("So dien thoai *");
        tenantForm.AddField("Can cuoc cong dan *");
        tenantForm.AddField("Que quan");
        tenantForm.GoToCenter(x, y);
    }

    string tenantName, tenantPhone, tenantCCCD, tenantHometown;
    int gender, tmpRsd;
    Console::Key key;
    bool confirm;
    string error;

    tenantForm.Draw();
    while (true) {
    if (!error.empty()) {
        Console::DrawManagerError(error, x, tenantForm.GetY() + tenantForm.GetH());
        error.clear();
    }
    try {
        confirm = tenantForm(key);
        if (key == Console::ESC) return;

        if (confirm) {
            if (tenant) {
                tenantName = tenantForm.GetValue(1);
                gender = tenantForm.GetChosenValue(2);
                tenantPhone = tenantForm.GetValue(3);
                tenantCCCD = tenantForm.GetValue(4);
                tenantHometown = tenantForm.GetValue(5);
                tmpRsd = tenantForm.GetChosenValue(6);
            }
            else {
                tenantName = tenantForm.GetValue(0);
                gender = tenantForm.GetChosenValue(1);
                tenantPhone = tenantForm.GetValue(2);
                tenantCCCD = tenantForm.GetValue(3);
                tenantHometown = tenantForm.GetValue(4);
            }

            CheckPersonName(tenantName);
            CheckPhoneNum(tenantPhone);
            CheckCCCD(tenantCCCD);
            if (tenantHometown != "" || tenant != nullptr) CheckPersonName(tenantHometown); 

            Gender gend = (gender == 0 ? Gender::Male : Gender::Female);
            if (tenant == nullptr) {
                unique_ptr<Tenant> newTenant = make_unique<Tenant>
                    (tenantName, gend, tenantPhone, tenantCCCD, tenantHometown);
                nhatro.AddTenant(move(newTenant));
            }
            else {
                tenant->SetName(tenantName);
                tenant->SetGender(gend);
                tenant->SetPhone(tenantPhone);
                tenant->SetCCCD(tenantCCCD);
                tenant->SetHometown(tenantHometown);
                tenant->SetTemporaryResidence(tmpRsd == 1);
            }
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

void AddTenant(Nhatro& nhatro)
{
    system("cls");
    GetTenantForm(3, 3, nhatro);
}

void AddTenantToRoom(Nhatro& nhatro, Tenant* tenant)
{
    if (tenant == nullptr) throw runtime_error("Khong ton tai du lieu khach thue");
    system("cls");
    int padding = 3;

    string query = "";
    SingleLineBox searchRoomBox(padding, padding, 50, "TIM KIEM", query);

    List<Room*> availRooms, results;
    auto& rooms = nhatro.GetRooms();
    for (const auto& per : rooms) {
        if (!per.get()->IsFull()) availRooms.push_back(per.get());
    }
    results = availRooms;
    TableBox<Room*> availRoomsBox(searchRoomBox.GetX(), searchRoomBox.GetY()+searchRoomBox.GetH(), 
                    RoomTableWidth(), results, RoomTableInit, RoomTableEnd, RoomTableShow);

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
            cout << "CHECK IN CHO KHACH THUE " << tenant->GetName() << " (SDT: " << tenant->GetPhone() << ")";
            searchRoomBox.Draw();
            availRoomsBox.Draw();
            redraw = false;
        }
        if (!error.empty()) {
            Console::DrawManagerError(error, searchRoomBox.GetX(), searchRoomBox.GetY() - 1);
            error.clear();
        }

        if (search) {
            searchRoomBox(key);
            if (key == Console::DOWN) search = false;

            if (key == Console::ENTER) {
                results.clear();
                int id;
                if ((id = nhatro.FindRoom(query)) != -1) {
                    if (!rooms[id]->IsFull()) results.push_back(rooms[id].get());
                }
                else {
                    for (const auto& per : availRooms) {
                        if (SearchRooms(per, query)) results.push_back(per);
                    }
                }

                availRoomsBox.Draw();
            }
        }
        else {
            int choice = availRoomsBox(key);

            if (choice != -1) {
                Gender currGend;
                bool singleGender = results[choice]->IsSingleGender(currGend);
                if (singleGender && currGend != tenant->GetGender()) {
                    bool confirm = WarningBox::Warning(30, "CANH BAO", 
                        "Co xac nhan them nguoi khac gioi vao phong khong?");
                    redraw = true;
                    if (!confirm) continue;
                }
                if (!results[choice]->IsRented()) results[choice]->SetDate(Nhatro::today);
                results[choice]->AddTenant(tenant);
                break;
            }
            else search = true;
            key = Console::OTHER;
        }

        if (key == Console::ESC) return;
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

void UpdateTenant(Nhatro& nhatro, Tenant* tenant)
{
    NavigationBox navBox(50, 2);
    navBox.AddItem("XEM THONG TIN KHACH THUE");
    navBox.AddItem("NHAN / TRA PHONG");
    navBox.Draw();

    int startX = navBox.GetX(), startY = navBox.GetY() + 4;

    ButtonBox checkInBtn(startX, startY, 20, "NHAN PHONG");
    ButtonBox checkOutBtn(startX, startY, 20, "TRA PHONG");

    enum Action {
        NONE,
        NAVIGATION,
        UPDATE,
        IN_OUT
    };
    InputHandler<Action> btnLink(NONE, IN_OUT);
    btnLink.AddRelation(IN_OUT, Console::UP, NAVIGATION);

    Console::Key key;
    bool redraw = true;
    bool isClick;
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
                GetTenantDetails(tenant, startX, startY, true);
                redraw = false;
            }

            if (action == UPDATE) {
                GetTenantForm(startX, startY, nhatro, tenant);
                action = NAVIGATION;
                redraw = true;
            }
        }
        else if (page == 1) {
            if (redraw) {
                if (tenant->GetRoom() != nullptr) {
                    checkOutBtn.Draw();
                    GetRoomDetails(tenant->GetRoom(), startX, checkInBtn.GetY() + checkInBtn.GetH(), true);
                }
                else {
                    checkInBtn.Draw();
                }
                redraw = false;
            }

            if (action == IN_OUT) {
                if (tenant->GetRoom() != nullptr) {
                    isClick = checkOutBtn(key);
                    if (isClick) {
                        redraw = true;
                        bool confirm = WarningBox::Warning(30, "XAC NHAN", 
                            "Xac nhan khach thue tra phong", "XAC NHAN", "HUY");
                        if (confirm) tenant->GetRoom()->RemoveTenant(tenant);
                    }
                }
                else {
                    isClick = checkInBtn(key);
                    if (isClick) {
                        AddTenantToRoom(nhatro, tenant);
                        redraw = true;
                    }
                }
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
                action = IN_OUT;
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

void TenantTableInit(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_TL
        << Repeat(TENANT_ID_W, BORDER_H)      << BORDER_DT
        << Repeat(TENANT_NAME_W, BORDER_H)    << BORDER_DT
        << Repeat(TENANT_CCCD_W, BORDER_H)    << BORDER_DT
        << Repeat(TENANT_PHONE_W, BORDER_H)   << BORDER_DT
        << Repeat(TENANT_GENDER_W, BORDER_H)  << BORDER_DT
        << Repeat(TENANT_ROOM_W, BORDER_H)    << BORDER_DT
        << Repeat(TENANT_TMP_RSD_W, BORDER_H) << BORDER_DT
        << Repeat(TENANT_NOTE_W, BORDER_H)
        << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(TENANT_ID_W)        << "Ma khach"       << BORDER_V
        << setw(TENANT_NAME_W)      << "Ho va ten"      << BORDER_V
        << setw(TENANT_CCCD_W)      << "CCCD"           << BORDER_V
        << setw(TENANT_PHONE_W)     << "SDT"            << BORDER_V
        << setw(TENANT_GENDER_W)    << "Gioi tinh"      << BORDER_V
        << setw(TENANT_ROOM_W)      << "Dang thue phong"<< BORDER_V
        << setw(TENANT_TMP_RSD_W)   << "Dang ki tam tru"<< BORDER_V
        << setw(TENANT_NOTE_W)      << "Ghi chu"        << BORDER_V;

    Console::Goto(x, y+2);
    cout << BORDER_RT
        << Repeat(TENANT_ID_W, BORDER_H)      << BORDER_CROSS
        << Repeat(TENANT_NAME_W, BORDER_H)    << BORDER_CROSS
        << Repeat(TENANT_CCCD_W, BORDER_H)    << BORDER_CROSS
        << Repeat(TENANT_PHONE_W, BORDER_H)   << BORDER_CROSS
        << Repeat(TENANT_GENDER_W, BORDER_H)  << BORDER_CROSS
        << Repeat(TENANT_ROOM_W, BORDER_H)    << BORDER_CROSS
        << Repeat(TENANT_TMP_RSD_W, BORDER_H) << BORDER_CROSS
        << Repeat(TENANT_NOTE_W, BORDER_H)
        << BORDER_LT;
}
void TenantTableEnd(int x, int y)
{
    Console::Goto(x, y);
    cout << BORDER_BL
        << Repeat(TENANT_ID_W, BORDER_H)      << BORDER_UT
        << Repeat(TENANT_NAME_W, BORDER_H)    << BORDER_UT
        << Repeat(TENANT_CCCD_W, BORDER_H)    << BORDER_UT
        << Repeat(TENANT_PHONE_W, BORDER_H)   << BORDER_UT
        << Repeat(TENANT_GENDER_W, BORDER_H)  << BORDER_UT
        << Repeat(TENANT_ROOM_W, BORDER_H)    << BORDER_UT
        << Repeat(TENANT_TMP_RSD_W, BORDER_H) << BORDER_UT
        << Repeat(TENANT_NOTE_W, BORDER_H)
        << BORDER_BR;
}
int TenantTableWidth()
{
    return TENANT_ID_W + TENANT_NAME_W + TENANT_CCCD_W + TENANT_PHONE_W + TENANT_GENDER_W 
            + TENANT_ROOM_W + TENANT_TMP_RSD_W + TENANT_NOTE_W + 9;
}
void TenantTableShow(Tenant* const& tenant)
{
    if (tenant == nullptr) {
        cout << "Khong ton tai du lieu";
        return;
    }
    cout << std::left << BORDER_V
        << setw(TENANT_ID_W)        << tenant->GetId()      << BORDER_V
        << setw(TENANT_NAME_W)      << postsub(TENANT_NAME_W, tenant->GetName())     << BORDER_V
        << setw(TENANT_CCCD_W)      << tenant->GetCCCD()   << BORDER_V
        << setw(TENANT_PHONE_W)     << tenant->GetPhone()   << BORDER_V
        << setw(TENANT_GENDER_W)    << (tenant->GetGender() == Male ? "Nam" : "Nu")   << BORDER_V
        << setw(TENANT_ROOM_W)      << (tenant->IsRenting()
            ? postsub(TENANT_ROOM_W, tenant->GetRoom()->GetName()) : "")        << BORDER_V
        << setw(TENANT_TMP_RSD_W) << (tenant->IsTemporaryResidence() ? "Roi" : "Chua") << BORDER_V
        << setw(TENANT_NOTE_W) << (tenant->IsLeader() ? "TRUONG PHONG" : "") << BORDER_V;
}