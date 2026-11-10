#include <Nhatro.h>
#include <vector>
using namespace std;

Date Nhatro::today = { 1, 1, 2024 };

Nhatro::Nhatro(const string& name, const string& address, const string& phone)
    : name(name), address(address), phone(phone)
{}

Nhatro::~Nhatro()
{}

Nhatro& Nhatro::GetInstance(const string& name, const string& address, const string& phone)
{
    static Nhatro instance(name, address, phone);
    return instance;
}

void Nhatro::SetAddress(const string& newAddress)
{
    this->address = newAddress;
}
void Nhatro::SetPhone(const string& newPhone)
{
    this->phone = newPhone;
}
const string& Nhatro::GetName() const
{
    return this->name;
}
void Nhatro::SetName(const string& newName)
{
    this->name = newName;
}
void Nhatro::SetDate(const Date& date)
{
    Nhatro::today = date;
}

const string& Nhatro::GetAddress() const
{
    return this->address;
}
const string& Nhatro::GetPhone() const
{
    return this->phone;
}
const Date& Nhatro::GetDate() const
{
    return Nhatro::today;
}
int Nhatro::GetNumRoom() const
{
    return this->rooms.size();
}

int Nhatro::GetNumTenant() const
{
    return this->tenants.size();
}

const List<unique_ptr<Room>>& Nhatro::GetRooms()
{
    return this->rooms;
}

const List<unique_ptr<Tenant>>& Nhatro::GetTenants()
{
    return this->tenants;
}

void Nhatro::AddRoom(unique_ptr<Room> newRoom)
{
    if (!newRoom) throw runtime_error("Khong the tao phong moi!");
    if (this->FindRoom(newRoom->GetId()) != -1) throw runtime_error("Phong da ton tai!");
    
    int left = 0;
    int right = this->rooms.size();
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (this->rooms[mid]->GetId() < newRoom->GetId()) left = mid + 1;
        else right = mid;
    }
    this->rooms.insert(left, move(newRoom));
}

void Nhatro::RemoveRoom(Room* targetRoom)
{
    if (targetRoom == nullptr) throw runtime_error("Khong ton tai du lieu phong!");
    if (targetRoom->GetTenantCount() > 0) throw runtime_error("Phong con nguoi thue, khong the xoa!");
    if (targetRoom->GetDebt() > 0) throw runtime_error("Phong con tien no, khong the xoa!");
    int roomIndex = this->FindRoom(targetRoom->GetId());
    if (roomIndex == -1) throw runtime_error("Khong tim thay phong!");
    if (this->rooms[roomIndex].get() != targetRoom)
        throw runtime_error("Phong khong thuoc nha tro nay!");

    this->rooms.erase(this->rooms.begin() + roomIndex);
}

void Nhatro::AddTenant(unique_ptr<Tenant> newTenant)
{
    if (newTenant == nullptr) throw runtime_error("Khong the them nguoi thue moi!");
    if (this->FindTenant(newTenant->GetId()) != -1) throw runtime_error("Nguoi thue da ton tai!");
    this->tenants.push_back(std::move(newTenant));
}

void Nhatro::TenantCheckInRoom(Tenant* tenant, Room* room)
{
    if (tenant == nullptr || room == nullptr) throw runtime_error("Khong ton tai du lieu nguoi thue!");
    room->AddTenant(tenant);
}
void Nhatro::TenantCheckOutRoom(Tenant* tenant, Room* room)
{
    if (tenant == nullptr || room == nullptr) throw runtime_error("Khong ton tai du lieu nguoi thue!");
    room->RemoveTenant(tenant);
}

bool CmpRoomsId(const unique_ptr<Room>& a, const unique_ptr<Room>& b)
{
    return a->GetId() < b->GetId();
}

void Nhatro::SortRoomsById()
{
    this->rooms.sort(CmpRoomsId);
}

ostream& operator<<(ostream& o, const Nhatro& nhatro)
{
    o << "Thong tin " << nhatro.name << ":\n"
        << "Dia chi: " << nhatro.address << endl
        << "Lien he: " << nhatro.phone << endl;
    return o;
}

int Nhatro::FindRoom(const std::string& id)
{
    int left = 0, right = this->rooms.size() - 1;
    while (left <= right) { 
        int mid = left + (right - left) / 2;
        if (this->rooms[mid]->GetId() == id) {
            return mid;
        }
        else if (this->rooms[mid]->GetId() < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}
    
int Nhatro::FindTenant(const std::string& id)
{
    int left = 0, right = this->tenants.size() - 1;
    while (left <= right) { 
        int mid = left + (right - left) / 2;
        if (this->tenants[mid]->GetId() == id) {
            return mid;
        }
        else if (this->tenants[mid]->GetId() < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}

void Nhatro::LoadRoomData(const std::string& roomFile, const std::string& billFile, const std::string& furnishingFile, const std::string& damageFile)
{
    ifstream roomInp("data/" + roomFile);
    if (!roomInp) throw runtime_error("Khong the tai file du lieu phong!");
{
    string line, id, type, name, cap, floor, rentPrice, prepaid, oldElec, newElec, oldWater, newWater, rentDate;
    while (getline(roomInp, line)){
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        istringstream iss(line);
        getline(iss, id, '|');
        getline(iss, type, '|');
        getline(iss, name, '|');
        getline(iss, cap, '|');
        getline(iss, floor, '|');
        getline(iss, rentPrice, '|');
        getline(iss, prepaid, '|');
        getline(iss, oldElec, '|');
        getline(iss, newElec, '|');
        getline(iss, oldWater, '|');
        getline(iss, newWater, '|');
        getline(iss, rentDate);

    try {
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
        if (!rentDate.empty()) newRoom->SetDate(Date(rentDate));

        this->AddRoom(move(newRoom));
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
    catch (const string& e) {
        cout << e << endl;
    }
    }
    this->SortRoomsById();
}
    roomInp.close();
    cout << "Tai du lieu phong thanh cong!\n";

    ifstream billInp("data/" + billFile);
    if (!billInp) throw runtime_error("Khong the tai file du lieu hoa don!");
{
    int index = 0;
    string line, roomId, month, rentCost, trashCost, wifiCost, serviceCost, electricCost, waterCost, damageCost, oldElecIndex, newElecIndex, oldWaterIndex, newWaterIndex, totalCost, isPaid, paymentDate;
    while (getline(billInp, line)){
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        istringstream iss(line);
        getline(iss, roomId,        '|');
        getline(iss, month,         '|');
        getline(iss, rentCost,      '|');
        getline(iss, trashCost,     '|');
        getline(iss, wifiCost,      '|');
        getline(iss, serviceCost,   '|');
        getline(iss, electricCost,  '|');
        getline(iss, waterCost,     '|');
        getline(iss, damageCost,    '|');
        getline(iss, oldElecIndex,  '|');
        getline(iss, newElecIndex,  '|');
        getline(iss, oldWaterIndex, '|');
        getline(iss, newWaterIndex, '|');
        getline(iss, totalCost,     '|');
        getline(iss, isPaid,        '|');
        getline(iss, paymentDate);

        while (index < this->rooms.size() && roomId != this->rooms[index]->GetId()) index++;
        if (index >= this->rooms.size()) break;

    try {
        MonthlyBill bill(Month(month), this->rooms[index].get(), stoull(rentCost), stoull(trashCost),
            stoull(wifiCost), stoull(serviceCost), stoull(damageCost),
            stoull(oldElecIndex), stoull(newElecIndex), stoull(electricCost),
            stoull(oldWaterIndex), stoull(newWaterIndex), stoull(waterCost),
            stoull(totalCost), (isPaid == "1"));
        if (paymentDate != "") bill.paymentDate = Date(paymentDate);
        this->rooms[index]->AddBill(bill);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
    catch (const string& e) {
        cout << e << endl;
    }
    }
}
    billInp.close();
    cout << "Tai du lieu hoa don thanh cong!\n";

    ifstream furnishingInp("data/" + furnishingFile);
    if (!furnishingInp) throw runtime_error("Khong the tai file du lieu tien ich!");
{
    int index = 0;
    string line, roomId, id, name, price;
    while (getline(furnishingInp, line)){
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        istringstream iss(line);
        getline(iss, roomId, '|');
        getline(iss, id, '|');
        getline(iss, name, '|');
        getline(iss, price);

        while (index < this->rooms.size() && roomId != this->rooms[index]->GetId()) index++;
        if (index >= this->rooms.size()) break;

    try {
        this->rooms[index]->AddFurnishing(Furnishing(id, name, stoull(price)));
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
    catch (const string& e) {
        cout << e << endl;
    }
    }
}
    furnishingInp.close();
    cout << "Tai du lieu tien ich thanh cong!\n";

    ifstream damageInp("data/" + damageFile);
    if (!damageInp) throw runtime_error("Khong the tai file du lieu bao hong!");
{
    int index = 0;
    string line, roomId, date, furnId, descrip, cost;
    while (getline(damageInp, line)){
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        istringstream iss(line);
        getline(iss, roomId, '|');
        getline(iss, date, '|');
        getline(iss, furnId, '|');
        getline(iss, descrip, '|');
        getline(iss, cost);

        while (index < this->rooms.size() && roomId != this->rooms[index]->GetId()) index++;
        if (index >= this->rooms.size()) break;

    try {
        const Furnishing* brokenOne = this->rooms[index]->GetFurnishing(furnId);
        if (brokenOne == nullptr) continue;
        this->rooms[index]->AddDamage(Damage(date, brokenOne, descrip, stoull(cost)));
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
    catch (const string& e) {
        cout << e << endl;
    }
    }
}
    damageInp.close();
    cout << "Tai du lieu bao hong thanh cong!\n";
}

void Nhatro::SaveRoomData(const std::string& roomFile, const std::string& billFile, const std::string& furnishingFile, const std::string& damageFile)
{
    this->SortRoomsById();
    ofstream roomOup("data/" + roomFile);
    if (!roomOup) throw runtime_error("Khong the luu file du lieu phong!");
    for (const auto& room : this->rooms){
        roomOup << room->GetId()        << "|"
                << (int) room->Type()   << "|"
                << room->GetName()      << "|"
                << room->GetCapacity()  << "|"
                << room->GetFloor()     << "|"
                << room->GetRentPrice() << "|"
                << room->GetPrepaid()   << "|"
                << room->GetOldElectricIndex() << "|"
                << room->GetNewElectricIndex() << "|"
                << room->GetOldWaterIndex()    << "|"
                << room->GetNewWaterIndex()    << "|"
                << room->GetDate()
                << endl;
    }
    roomOup.close();
    cout << "Luu du lieu phong thanh cong!\n";

    ofstream billOup("data/" + billFile);
    if (!billOup) throw runtime_error("Khong the luu file du lieu hoa don!");
    for (const auto& room : this->rooms){
        for (const auto& bill : room->GetMonthlyBills()){
            billOup << room->GetId()         << "|"
                    << bill.month            << "|"
                    << bill.rentCost         << "|"
                    << bill.trashCost        << "|"
                    << bill.wifiCost         << "|"
                    << bill.serviceCost      << "|"
                    << bill.electricCost     << "|"
                    << bill.waterCost        << "|"
                    << bill.damageCost       << "|"
                    << bill.oldElectricIndex << "|"
                    << bill.newElectricIndex << "|"
                    << bill.oldWaterIndex    << "|"
                    << bill.newWaterIndex    << "|"
                    << bill.totalCost        << "|"
                    << (bill.isPaid ? "1" : "0")  << "|";
            if (bill.isPaid) billOup << bill.paymentDate;
            billOup << endl;
        }
    }
    billOup.close();
    cout << "Luu du lieu hoa don thanh cong!\n";

    ofstream furnishingOup("data/" + furnishingFile);
    if (!furnishingOup) throw runtime_error("Khong the luu file du lieu tien ich!");
    for (const auto& room : this->rooms){
        for (const auto& furnishing : room->GetFurnishings()){
            furnishingOup << room->GetId()   << "|"
                          << furnishing.id   << "|"
                          << furnishing.name << "|"
                          << furnishing.price
                          << endl;
        }
    }
    furnishingOup.close();
    cout << "Luu du lieu tien ich thanh cong!\n";

    ofstream damageOup("data/" + damageFile);
    if (!damageOup) throw runtime_error("Khong the luu file du lieu thiet bi bi hu!");
    for (const auto& room : this->rooms){
        for (const auto& damage : room->GetDamages()){
            damageOup << room->GetId()          << "|"
                      << damage.date            << "|"
                      << damage.furnishing->id  << "|"
                      << damage.description     << "|"
                      << damage.compen
                      << endl;
        }
    }
    damageOup.close();
    cout << "Luu du lieu bao hong thanh cong!\n";
}

void Nhatro::LoadTenantData(const std::string& tenantFile)
{
    ifstream tenantInp("data/" + tenantFile);
    if (!tenantInp) throw runtime_error("Khong the tai file du lieu nguoi thue!");
    string line, id, name, phone, gend, cccd, home, tmpRsd, roomId, isLeader;
    while (getline(tenantInp, line)){
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        istringstream iss(line);
        getline(iss, id, '|');
        getline(iss, name, '|');
        getline(iss, gend, '|');
        getline(iss, cccd, '|');
        getline(iss, phone, '|');
        getline(iss, home, '|');
        getline(iss, roomId, '|');
        getline(iss, isLeader, '|');
        getline(iss, tmpRsd);

    try {
        unique_ptr<Tenant> newTenant = make_unique<Tenant>(id, name, static_cast<Gender>(std::stoi(gend)), phone, cccd, home);
        newTenant->SetTemporaryResidence(tmpRsd == "1");
        Tenant* tenant = newTenant.get();
        this->AddTenant(move(newTenant));
        if (!roomId.empty()){
            int roomIndex = this->FindRoom(roomId);
            if (roomIndex != -1){
                this->rooms[roomIndex]->AddTenant(tenant);
                tenant->SetRoom(this->rooms[roomIndex].get());
                
                if (isLeader == "1") this->rooms[roomIndex]->SetRoomLeader(tenant);
            }
        }
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
    catch (const string& e) {
        cout << e << endl;
    }
    }
    tenantInp.close();
    cout << "Tai du lieu khach thue thanh cong!\n";
}

void Nhatro::SaveTenantData(const std::string& tenantFile) const
{
    ofstream tenantOup("data/" + tenantFile);
    if (!tenantOup) throw runtime_error("Khong the luu file du lieu nguoi thue!");
    for (const auto& tenant : this->tenants){
        tenantOup << tenant->GetId()            << "|"
                  << tenant->GetName()          << "|"
                  << tenant->GetGender()        << "|"
                  << tenant->GetCCCD()          << "|"
                  << tenant->GetPhone()         << "|"
                  << tenant->GetHometown()      << "|"
                  << (tenant->GetRoom() != nullptr ? tenant->GetRoom()->GetId() : "") << "|"
                  << (tenant->IsLeader() ? (tenant.get() == tenant->GetRoom()->GetRoomLeader() ? "1" : "0") : "") << "|"
                  << (tenant->IsTemporaryResidence() ? "1" : "0") << "|";
        tenantOup << endl;
    }
    tenantOup.close();
    cout << "Luu du lieu khach thue thanh cong!\n";
}