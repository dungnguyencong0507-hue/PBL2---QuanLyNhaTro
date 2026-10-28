#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <header.h>
#include <BillManager.h>
#include <RoomManager.h>
#include <ConsoleUI.h>
using namespace std;

namespace {
const string BILL_DATA_FILE = "Bill.txt";
const string ROOM_DATA_FILE = "Room.txt";

bool ParseUnsigned(const string& value, ull& number)
{
    if (value.empty()) return false;
    for (unsigned char c : value)
        if (c < '0' || c > '9') return false;

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

vector<string> Split(const string& line, char delimiter)
{
    vector<string> values;
    string value;
    istringstream input(line);
    while (getline(input, value, delimiter))
        values.push_back(value);
    if (!line.empty() && line.back() == delimiter)
        values.push_back("");
    return values;
}

void Pause()
{
    cout << "\nNhan mot phim bat ky de tiep tuc...";
    char c;
    Console::Read(c);
}

bool IsValidMonth(int month, int year)
{
    return month >= 1 && month <= 12 && year >= 1;
}

int DaysInMonth(int month, int year)
{
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
        return 29;
    return days[month - 1];
}

bool IsValidDate(int day, int month, int year)
{
    return IsValidMonth(month, year) && day >= 1 && day <= DaysInMonth(month, year);
}

void ShowInvoice(const Room& room, const MonthlyBill& bill)
{
    cout << "Phong " << room.GetName() << " (" << room.GetId() << ")\n"
        << bill;
}
}

void LoadInvoiceData(string& filename, Nhatro& nhatro)
{
    ifstream inp("data/" + filename);
    if (!inp) throw runtime_error("Khong the tai file du lieu hoa don!");

    string line;
    int lineNumber = 0;
    while (getline(inp, line)){
        lineNumber++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        vector<string> values = Split(line, '|');
        ull electricPrice, waterPrice, servicePrice, internetPrice, trashPrice;
        if (!values.empty() && values[0] == "PRICE"){
            if (values.size() != 6
                || !ParseUnsigned(values[1], electricPrice)
                || !ParseUnsigned(values[2], waterPrice)
                || !ParseUnsigned(values[3], servicePrice)
                || !ParseUnsigned(values[4], internetPrice)
                || !ParseUnsigned(values[5], trashPrice))
                throw runtime_error("Bang gia khong hop le tai dong "
                    + to_string(lineNumber) + ".");

            Room::SetElectricUnitPrice(electricPrice);
            Room::SetWaterUnitPrice(waterPrice);
            Room::SetServiceCost(servicePrice);
            Room::SetInternetCost(internetPrice);
            Room::SetTrashCost(trashPrice);
            continue;
        }

        ull monthValue, yearValue, paidValue, dayValue, paidMonthValue, paidYearValue;
        ull rentCost, serviceCost, damageCost, oldElectric, newElectric, electricCost;
        ull oldWater, newWater, waterCost, totalCost;
        if (values.size() != 18 || values[0] != "BILL"
            || values[1].empty()
            || !ParseUnsigned(values[2], monthValue) || monthValue < 1 || monthValue > 12
            || !ParseUnsigned(values[3], yearValue) || yearValue == 0
            || yearValue > static_cast<ull>(numeric_limits<int>::max())
            || !ParseUnsigned(values[4], paidValue) || paidValue > 1
            || !ParseUnsigned(values[5], dayValue)
            || !ParseUnsigned(values[6], paidMonthValue)
            || !ParseUnsigned(values[7], paidYearValue)
            || !ParseUnsigned(values[8], rentCost)
            || !ParseUnsigned(values[9], serviceCost)
            || !ParseUnsigned(values[10], damageCost)
            || !ParseUnsigned(values[11], oldElectric)
            || !ParseUnsigned(values[12], newElectric)
            || !ParseUnsigned(values[13], electricCost)
            || !ParseUnsigned(values[14], oldWater)
            || !ParseUnsigned(values[15], newWater)
            || !ParseUnsigned(values[16], waterCost)
            || !ParseUnsigned(values[17], totalCost))
            throw runtime_error("Hoa don khong hop le tai dong "
                + to_string(lineNumber) + ".");

        Room* room = nhatro.FindRoom(values[1]);
        if (!room)
            throw runtime_error("Khong tim thay phong " + values[1]
                + " tai dong " + to_string(lineNumber) + ".");

        MonthlyBill bill{};
        bill.month = Month{static_cast<int>(monthValue), static_cast<int>(yearValue)};
        bill.isPaid = paidValue == 1;
        bill.rentCost = rentCost;
        bill.serviceCost = serviceCost;
        bill.damageCost = damageCost;
        bill.oldElectricIndex = oldElectric;
        bill.newElectricIndex = newElectric;
        bill.electricCost = electricCost;
        bill.oldWaterIndex = oldWater;
        bill.newWaterIndex = newWater;
        bill.waterCost = waterCost;
        bill.totalCost = totalCost;

        if (bill.isPaid){
            if (dayValue > static_cast<ull>(numeric_limits<int>::max())
                || paidMonthValue > static_cast<ull>(numeric_limits<int>::max())
                || paidYearValue > static_cast<ull>(numeric_limits<int>::max())
                || !IsValidDate(static_cast<int>(dayValue),
                    static_cast<int>(paidMonthValue), static_cast<int>(paidYearValue)))
                throw runtime_error("Ngay thanh toan khong hop le tai dong "
                    + to_string(lineNumber) + ".");
            bill.paymentDate.day = static_cast<int>(dayValue);
            bill.paymentDate.month = static_cast<int>(paidMonthValue);
            bill.paymentDate.year = static_cast<int>(paidYearValue);
        }

        if (!room->AddBill(bill))
            throw runtime_error("Hoa don bi trung tai dong " + to_string(lineNumber) + ".");
    }
}

void SaveInvoiceData(string& filename, Nhatro& nhatro)
{
    ofstream oup("data/" + filename);
    if (!oup) throw runtime_error("Khong the luu file du lieu hoa don!");

    oup << "PRICE|" << Room::electricUnitPrice << "|"
        << Room::waterUnitPrice << "|"
        << Room::serviceCost << "|"
        << Room::internetCost << "|"
        << Room::trashCost << '\n';

    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        for (const MonthlyBill& bill : room->GetMonthlyBills()){
            oup << "BILL|" << room->GetId() << "|"
                << bill.month.month << "|" << bill.month.year << "|"
                << (bill.isPaid ? 1 : 0) << "|"
                << bill.paymentDate.day << "|"
                << bill.paymentDate.month << "|"
                << bill.paymentDate.year << "|"
                << bill.rentCost << "|"
                << bill.serviceCost << "|"
                << bill.damageCost << "|"
                << bill.oldElectricIndex << "|"
                << bill.newElectricIndex << "|"
                << bill.electricCost << "|"
                << bill.oldWaterIndex << "|"
                << bill.newWaterIndex << "|"
                << bill.waterCost << "|"
                << bill.totalCost << '\n';
        }
    }
    if (!oup) throw runtime_error("Loi khi ghi file du lieu hoa don!");
}

void ManageBills(Nhatro& nhatro)
{
    MenuBox menu(2, 1, 42, "QUAN LY HOA DON");
    menu.AddOption("Tao hoa don thang moi");
    menu.AddOption("Cap nhat trang thai thanh toan");
    menu.AddOption("Cap nhat gia dich vu chung");
    menu.AddOption("Cap nhat chi so dien, nuoc");
    menu.AddOption("Bao cao tai chinh");
    menu.AddOption("Tra cuu lich su hoa don phong");

    int choice;
    while ((choice = menu.Exec()) != -1){
        switch (choice){
            case 0: CreateBill(nhatro); break;
            case 1: UpdateBillStatus(nhatro); break;
            case 2: UpdateGeneralPrice(nhatro); break;
            case 3: UpdateUtilityIndices(nhatro); break;
            case 4: ShowFinanceReport(nhatro); break;
            case 5: ShowInvoiceHistory(nhatro); break;
        }
    }
}

void CreateBill(Nhatro& nhatro)
{
    FormBox form(2, 1, 60, "TAO HOA DON HANG LOAT");
    form.AddField("Thang", "");
    form.AddField("Nam", "");
    if (!form.Exec()) return;

    int month, year;
    const vector<Field>& fields = form.GetFields();
    if (!ParsePositiveInt(fields[0].value, month)
        || !ParsePositiveInt(fields[1].value, year)
        || !IsValidMonth(month, year)){
        cout << "Thang hoac nam khong hop le.\n";
        Pause();
        return;
    }

    Month billingMonth{month, year};
    int created = 0, emptyRooms = 0, existingBills = 0;
    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        if (room->GetTenantCount() == 0){
            emptyRooms++;
            continue;
        }
        if (room->GetBill(billingMonth)){
            existingBills++;
            continue;
        }
        if (room->CreateBill(billingMonth)) created++;
    }

    if (created > 0){
        string filename = BILL_DATA_FILE;
        SaveInvoiceData(filename, nhatro);
    }
    cout << "Da tao " << created << " hoa don cho thang "
        << month << "/" << year << ".\n"
        << "Bo qua " << emptyRooms << " phong trong va "
        << existingBills << " hoa don da ton tai.\n";
    Pause();
}

void UpdateBillStatus(Nhatro& nhatro)
{
    FormBox form(2, 1, 70, "CAP NHAT TRANG THAI HOA DON");
    form.AddField("Ma phong", "");
    form.AddField("Thang hoa don", "");
    form.AddField("Nam hoa don", "");
    form.AddField("Trang thai (1 Da tra, 0 Chua tra)", "1");
    if (!form.Exec()) return;

    const vector<Field>& fields = form.GetFields();
    int month, year;
    ull paid;
    if (!ParsePositiveInt(fields[1].value, month)
        || !ParsePositiveInt(fields[2].value, year)
        || !IsValidMonth(month, year)
        || !ParseUnsigned(fields[3].value, paid) || paid > 1){
        cout << "Thong tin hoa don khong hop le.\n";
        Pause();
        return;
    }

    Room* room = nhatro.FindRoom(fields[0].value);
    if (!room){
        cout << "Khong tim thay phong.\n";
        Pause();
        return;
    }

    Month invoiceMonth{month, year};
    MonthlyBill* bill = room->GetBill(invoiceMonth);
    if (!bill){
        cout << "Phong khong co hoa don trong thang nay.\n";
        Pause();
        return;
    }

    if (paid == 1){
        FormBox dateForm(2, 1, 60, "NGAY THANH TOAN");
        dateForm.AddField("Ngay", "");
        dateForm.AddField("Thang", "");
        dateForm.AddField("Nam", "");
        if (!dateForm.Exec()) return;

        const vector<Field>& dateFields = dateForm.GetFields();
        int day, paymentMonth, paymentYear;
        if (!ParsePositiveInt(dateFields[0].value, day)
            || !ParsePositiveInt(dateFields[1].value, paymentMonth)
            || !ParsePositiveInt(dateFields[2].value, paymentYear)
            || !IsValidDate(day, paymentMonth, paymentYear)){
            cout << "Ngay thanh toan khong hop le.\n";
            Pause();
            return;
        }
        bill->isPaid = true;
        bill->paymentDate.day = day;
        bill->paymentDate.month = paymentMonth;
        bill->paymentDate.year = paymentYear;
    }
    else{
        bill->isPaid = false;
        bill->paymentDate = Date{};
    }

    string filename = BILL_DATA_FILE;
    SaveInvoiceData(filename, nhatro);
    cout << "Da cap nhat trang thai hoa don.\n";
    Pause();
}

void UpdateGeneralPrice(Nhatro& nhatro)
{
    FormBox form(2, 1, 78, "CAP NHAT GIA DICH VU CHUNG");
    form.AddField("Gia dien / kWh", to_string(Room::electricUnitPrice));
    form.AddField("Gia nuoc / m3", to_string(Room::waterUnitPrice));
    form.AddField("Dich vu / nguoi", to_string(Room::serviceCost));
    form.AddField("Internet / phong", to_string(Room::internetCost));
    form.AddField("Rac / nguoi", to_string(Room::trashCost));
    if (!form.Exec()) return;

    const vector<Field>& fields = form.GetFields();
    ull electricPrice, waterPrice, servicePrice, internetPrice, trashPrice;
    if (!ParseUnsigned(fields[0].value, electricPrice)
        || !ParseUnsigned(fields[1].value, waterPrice)
        || !ParseUnsigned(fields[2].value, servicePrice)
        || !ParseUnsigned(fields[3].value, internetPrice)
        || !ParseUnsigned(fields[4].value, trashPrice)){
        cout << "Gia tien khong hop le. Bang gia chua thay doi.\n";
        Pause();
        return;
    }

    Room::SetElectricUnitPrice(electricPrice);
    Room::SetWaterUnitPrice(waterPrice);
    Room::SetServiceCost(servicePrice);
    Room::SetInternetCost(internetPrice);
    Room::SetTrashCost(trashPrice);

    string filename = BILL_DATA_FILE;
    SaveInvoiceData(filename, nhatro);
    cout << "Da cap nhat bang gia chung.\n";
    Pause();
}

void UpdateUtilityIndices(Nhatro& nhatro)
{
    FormBox form(2, 1, 70, "CAP NHAT CHI SO DIEN NUOC");
    form.AddField("Ma phong", "");
    form.AddField("Chi so dien moi", "");
    form.AddField("Chi so nuoc moi", "");
    if (!form.Exec()) return;

    const vector<Field>& fields = form.GetFields();
    ull electricIndex, waterIndex;
    if (!ParseUnsigned(fields[1].value, electricIndex)
        || !ParseUnsigned(fields[2].value, waterIndex)){
        cout << "Chi so dien nuoc phai la so khong am.\n";
        Pause();
        return;
    }

    Room* room = nhatro.FindRoom(fields[0].value);
    if (!room){
        cout << "Khong tim thay phong.\n";
        Pause();
        return;
    }
    if (electricIndex < room->GetNewElectricIndex()
        || waterIndex < room->GetNewWaterIndex()){
        cout << "Chi so moi khong duoc nho hon chi so hien tai.\n";
        Pause();
        return;
    }

    room->SetElectricIndex(electricIndex);
    room->SetWaterIndex(waterIndex);
    string roomFilename = ROOM_DATA_FILE;
    SaveRoomData(roomFilename, nhatro);
    cout << "Da cap nhat chi so dien va nuoc.\n";
    Pause();
}

void ShowFinanceReport(Nhatro& nhatro)
{
    ull revenue = 0;
    ull debt = 0;
    int paidCount = 0, unpaidCount = 0;
    for (int i = 0; i < nhatro.GetNumRoom(); i++){
        Room* room = nhatro.GetRoom(i);
        for (const MonthlyBill& bill : room->GetMonthlyBills()){
            if (bill.isPaid){
                revenue += bill.totalCost;
                paidCount++;
            }
            else{
                debt += bill.totalCost;
                unpaidCount++;
            }
        }
    }

    cout << "\n========== BAO CAO TAI CHINH ==========\n"
        << "Tong doanh thu da thu: " << revenue << " VND\n"
        << "Tong du no chua thu: " << debt << " VND\n"
        << "So hoa don da thanh toan: " << paidCount << '\n'
        << "So hoa don chua thanh toan: " << unpaidCount << '\n'
        << "Tong cong: " << revenue + debt << " VND\n"
        << "=======================================\n";
    Pause();
}

void ShowInvoiceHistory(Nhatro& nhatro)
{
    FormBox form(2, 1, 60, "TRA CUU LICH SU HOA DON");
    form.AddField("Ma phong", "");
    if (!form.Exec()) return;

    Room* room = nhatro.FindRoom(form.GetFields()[0].value);
    if (!room){
        cout << "Khong tim thay phong.\n";
        Pause();
        return;
    }

    cout << "\nLich su hoa don phong " << room->GetName()
        << " (" << room->GetId() << "):\n";
    if (room->GetMonthlyBills().empty()) cout << "Phong chua co hoa don.\n";
    cout << endl;
    for (const MonthlyBill& bill : room->GetMonthlyBills())
        ShowInvoice(*room, bill);
    Pause();
}
