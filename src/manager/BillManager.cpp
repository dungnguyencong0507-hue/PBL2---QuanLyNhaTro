#include <BillManager.h>
using namespace std;

void ManageBills(Nhatro& nhatro)
{
    system("cls");
    int padding = 2;    

    NavigationBox navBox(40, padding);
    navBox.AddItem("QUAN LY HOA DON THEO THANG");
    navBox.AddItem("QUAN LY HOA DON THEO NAM");
    navBox.AddItem("NHAP CHI SO DIEN-NUOC");
    navBox.AddItem("QUAN LY BANG GIA CHUNG");
    
    enum Action {
        NONE,
        NAVIGATION,
        FUNCTION,
        MONTH_BILL,
        YEAR_BILL,
        INDEX,
        GENERAL
    };
    Action selection = NAVIGATION;

    InputHandler<Action> btnLink(NONE, GENERAL);
    btnLink.AddRelation(MONTH_BILL, Console::UP, NAVIGATION);
    btnLink.AddRelation(YEAR_BILL, Console::UP, NAVIGATION);
    btnLink.AddRelation(INDEX, Console::UP, NAVIGATION);
    btnLink.AddRelation(GENERAL, Console::UP, NAVIGATION);
    btnLink.AddRelation(NAVIGATION, Console::DOWN, FUNCTION);

    int page = 0, currPage = -1;
    Console::Key key = Console::OTHER;
    
    while (true) {
    try {
        if (page != currPage) {
            switch (page)
            {
            case 0:
                ManageBillsInMonth(nhatro, navBox, key);
                break;
            case 1:
                ManageBillsInYear(nhatro, navBox, key);
                break;
            case 2:
                ManageIndex(nhatro, navBox, key);
                break;
            case 3:
                UpdateGeneralPrice(navBox, key);
                break;
            default:
                break;
            }
        }

        if (selection != NAVIGATION) {
            switch (selection)
            {
            case MONTH_BILL:
                ManageBillsInMonth(nhatro, navBox, key);
                break;
            case YEAR_BILL:
                ManageBillsInYear(nhatro, navBox, key);
                break;
            case INDEX:
                ManageIndex(nhatro, navBox, key);
                break;
            case GENERAL:
                UpdateGeneralPrice(navBox, key);
                break;
            default:
                break;
            }
        }

        if(key == Console::ESC) return;

        currPage = page;
        if (selection == NAVIGATION) page = navBox(key);

        selection = btnLink.GetDest(selection, key);
        if (selection == FUNCTION) {
            switch (page)
            {
            case 0:
                selection = MONTH_BILL;
                break;
            case 1:
                selection = YEAR_BILL;
                break;
            case 2:
                selection = INDEX;
                break;
            case 3:
                selection = GENERAL;
                break;
            default:
                break;
            }
        }
    }
    catch (const exception& e) {
        Console::Goto(padding, padding);
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
    }
    catch (const string& e) {
        Console::Goto(padding, padding);
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
    }
    }
}

void ManageIndex(Nhatro& nhatro, NavigationBox& navBox, Console::Key& key)
{
    // NHAP MOI, CHINH SUA, CHOT SO
    int toolBarW = 32;
    int padding = 2;    
    
    const auto& rooms = nhatro.GetRooms();
    int startX = navBox.GetX(), startY = navBox.GetY() + navBox.GetH();
    
    ButtonBox updateBtn(startX, navBox.GetY() + navBox.GetH(), toolBarW, "NHAP VA CHINH SUA");

    List<Room*> roomTable;
    for (const auto& room : rooms) roomTable.push_back(room.get());
    TableBox<Room*> dataBox(padding*2 + toolBarW, startY + 2, 
    IndexTableWidth(), roomTable, IndexTableInit, IndexTableEnd, IndexTableShow);
    
    enum Action {
        NONE,
        NAVIGATION,
        UPDATE
    };
    Action selection = NAVIGATION;

    InputHandler<Action> btnLink(NONE, UPDATE);
    btnLink.AddRelation(NAVIGATION, Console::DOWN, UPDATE);
    btnLink.AddRelation(UPDATE, Console::UP, NAVIGATION);

    bool redraw = 1;
    selection = btnLink.GetDest(selection, key);
    
    while (true) {
    try {
        if (redraw) {
            system("cls");
            navBox.Draw();
            updateBtn.Draw();
            dataBox.Draw();
            redraw = false;
        }

        if (selection == UPDATE) {
            bool result = updateBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    UpdateIndex(roomTable[index]);
                    redraw = true;
                }
                key = Console::OTHER;
            }
        } 

        if (key == Console::ESC) break;
        selection = btnLink.GetDest(selection, key);
        
        if (selection == NAVIGATION) break;
    }
    catch (const exception& e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
        redraw = true;
    }
    catch (const char* e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
        redraw = true;
    }
    }
    key = Console::UP;
}
void UpdateIndex(Room* room)
{
    system("cls");
    int padding =3;
    FormBox indexBox(3, 3, 70, 0.5, "THONG TIN CHI SO DIEN NUOC");
    if (room->IsElectricEditable()) 
        indexBox.AddField("Chinh sua chi so dien", to_string(room->GetNewElectricIndex()),
        to_string(room->GetOldElectricIndex()) + " -> ", " kWh");
    else indexBox.AddField("Nhap chi so dien moi", "",
        to_string(room->GetOldElectricIndex()) + " -> " + to_string(room->GetNewElectricIndex()) + " -> ", " kWh");

    if (room->IsWaterEditable()) 
        indexBox.AddField("Chinh sua chi so nuoc", to_string(room->GetNewWaterIndex()),
        to_string(room->GetOldWaterIndex()) + " -> ", " kWh");
    else indexBox.AddField("Nhap chi so nuoc moi", "",
        to_string(room->GetOldWaterIndex()) + " -> " + to_string(room->GetNewWaterIndex()) + " -> ", " m3");

    indexBox.Draw();

    while(true) {
    while (true) {
    try {
        Console::Key key;
        bool confirm = indexBox(key);
        if (key == Console::ESC) return;
        if (confirm) {
            if (room->IsElectricEditable()) {
                room->EditElectricIndex(SToULL(indexBox.GetValue(0)));
            } else room->SetElectricIndex(SToULL(indexBox.GetValue(0)));

            if (room->IsWaterEditable()) {
                room->EditWaterIndex(SToULL(indexBox.GetValue(1)));
            } else room->SetWaterIndex(SToULL(indexBox.GetValue(1)));

            return;
        }
    }
    catch (const exception& e) {
        Console::ClearSegment(padding, indexBox.GetY() + indexBox.GetH(), 1, 100);
        Console::Goto(padding, indexBox.GetY() + indexBox.GetH());
        Console::SetColor(Console::RED);
        cout << e.what();
    }
    catch (const char* e) {
        Console::ClearSegment(padding, indexBox.GetY() + indexBox.GetH(), 1, 100);
        Console::Goto(padding, indexBox.GetY() + indexBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (const string& e) {
        Console::ClearSegment(padding, indexBox.GetY() + indexBox.GetH(), 1, 100);
        Console::Goto(padding, indexBox.GetY() + indexBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (...) {
        Console::ClearSegment(padding, indexBox.GetY() + indexBox.GetH(), 1, 100);
        Console::Goto(padding, indexBox.GetY() + indexBox.GetH());
        Console::SetColor(Console::RED);
        cout << "Loi khong xac dinh";
    }
    }
    }
}
void UpdateGeneralPrice(NavigationBox& navBox, Console::Key& key)
{
    enum Action {
        NONE,
        NAVIGATION,
        PRICE
    };
    Action selection = NAVIGATION;
    InputHandler<Action> btnLink(NONE, PRICE);
    btnLink.AddRelation(NAVIGATION, Console::DOWN, PRICE);

    int padding = navBox.GetX();
    FormBox priceBox(navBox.GetX(), navBox.GetY() + navBox.GetH() + 1, 70, 0.6, "BANG GIA CHUNG");
    priceBox.AddField("Don gia dien", to_string(Room::electricUnitPrice), "", " VND/kWh");
    priceBox.AddField("Don gia nuoc", to_string(Room::waterUnitPrice), "", " VND/m3");
    priceBox.AddField("Gia cuoc Internet", to_string(Room::internetCost), "", " VND/phong");
    priceBox.AddField("Don gia dich vu", to_string(Room::serviceCost), "", " VND/nguoi");
    priceBox.AddField("Phi rac thai", to_string(Room::trashCost), "", " VND/nguoi");
    priceBox.AddField("Gia thue chung phong CO BAN", to_string(BasicRoom::stdRentPrice), "", " VND");
    priceBox.AddField("Gia thue chung phong VIP", to_string(VipRoom::stdRentPrice), "", " VND");
    priceBox.AddField("Gia thue chung phong STUDIO", to_string(StudioRoom::stdRentPrice), "", " VND");
    system("cls");
    navBox.Draw();
    priceBox.Draw();

    selection = btnLink.GetDest(selection, key);
    ull uElec, uWtr, inte, serv, trash, basic, vip, studio;
    while (selection == PRICE) {
    try {
        bool confirm = priceBox(key);
        if (key == Console::ESC) return;
        if (confirm) {
            uElec = SToULL(priceBox.GetValue(0));
            uWtr = SToULL(priceBox.GetValue(1));
            inte = SToULL(priceBox.GetValue(2));
            serv = SToULL(priceBox.GetValue(3));
            trash = SToULL(priceBox.GetValue(4));
            basic = SToULL(priceBox.GetValue(5));
            vip = SToULL(priceBox.GetValue(6));
            studio = SToULL(priceBox.GetValue(7));

            Room::electricUnitPrice = uElec;
            Room::waterUnitPrice = uWtr;
            Room::internetCost = inte;
            Room::serviceCost = serv;
            Room::trashCost = trash;
            BasicRoom::stdRentPrice = basic;
            VipRoom::stdRentPrice = vip;
            StudioRoom::stdRentPrice = studio;

            Console::SetColor(Console::WHITE);
            Console::ClearSegment(padding, priceBox.GetY() + priceBox.GetH(), 2, 100);
            Console::Goto(padding, priceBox.GetY() + priceBox.GetH());
            cout << "Cap nhat bang gia thanh cong ";
            system("pause");
            break;
        }
    }
    catch (const exception& e) {
        Console::ClearSegment(padding, priceBox.GetY() + priceBox.GetH(), 1, 100);
        Console::Goto(padding, priceBox.GetY() + priceBox.GetH());
        Console::SetColor(Console::RED);
        cout << e.what();
    }
    catch (const char* e) {
        Console::ClearSegment(padding, priceBox.GetY() + priceBox.GetH(), 1, 100);
        Console::Goto(padding, priceBox.GetY() + priceBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (const string& e) {
        Console::ClearSegment(padding, priceBox.GetY() + priceBox.GetH(), 1, 100);
        Console::Goto(padding, priceBox.GetY() + priceBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (...) {
        Console::ClearSegment(padding, priceBox.GetY() + priceBox.GetH(), 1, 100);
        Console::Goto(padding, priceBox.GetY() + priceBox.GetH());
        Console::SetColor(Console::RED);
        cout << "Loi khong xac dinh";
    }
    }
    key = Console::UP;
}

void ManageBillsInMonth(Nhatro& nhatro, NavigationBox& navBox, Console::Key& key)
{
    int toolBarW = 32;
    int searchBoxW = 50;
    int padding = 2;    
    
    const auto& rooms = nhatro.GetRooms();
    int startX = navBox.GetX(), startY = navBox.GetY() + navBox.GetH();
    
    NumberMenuBox filterBox(startX, startY, toolBarW, "LOC THONG TIN HOA DON");
    filterBox.AddOption("Tat ca hoa don");
    filterBox.AddOption("Hoa don da thanh toan");
    filterBox.AddOption("Hoa don chua thanh toan");
    filterBox.AddOption("Phong co hoa don");
    filterBox.AddOption("Phong khong co hoa don");

    NumberMenuBox sortBox(startX, filterBox.GetY() + filterBox.GetH(), toolBarW, "SAP XEP HOA DON");
    sortBox.AddOption("Theo ma phong");
    sortBox.AddOption("Ngay thanh toan moi nhat");
    sortBox.AddOption("Tong hoa don tang dan");
    sortBox.AddOption("Tong hoa don giam dan");

    ButtonBox addBtn(startX, sortBox.GetY() + sortBox.GetH(), toolBarW, "TAO HOA DON");
    ButtonBox addAllBtn(startX, addBtn.GetY() + addBtn.GetH(), toolBarW, "TAO TOAN BO HOA DON");
    ButtonBox detailBtn(startX, addAllBtn.GetY() + addAllBtn.GetH(), toolBarW, "XEM VA CAP NHAT HOA DON");
    ButtonBox historyBtn(startX, detailBtn.GetY() + detailBtn.GetH(), toolBarW, "XEM LICH SU HOA DON");
    ButtonBox deleteBtn(startX, detailBtn.GetY() + detailBtn.GetH(), toolBarW, "XOA HOA DON");
    string searchValue = "";
    SingleLineBox searchBox(padding*2 + toolBarW, startY, searchBoxW, "TIM KIEM", searchValue);

    List<BillInMonth> sBillsInMonth;
    List<BillInMonth> fBillsInMonth;
    TableBox<BillInMonth> dataBox(padding*2 + toolBarW, searchBox.GetY() + searchBox.GetH() + 2, 
    BillInMonthTableWidth(), fBillsInMonth, BillInMonthTableInit, BillInMonthTableEnd, BillInMonthTableShow);
    
    Month month = Nhatro::today;
    string monStr = to_string(month.month);
    string yearStr = to_string(month.year);
    SingleLineBox monBox(searchBox.GetX() + searchBox.GetW(), startY, 15, "Thang", monStr);
    SingleLineBox yearBox(monBox.GetX() + monBox.GetW(), startY, 15, "Nam", yearStr);

    enum Action {
        NONE,
        NAVIGATION,
        FILTER,
        SORT,
        ADD,
        ADDALL,
        DETAIL,
        HISTORY,
        REMOVE,
        MONTH,
        YEAR,
        SEARCH,
    };
    Action selection = NAVIGATION;

    InputHandler<Action> btnLink(NONE, SEARCH);
    btnLink.AddRelation(NAVIGATION, Console::DOWN, FILTER);
    btnLink.AddRelation(FILTER, Console::UP, NAVIGATION);
    btnLink.AddRelation(SEARCH, Console::UP, NAVIGATION);
    btnLink.AddRelation(MONTH, Console::UP, NAVIGATION);
    btnLink.AddRelation(YEAR, Console::UP, NAVIGATION);
    btnLink.AddRelation(SORT, Console::UP, FILTER);
    btnLink.AddRelation(ADD, Console::UP, SORT);
    btnLink.AddRelation(ADDALL, Console::UP, ADD);
    btnLink.AddRelation(DETAIL, Console::UP, ADDALL);
    btnLink.AddRelation(HISTORY, Console::UP, DETAIL);
    btnLink.AddRelation(REMOVE, Console::UP, HISTORY);
    btnLink.AddRelation(FILTER, Console::DOWN, SORT);
    btnLink.AddRelation(SORT, Console::DOWN, ADD);
    btnLink.AddRelation(ADD, Console::DOWN, ADDALL);
    btnLink.AddRelation(ADDALL, Console::DOWN, DETAIL);
    btnLink.AddRelation(DETAIL, Console::DOWN, HISTORY);
    btnLink.AddRelation(HISTORY, Console::DOWN, REMOVE);
    btnLink.AddRelation(FILTER, Console::RIGHT, SEARCH);
    btnLink.AddRelation(SORT, Console::RIGHT, SEARCH);
    btnLink.AddRelation(ADD, Console::RIGHT, SEARCH);
    btnLink.AddRelation(ADDALL, Console::RIGHT, SEARCH);
    btnLink.AddRelation(HISTORY, Console::RIGHT, SEARCH);
    btnLink.AddRelation(DETAIL, Console::RIGHT, SEARCH);
    btnLink.AddRelation(REMOVE, Console::RIGHT, SEARCH);
    btnLink.AddRelation(SEARCH, Console::RIGHT, MONTH);
    btnLink.AddRelation(MONTH, Console::RIGHT, YEAR);
    btnLink.AddRelation(YEAR, Console::LEFT, MONTH);
    btnLink.AddRelation(MONTH, Console::LEFT, SEARCH);
    btnLink.AddRelation(SEARCH, Console::LEFT, FILTER);

    bool redraw = 1;
    bool reload = redraw;
    int filterChoice = 0;
    int sortChoice = 0;
    selection = btnLink.GetDest(selection, key);
    
    while (true) {
    try {
        if (reload) {

            sBillsInMonth = GetBills(month, rooms);
            SortBills(sortChoice, sBillsInMonth);
            fBillsInMonth = FilterBills(filterChoice, sBillsInMonth);
            dataBox.Draw();
            reload = false;
        }
        if (redraw) {
            system("cls");
            navBox.Draw();
            searchBox.Draw();
            monBox.Draw();
            yearBox.Draw();
            filterBox.Draw();
            sortBox.Draw();
            addBtn.Draw();
            addAllBtn.Draw();
            deleteBtn.Draw();
            detailBtn.Draw();
            historyBtn.Draw();
            dataBox.Draw();
            redraw = false;
        }

        if (selection == FILTER) {
            int choice = filterBox(key);
            if (choice != -1) {
                fBillsInMonth = FilterBills(choice, fBillsInMonth);
                dataBox.Draw();
                filterChoice = choice;
            }
        }
        if (selection == SORT) {
            int choice = sortBox(key);
            if (choice != -1) {
                SortBills(choice, fBillsInMonth);
                fBillsInMonth = FilterBills(filterChoice, fBillsInMonth);
                dataBox.Draw();
                sortChoice = choice;
            } 
        }
        if (selection == ADD) {
            bool result = addBtn(key);
            if (result) {
                int choice = dataBox(key);
                if (choice != -1) {
                    if (fBillsInMonth[choice].bill == nullptr) throw runtime_error("Thang nay da co hoa don");
                    AddBill(fBillsInMonth[choice].room, month);
                }
                key = Console::OTHER;
            }
        }
        if (selection == ADDALL) {
            bool result = addAllBtn(key);
            if (result) {
                CreateBills(nhatro, month);
                reload = true;
            }
        }
        if (selection == DETAIL) {
            bool result = detailBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    UpdateBill(fBillsInMonth[index].bill);
                    redraw = true;
                }
                key = Console::OTHER;
            }
        } 
        if (selection == HISTORY) {
            bool result = historyBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    UpdateBill(fBillsInMonth[index].room);
                    redraw = true;
                }
                key = Console::OTHER;
            }
        }
        if (selection == REMOVE) {
            bool result = deleteBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    fBillsInMonth[index].room->RemoveBill(month);
                    reload = true;
                }
                key = Console::OTHER;
            }
        }
        if (selection == SEARCH) {
            searchBox(key);
            if (key == Console::ENTER) {
                fBillsInMonth = SearchBills(searchValue, fBillsInMonth);
                dataBox.Draw();
            }
        }
        if (selection == MONTH) {
            monBox(key);
            if (key == Console::ENTER) {
                month = SToMonth(monStr + "/" + yearStr);
                reload = true;
            }
        }
        if (selection == YEAR) {
            yearBox(key);
            if (key == Console::ENTER) {
                month = SToMonth(monStr + "/" + yearStr);
                reload = true;
            }
        }

        if (key == Console::ESC) break;
        selection = btnLink.GetDest(selection, key);
        
        if (selection == NAVIGATION) return;
    }
    catch (const exception& e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
        redraw = true;
    }
    catch (const char* e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
        redraw = true;
    }
    }
}

void ManageBillsInYear(Nhatro& nhatro, NavigationBox& navBox, Console::Key& key)
{
    int toolBarW = 32;
    int searchBoxW = 50;
    int padding = 2;    
    
    const auto& rooms = nhatro.GetRooms();
    int startX = navBox.GetX(), startY = navBox.GetY() + navBox.GetH();
    
    NumberMenuBox filterBox(startX, startY, toolBarW, "LOC THONG TIN PHONG");
    filterBox.AddOption("Tat ca phong");
    filterBox.AddOption("Phong con no tien");
    filterBox.AddOption("Phong khong no tien");
    filterBox.AddOption("Tien no vuot qua tien coc");
    NumberMenuBox sortBox(startX, filterBox.GetY() + filterBox.GetH(), toolBarW, "SAP XEP PHONG");
    sortBox.AddOption("Theo ma phong");
    sortBox.AddOption("Tong doanh thu tang dan");
    sortBox.AddOption("Tong doanh thu giam dan");
    sortBox.AddOption("Tong no tang dan");
    sortBox.AddOption("Tong no giam dan");
    ButtonBox detailBtn(startX, sortBox.GetY() + sortBox.GetH(), toolBarW, "XEM CHI TIET LICH SU HOA DON");
    string searchValue = "";
    SingleLineBox searchBox(padding*2 + toolBarW, startY, searchBoxW, "TIM KIEM", searchValue);
    List<BillInYear> sBillsInYear;
    List<BillInYear> fBillsInYear;
    TableBox<BillInYear> dataBox(padding*2 + toolBarW, searchBox.GetY() + searchBox.GetH() + 2, 
    BillInYearTableWidth(), fBillsInYear, BillInYearTableInit, BillInYearTableEnd, BillInYearTableShow);

    int year = Nhatro::today.year;
    string yearStr = to_string(year);
    SingleLineBox yearBox(searchBox.GetX() + searchBox.GetW(), startY, 15, "Nam", yearStr);
    enum Action {
        NONE,
        NAVIGATION,
        FILTER,
        SORT,
        DETAIL,
        YEAR,
        SEARCH,
    };
    Action selection = NAVIGATION;

    InputHandler<Action> btnLink(NONE, SEARCH);
    btnLink.AddRelation(NAVIGATION, Console::DOWN, FILTER);
    btnLink.AddRelation(FILTER, Console::UP, NAVIGATION);
    btnLink.AddRelation(SEARCH, Console::UP, NAVIGATION);
    btnLink.AddRelation(YEAR, Console::UP, NAVIGATION);
    btnLink.AddRelation(SORT, Console::UP, FILTER);
    btnLink.AddRelation(DETAIL, Console::UP, SORT);
    btnLink.AddRelation(FILTER, Console::DOWN, SORT);
    btnLink.AddRelation(SORT, Console::DOWN, DETAIL);
    btnLink.AddRelation(FILTER, Console::RIGHT, SEARCH);
    btnLink.AddRelation(SORT, Console::RIGHT, SEARCH);
    btnLink.AddRelation(DETAIL, Console::RIGHT, SEARCH);
    btnLink.AddRelation(SEARCH, Console::RIGHT, YEAR);
    btnLink.AddRelation(YEAR, Console::LEFT, SEARCH);
    btnLink.AddRelation(SEARCH, Console::LEFT, FILTER);

    bool redraw = 1;
    bool reload = redraw;
    int filterChoice = 0;
    int sortChoice = 0;
    selection = btnLink.GetDest(selection, key);
    
    while (true) {
    try {
        if (reload) {
            sBillsInYear = GetBills(year, rooms);
            SortBills(sortChoice, sBillsInYear);
            fBillsInYear = FilterBills(filterChoice, sBillsInYear);
            dataBox.Draw();
            reload = false;
        }
        if (redraw) {
            std::system("cls");
            navBox.Draw();
            searchBox.Draw();
            yearBox.Draw();
            filterBox.Draw();
            sortBox.Draw();
            detailBtn.Draw();
            dataBox.Draw();
            redraw = false;
        }

        if (selection == SEARCH) {
            searchBox(key);
            if (key == Console::ENTER) {
                fBillsInYear = SearchBills(searchValue, sBillsInYear);
                dataBox.Draw();
            }
        }
        if (selection == FILTER) {
            int choice = filterBox(key);
            if (choice != -1) {
                fBillsInYear = FilterBills(choice, sBillsInYear);
                dataBox.Draw();
                filterChoice = choice;
            }
        }
        if (selection == SORT) {
            int choice = sortBox(key);
            if (choice != -1) {
                SortBills(choice, sBillsInYear);
                fBillsInYear = FilterBills(filterChoice, sBillsInYear);
                dataBox.Draw();
                sortChoice = choice;
            } 
        }
        if (selection == DETAIL) {
            bool result = detailBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    UpdateBill(fBillsInYear[index].room);
                    redraw = true;
                }
                key = Console::OTHER;
            }
        } 
        if (selection == YEAR) {
            yearBox(key);
            if (key == Console::ENTER) {
                year = SToI(yearStr);
                reload = true;
            } 
        }

        if (key == Console::ESC) return;
        selection = btnLink.GetDest(selection, key);
        if (selection == NAVIGATION) return;
    }
    catch (const exception& e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
        redraw = true;
    }
    catch (const string& e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
        redraw = true;
    }
    }
}

void CreateBills(Nhatro& nhatro, Month month)
{
    auto& rooms = nhatro.GetRooms();
    for (auto& room : rooms) {
        if (room.get()->IsRented()) room.get()->CreateBill(month);
    }
}
bool GetBillForm(MonthlyBill* bill, const string& title, bool add = 0) {
    system("cls");
    int padding = 3;
    FormBox billBox(padding, padding, 80, 0.6, title);
    billBox.AddConstField("Phong", bill->room->GetName());
    if (add) billBox.AddField("Ky thanh toan (MM/YYYY)", to_string(bill->month));
    else billBox.AddConstField("Ky thanh toan", to_string(bill->month));
    billBox.AddField("Tien thue phong", to_string(bill->rentCost), "", " VND");
    billBox.AddField("Tien rac", to_string(bill->trashCost), "", " VND");
    billBox.AddField("Tien mang", to_string(bill->wifiCost), "", " VND");
    billBox.AddField("Tien dich vu", to_string(bill->serviceCost), "", " VND");
    billBox.AddField("Phi thiet hai", to_string(bill->damageCost), "", " VND");
    billBox.AddField("Chi so dien cu", to_string(bill->oldElectricIndex), "", " kWh"); 
    billBox.AddField("Chi so dien moi", to_string(bill->newElectricIndex), "", " kWh"); 
    billBox.AddField("Tien dien", to_string(Room::electricUnitPrice), 
        to_string(bill->electricCost) + " VND (Don gia: ", " VND/kWh)");
    billBox.AddField("Chi so nuoc cu", to_string(bill->oldWaterIndex), "", " m3"); 
    billBox.AddField("Chi so nuoc moi", to_string(bill->newWaterIndex), "", " m3"); 
    billBox.AddField("Tien nuoc", to_string(Room::waterUnitPrice), 
        to_string(bill->waterCost) + " VND (Don gia: ", " VND/m3)");
    billBox.AddChosenField("Da thanh toan", {"CHUA", "ROI"}, bill->isPaid);
    billBox.Draw();

    ull oldElc, newElc, uElec, oldWtr, newWtr, uWtr;
    Month month;
    while (true) {
    try {
        Console::Key key;
        bool confirm = billBox(key);
        if (key == Console::ESC) return false;
        if (confirm) {
            if (add) bill->month = SToDate(billBox.GetValue(1));
            bill->rentCost = SToULL(billBox.GetValue(2));
            bill->trashCost = SToULL(billBox.GetValue(3));
            bill->wifiCost = SToULL(billBox.GetValue(4));
            bill->serviceCost = SToULL(billBox.GetValue(5));
            bill->damageCost = SToULL(billBox.GetValue(6));
            oldElc = SToULL(billBox.GetValue(7));
            newElc = SToULL(billBox.GetValue(8));
            uElec = SToULL(billBox.GetValue(9));
            oldWtr = SToULL(billBox.GetValue(10));
            newWtr = SToULL(billBox.GetValue(11));
            uWtr = SToULL(billBox.GetValue(12));
            bill->isPaid = billBox.GetChosenValue(13);

            if (oldElc > newElc) throw invalid_argument("So dien khong hop le");
            if (oldWtr > newWtr) throw invalid_argument("So nuoc khong hop le");
            bill->oldElectricIndex = oldElc;
            bill->newElectricIndex = newElc;
            bill->oldWaterIndex = oldWtr;
            bill->newWaterIndex = newWtr;

            bill->electricCost = uElec * (newElc - oldElc);
            bill->waterCost = uWtr * (newWtr - oldWtr);
            bill->totalCost = bill->rentCost + bill->trashCost + bill->wifiCost + bill->serviceCost 
            + bill->damageCost + bill->electricCost + bill->waterCost;
            if (bill->isPaid) bill->paymentDate = Nhatro::today;

            Console::SetColor(Console::WHITE);
            Console::ClearSegment(padding, billBox.GetY() + billBox.GetH(), 2, 100);
            Console::Goto(padding, billBox.GetY() + billBox.GetH());
            return true;
        }
    }
    catch (const exception& e) {
        Console::ClearSegment(padding, billBox.GetY() + billBox.GetH(), 1, 100);
        Console::Goto(padding, billBox.GetY() + billBox.GetH());
        Console::SetColor(Console::RED);
        cout << e.what();
    }
    catch (const char* e) {
        Console::ClearSegment(padding, billBox.GetY() + billBox.GetH(), 1, 100);
        Console::Goto(padding, billBox.GetY() + billBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (const string& e) {
        Console::ClearSegment(padding, billBox.GetY() + billBox.GetH(), 1, 100);
        Console::Goto(padding, billBox.GetY() + billBox.GetH());
        Console::SetColor(Console::RED);
        cout << e;
    }
    catch (...) {
        Console::ClearSegment(padding, billBox.GetY() + billBox.GetH(), 1, 100);
        Console::Goto(padding, billBox.GetY() + billBox.GetH());
        Console::SetColor(Console::RED);
        cout << "Loi khong xac dinh";
    }
    }
} 
void UpdateBill(MonthlyBill* bill)
{
    MonthlyBill tmpBill = *bill;
    bool success = GetBillForm(&tmpBill, "CAP NHAT HOA DON");
    if (success) bill->room->UpdateBill(tmpBill);
}
void UpdateBill(Room* room)
{
    system("cls");
    int toolBarW = 32;
    int padding = 2;    

    string prepaidStr = to_string(room->GetPrepaid());
    SingleLineBox prepaidBox(padding, padding + 3, 40, "Tien coc", prepaidStr);
    
    const auto& bills = room->GetMonthlyBills();
    
    NumberMenuBox filterBox(padding, prepaidBox.GetY() + prepaidBox.GetH(), toolBarW, "LOC THONG TIN HOA DON");
    filterBox.AddOption("1. Tat ca hoa don");
    filterBox.AddOption("2. Hoa don da tra");
    filterBox.AddOption("3. Hoa don chua tra");

    NumberMenuBox sortBox(padding, filterBox.GetY() + filterBox.GetH(), toolBarW, "SAP XEP HOA DON");
    sortBox.AddOption("1. Ky thanh toan moi nhat");
    sortBox.AddOption("2. Tong hoa don tang dan");
    sortBox.AddOption("3. Tong hoa don giam dan");

    ButtonBox addBtn(padding, sortBox.GetY() + sortBox.GetH(), toolBarW, "TAO HOA DON");
    ButtonBox detailBtn(padding, addBtn.GetY() + addBtn.GetH(), toolBarW, "XEM VA CAP NHAT HOA DON");
    ButtonBox deleteBtn(padding, detailBtn.GetY() + detailBtn.GetH(), toolBarW, "XOA HOA DON");
    ButtonBox allBtn(padding, deleteBtn.GetY() + deleteBtn.GetH(), toolBarW, "THANH TOAN TOAN BO");

    string fromM, toM;
    Month month1, month2;
    SingleLineBox fromBox(padding*2 + toolBarW, prepaidBox.GetY() + prepaidBox.GetH(), 20, "Tu thang", fromM);
    SingleLineBox toBox(fromBox.GetX() + fromBox.GetW(), fromBox.GetY(), 20, "Den thang", toM);

    List<MonthlyBill*> sBills;
    List<MonthlyBill*> fBills;
    TableBox<MonthlyBill*> dataBox(fromBox.GetX(), fromBox.GetY() + fromBox.GetH(), 
    RoomBillTableWidth(), fBills, RoomBillTableInit, RoomBillTableEnd, RoomBillTableShow);
    
    enum Action {
        NONE,
        PREPAID,
        FROM,
        TO,
        FILTER,
        SORT,
        ADD,
        DETAIL,
        REMOVE,
        ALL
    };
    Action selection = FILTER;

    InputHandler<Action> btnLink(NONE, ALL);
    btnLink.AddRelation(FILTER, Console::UP, PREPAID);
    btnLink.AddRelation(SORT, Console::UP, FILTER);
    btnLink.AddRelation(ADD, Console::UP, SORT);
    btnLink.AddRelation(DETAIL, Console::UP, ADD);
    btnLink.AddRelation(REMOVE, Console::UP, DETAIL);
    btnLink.AddRelation(ALL, Console::UP, REMOVE);
    btnLink.AddRelation(PREPAID, Console::DOWN, FILTER);
    btnLink.AddRelation(FILTER, Console::DOWN, SORT);
    btnLink.AddRelation(SORT, Console::DOWN, ADD);
    btnLink.AddRelation(ADD, Console::DOWN, DETAIL);
    btnLink.AddRelation(DETAIL, Console::DOWN, REMOVE);
    btnLink.AddRelation(REMOVE, Console::DOWN, ALL);
    btnLink.AddRelation(TO, Console::LEFT, FROM);
    btnLink.AddRelation(FROM, Console::LEFT, FILTER);
    btnLink.AddRelation(FROM, Console::RIGHT, TO);
    btnLink.AddRelation(FILTER, Console::RIGHT, FROM);
    btnLink.AddRelation(SORT, Console::RIGHT, FROM);
    btnLink.AddRelation(ADD, Console::RIGHT, FROM);
    btnLink.AddRelation(DETAIL, Console::RIGHT, FROM);
    btnLink.AddRelation(REMOVE, Console::RIGHT, FROM);
    btnLink.AddRelation(ALL, Console::RIGHT, FROM);

    bool redraw = true;
    bool reload = true;
    int filterChoice = 0;
    int sortChoice = 0;
    
    while (true) {
    try {
        if (reload) {
            int st, en;
            if (fromM == "") st = 0;
            else {
                month1 = SToMonth(fromM);
                st = LowerBound(bills, month1) - bills.begin();
            }
            if (toM == "") en = bills.size();
            else {
                month2 = SToMonth(toM);
                en = UpperBound(bills, month2 + 1) - bills.begin();
            }
            sBills.clear();
            for (int i = en - 1; i >= st; i--) sBills.push_back(const_cast<MonthlyBill*>(&bills[i]));
            SortBills(sortChoice, sBills);
            fBills = FilterBills(filterChoice, sBills);
            dataBox.Draw();
            reload = false;
        }
        if (redraw) {
            system("cls");
            Console::Goto(padding, padding);
            cout << setw(20) << "Phong" << ": " << room->GetName() << " (" << room->GetId() << ")";
            Console::Goto(padding, padding + 1);
            cout << setw(20) << "Truong phong" << ": " << room->GetRoomLeader()->GetName();
            Console::Goto(padding, padding + 2);
            cout << setw(20) << "Tong no" << ": " << room->GetDebt();
            prepaidBox.Draw();
            fromBox.Draw();
            toBox.Draw();
            filterBox.Draw();
            sortBox.Draw();
            addBtn.Draw();
            deleteBtn.Draw();
            detailBtn.Draw();
            allBtn.Draw();
            dataBox.Draw();
            redraw = false;
        }

        Console::Key key;
        if (selection == PREPAID) {
            prepaidBox(key);
            room->SetPrepaid(SToULL(prepaidStr));
        }
        if (selection == FILTER) {
            int choice = filterBox(key);
            if (choice != -1) {
                fBills = FilterBills(choice, sBills);
                dataBox.Draw();
                filterChoice = choice;
            }
        }
        if (selection == SORT) {
            int choice = sortBox(key);
            if (choice != -1) {
                SortBills(choice, sBills);
                fBills = FilterBills(filterChoice, sBills);
                dataBox.Draw();
                sortChoice = choice;
            } 
        }
        if (selection == ADD) {
            bool result = addBtn(key);
            if (result) {
                AddBill(room, Nhatro::today);
                reload = true;
            }
        } 
        if (selection == REMOVE) {
            bool result = deleteBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    fBills[index]->room->RemoveBill(fBills[index]->month);
                    reload = true;
                }
                key = Console::OTHER;
            }
        }
        if (selection == DETAIL) {
            bool result = detailBtn(key);
            if (result) {
                int index = dataBox(key);
                if (index != -1) {
                    UpdateBill(fBills[index]);
                    redraw = true;
                }
                key = Console::OTHER;
            }
        } 
        if (selection == ALL) {
            bool result = allBtn(key);
            if (result) {
                for (auto& bill : sBills) bill->isPaid = true;
                dataBox.Draw();
            }
        }
        if (selection == FROM) {
            fromBox(key);
            if (key == Console::ENTER && !fromM.empty()) {
                month1 = SToMonth(fromM);
                reload = true;
            }
        }
        if (selection == TO) {
            toBox(key);
            if (key == Console::ENTER && !toM.empty()) {
                month2 = SToMonth(toM);
                reload = true;
            }
        }
        if (key == Console::ESC) break;
        selection = btnLink.GetDest(selection, key);
    }
    catch (const exception& e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
        redraw = true;
    }
    catch (const char* e) {
        Console::Goto(padding, 0);
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
        redraw = true;
    }
    }
}
List<MonthlyBill*> FilterBills(int choice, const List<MonthlyBill*>& bills)
{
    if (choice <= 0 || choice > 4 || bills.empty()) return bills;
    List<MonthlyBill*> filteredBills;
    bool plea;
    for (const auto& bill : bills) {
        switch (choice) {
        case 1:
            plea = FilterPaidBills(bill);
            break;
        case 2:
            plea = FilterUnPaidBills(bill);
            break;
        default:
            plea = false;
            break;
        }
        if (plea) filteredBills.push_back(bill);
    }
    return filteredBills;
}
void SortBills(int choice, List<MonthlyBill*>& bills)
{
    stable_sort(bills.begin(), bills.end(), 
        [&](MonthlyBill* a, MonthlyBill* b) {
            switch (choice)
            {
            case 1: return CompareBillsMonth(a, b);
            case 2: return CompareBillsTotal(a, b);
            case 3: return CompareBillsTotal1(a, b);
            default: return true;
            }
        });
}
void AddBill(Room* room, Month mon)
{
    MonthlyBill newBill;
    newBill.room = room;
    newBill.month = mon;
    bool success = GetBillForm(&newBill, "TAO HOA DON MOI", true);
    if (success) room->AddBill(newBill);
}

const MonthlyBill* LowerBound(const List<MonthlyBill>& bills, Month month)
{
    if (bills.empty()) return nullptr;
    int left = 0, right = int(bills.size()-1), mid;
    while (left < right) {
        mid = left + (right - left) / 2;
        if (bills[mid].month < month) left = mid + 1;
        else right = mid;
    }
    if (bills[left].month < month) return bills.end();
    return &bills[left];
}
const MonthlyBill* UpperBound(const List<MonthlyBill>& bills, Month month)
{
    if (bills.empty()) return nullptr;
    int left = 0, right = int(bills.size()-1), mid;
    while (left < right) {
        mid = left + (right - left) / 2;
        if (bills[mid].month <= month) left = mid + 1;
        else right = mid;
    }
    if (bills[left].month <= month) return bills.end();
    return &bills[left];
}
List<BillInYear> GetBills(int year, const List<unique_ptr<Room>>& rooms)
{
    List<BillInYear> billsInYear(rooms.size(), {nullptr, List<MonthlyBill*>(12, nullptr)});
    for (int i=0; i < int(rooms.size()); i++) {
        billsInYear[i].room = rooms[i].get();
        auto start = LowerBound(rooms[i].get()->GetMonthlyBills(), Month(1, year));
        auto end = UpperBound(rooms[i].get()->GetMonthlyBills(), Month(12, year));
        for (auto it = start; it != end; it++) {
            billsInYear[i].bills[it->month.month - 1] = const_cast<MonthlyBill*>(it);
        }
    }
    return billsInYear;
}
List<BillInMonth> GetBills(Month month, const List<unique_ptr<Room>>& rooms)
{
    List<BillInMonth> billsInMonth(rooms.size());
    for (int i=0; i < rooms.size(); i++) {
        billsInMonth[i].room = rooms[i].get();
        billsInMonth[i].bill = rooms[i]->GetBill(month);
    }
    return billsInMonth;
}
List<BillInMonth> FilterBills(int choice, const List<BillInMonth>& bills)
{
    if (choice <= 0 || choice > 4 || bills.empty()) return bills;
    List<BillInMonth> filteredBills;
    bool plea;
    for (const auto& bill : bills) {
        switch (choice) {
        case 1:
            plea = FilterPaidBills(bill.bill);
            break;
        case 2:
            plea = FilterUnPaidBills(bill.bill);
            break;
        case 3:
            plea = FilterRoomWithBills(bill.bill);
            break;
        case 4:
            plea = !FilterRoomWithBills(bill.bill);
            break;
        default:
            plea = false;
            break;
        }
        if (plea) filteredBills.push_back(bill);
    }
    return filteredBills;
}
bool FilterPaidBills(MonthlyBill* bill)
{
    if (bill == nullptr) return false;
    return bill->isPaid;
}
bool FilterUnPaidBills(MonthlyBill* bill)
{
    if (bill == nullptr) return false;
    return !bill->isPaid;
}
bool FilterRoomWithBills(MonthlyBill* bill)
{
    return bill != nullptr;
}
List<BillInYear> FilterBills(int choice, const List<BillInYear>& bills)
{
    if (choice <= 0 || choice > 3 || bills.empty()) return bills;
    List<BillInYear> filteredBills;
    bool plea;
    for (const auto& bill : bills) {
        switch (choice) {
        case 1:
            plea = FilterRoomWithDebt(bill.room);
            break;
        case 2:
            plea = !FilterRoomWithDebt(bill.room);
            break;
        case 3:
            plea = FilterRoomWithDebtOverPrepaid(bill.room);
            break;
        default:
            plea = false;
            break;
        }
        if (plea) filteredBills.push_back(bill);
    }
    return filteredBills;
}
bool FilterRoomWithDebt(Room* room)
{
    return room->GetDebt() > 0;
}
bool FilterRoomWithDebtOverPrepaid(Room* room)
{
    return room->GetDebt() > room->GetPrepaid();
}
void SortBills(int choice, List<BillInMonth>& bills)
{
    stable_sort(bills.begin(), bills.end(), 
        [&](BillInMonth a, BillInMonth b) {
            switch (choice)
            {
            case 0: return CompareBillsRoom(a.room, b.room);
            case 1: return CompareBillsPaymentDate(a.bill, b.bill);
            case 2: return CompareBillsTotal(a.bill, b.bill);
            case 3: return CompareBillsTotal1(a.bill, b.bill);
            default: return true;
            }
        });
}
bool CompareBillsRoom(Room* room1, Room* room2)
{
    return room1->GetId() < room2->GetId();
}
bool CompareBillsPaymentDate(MonthlyBill* bill1, MonthlyBill* bill2)
{
    if (bill1 == nullptr) return false;
    if (bill2 == nullptr) return true;
    return bill1->paymentDate > bill2->paymentDate;
}
bool CompareBillsMonth(MonthlyBill* bill1, MonthlyBill* bill2)
{
    if (bill1 == nullptr) return false;
    if (bill2 == nullptr) return true;
    return bill1->month > bill2->month;
}
bool CompareBillsTotal(MonthlyBill* bill1, MonthlyBill* bill2)
{
    if (bill1 == nullptr) return false;
    if (bill2 == nullptr) return true;
    return bill1->totalCost < bill2->totalCost;
}
bool CompareBillsTotal1(MonthlyBill* bill1, MonthlyBill* bill2)
{
    if (bill1 == nullptr) return false;
    if (bill2 == nullptr) return true;
    return bill1->totalCost > bill2->totalCost;
}
void SortBills(int choice, List<BillInYear>& bills)
{
    stable_sort(bills.begin(), bills.end(), 
        [&](BillInYear a, BillInYear b) {
            switch (choice)
            {
            case 0: return CompareBillsRoom(a.room, b.room);
            case 1: return CompareRevenue(a.bills, b.bills);
            case 2: return CompareRevenue1(a.bills, b.bills);
            case 3: return CompareDebt(a.room, b.room);
            case 4: return CompareDebt1(a.room, b.room);
            default: return true;
            }
        });
}
bool CompareRevenue(List<MonthlyBill*> bills1, List<MonthlyBill*> bills2)
{
    ull total1 = 0;
    for (const auto& bill : bills1) {
        if (bill != nullptr) total1 += bill->totalCost;
    }
    ull total2 = 0;
    for (const auto& bill : bills2) {
        if (bill != nullptr) total2 += bill->totalCost;
    }
    return total1 < total2;
}
bool CompareRevenue1(List<MonthlyBill*> bills1, List<MonthlyBill*> bills2)
{
    ull total1 = 0;
    for (const auto& bill : bills1) {
        if (bill != nullptr) total1 += bill->totalCost;
    }
    ull total2 = 0;
    for (const auto& bill : bills2) {
        if (bill != nullptr) total2 += bill->totalCost;
    }
    return total1 > total2;
}
bool CompareDebt(Room* room1, Room* room2)
{
    return room1->GetDebt() < room2->GetDebt();
}
bool CompareDebt1(Room* room1, Room* room2)
{
    return room1->GetDebt() > room2->GetDebt();
}
List<BillInMonth> SearchBills(const std::string& query, List<BillInMonth>& bills)
{
    List<BillInMonth> results;
    for (const auto& bill : bills) {
        if (SearchRooms(bill.room, query)) results.push_back(bill);
    }
    return results;
}
List<BillInYear> SearchBills(const std::string& query, List<BillInYear>& bills)
{
    List<BillInYear> results;
    for (const auto& bill : bills) {
        if (SearchRooms(bill.room, query)) results.push_back(bill);
    }
    return results;
}

void BillInMonthTableInit(int x, int y)
{
    string costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_DT;
    Console::Goto(x, y);
    cout << BORDER_TL 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_DT
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(ROOM_ID_W)      << "Ma phong"       << BORDER_V
        << setw(BILL_COST_W)    << "TIEN PHONG"      << BORDER_V
        << setw(BILL_COST_W)    << "TIEN RAC"       << BORDER_V
        << setw(BILL_COST_W)    << "TIEN MANG"      << BORDER_V
        << setw(BILL_COST_W)    << "TIEN D.VU"        << BORDER_V
        << setw(BILL_COST_W)    << "THIET HAI"      << BORDER_V
        << setw(BILL_COST_W)    << "TIEN DIEN"      << BORDER_V
        << setw(BILL_COST_W)    << "TIEN NUOC"      << BORDER_V
        << setw(BILL_COST_W)    << "Tong cong"      << BORDER_V
        << setw(BILL_STATUS_W)  << "Trang thai"     << BORDER_V;

    costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_CROSS;
    Console::Goto(x, y+2);
    cout << BORDER_RT 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_CROSS
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_LT;
}
void BillInMonthTableEnd(int x, int y)
{
    string costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_UT;
    Console::Goto(x, y);
    cout << BORDER_BL 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_UT
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_BR;
}
int BillInMonthTableWidth()
{
    return ROOM_ID_W + 8*BILL_COST_W + BILL_STATUS_W + 11;
}
void BillInMonthTableShow(const BillInMonth& bill)
{
    cout << left << BORDER_V
        << setw(ROOM_ID_W)      << bill.room->GetId()                                    << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->rentCost) : "")     << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->trashCost) : "")    << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->wifiCost) : "")     << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->serviceCost) : "")  << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->damageCost) : "")   << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->electricCost) : "") << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->waterCost) : "")    << BORDER_V
        << setw(BILL_COST_W)    << (bill.bill ? to_string(bill.bill->totalCost) : "")    << BORDER_V
        << setw(BILL_STATUS_W)  << (bill.bill ? (bill.bill->isPaid ? "Da thanh toan" : "Chua thanh toan") : "") << BORDER_V;
}

void BillInYearTableInit(int x, int y)
{
    string costCell = Repeat(BILL_Y_COST_W, BORDER_H) + BORDER_DT;
    Console::Goto(x, y);
    cout << BORDER_TL 
        << Repeat(ROOM_ID_W, BORDER_H)    << BORDER_DT
        << Repeat(12, costCell.c_str())
        << Repeat(BILL_Y_COST_W, BORDER_H)  << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V << setw(ROOM_ID_W) << "Ma phong" << BORDER_V;
    for (int i=1; i<=12; i++) cout << setw(BILL_Y_COST_W) << "Thang " + to_string(i) << BORDER_V;
    cout << setw(BILL_Y_COST_W) << "Tong no" << BORDER_V;

    costCell = Repeat(BILL_Y_COST_W, BORDER_H) + BORDER_CROSS;
    Console::Goto(x, y+2);
    cout << BORDER_RT 
        << Repeat(ROOM_ID_W, BORDER_H)    << BORDER_CROSS
        << Repeat(12, costCell.c_str())
        << Repeat(BILL_Y_COST_W, BORDER_H)  << BORDER_LT;
}
void BillInYearTableEnd(int x, int y)
{
    string costCell = Repeat(BILL_Y_COST_W, BORDER_H) + BORDER_UT;
    Console::Goto(x, y);
    cout << BORDER_BL 
        << Repeat(ROOM_ID_W, BORDER_H)    << BORDER_UT
        << Repeat(12, costCell.c_str())
        << Repeat(BILL_Y_COST_W, BORDER_H)  << BORDER_BR;
}
int BillInYearTableWidth()
{
    return ROOM_ID_W + 13*BILL_Y_COST_W + 15;
}
void BillInYearTableShow(const BillInYear& bill)
{
    cout << left << BORDER_V << setw(ROOM_ID_W) << bill.room->GetId() << BORDER_V;
    for (int i=0; i<bill.bills.size(); i++) {
        cout << setw(BILL_Y_COST_W);
        if (bill.bills[i] == nullptr) {
            cout << "";
        }
        else {
            cout << bill.bills[i]->totalCost;
        }
        cout << BORDER_V;
    }
    cout << setw(BILL_Y_COST_W) << (bill.room->GetDebt()) << BORDER_V;
}

void RoomBillTableInit(int x, int y)
{
    string costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_DT;
    Console::Goto(x, y);
    cout << BORDER_TL 
        << Repeat(BILL_MONTH_W, BORDER_H)  << BORDER_DT
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(BILL_MONTH_W)   << "Thang"          << BORDER_V
        << setw(BILL_COST_W)    << "$ PHONG"      << BORDER_V
        << setw(BILL_COST_W)    << "$ RAC"       << BORDER_V
        << setw(BILL_COST_W)    << "$ MANG"      << BORDER_V
        << setw(BILL_COST_W)    << "$ D.VU"        << BORDER_V
        << setw(BILL_COST_W)    << "$ HU HAI"      << BORDER_V
        << setw(BILL_COST_W)    << "$ DIEN"      << BORDER_V
        << setw(BILL_COST_W)    << "$ NUOC"      << BORDER_V
        << setw(BILL_COST_W)    << "Tong cong"      << BORDER_V
        << setw(BILL_STATUS_W)  << "Trang thai"     << BORDER_V;

    costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_CROSS;
    Console::Goto(x, y+2);
    cout << BORDER_RT 
        << Repeat(BILL_MONTH_W, BORDER_H)  << BORDER_CROSS
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_LT;
}
void RoomBillTableEnd(int x, int y)
{
    string costCell = Repeat(BILL_COST_W, BORDER_H) + BORDER_UT;
    Console::Goto(x, y);
    cout << BORDER_BL 
        << Repeat(BILL_MONTH_W, BORDER_H)  << BORDER_UT
        << Repeat(8, costCell.c_str())
        << Repeat(BILL_STATUS_W, BORDER_H) << BORDER_BR;
}
int RoomBillTableWidth()
{
    return BILL_MONTH_W + BILL_COST_W * 8 + BILL_STATUS_W + 11;
}
void RoomBillTableShow(MonthlyBill* const& bill)
{
    if (bill == nullptr) cout << "Khong tim thay du lieu";
    cout << left << BORDER_V
        << setw(BILL_MONTH_W)   << bill->month        << BORDER_V
        << setw(BILL_COST_W)    << bill->rentCost     << BORDER_V
        << setw(BILL_COST_W)    << bill->trashCost    << BORDER_V
        << setw(BILL_COST_W)    << bill->wifiCost     << BORDER_V
        << setw(BILL_COST_W)    << bill->serviceCost  << BORDER_V
        << setw(BILL_COST_W)    << bill->damageCost   << BORDER_V
        << setw(BILL_COST_W)    << bill->electricCost << BORDER_V
        << setw(BILL_COST_W)    << bill->waterCost    << BORDER_V
        << setw(BILL_COST_W)    << bill->totalCost    << BORDER_V
        << setw(BILL_STATUS_W)  << (bill->isPaid ? "Da thanh toan" : "Chua thanh toan") << BORDER_V;
}

void IndexTableInit(int x, int y)
{
    string costCell = Repeat(INDEX_W, BORDER_H) + BORDER_DT;
    Console::Goto(x, y);
    cout << BORDER_TL 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_DT
        << Repeat(3, costCell.c_str())
        << Repeat(INDEX_W, BORDER_H) << BORDER_TR;

    Console::Goto(x, y+1);
    cout << left << BORDER_V
        << setw(ROOM_ID_W)      << "Ma phong"       << BORDER_V
        << setw(INDEX_W)    << "So dien cu"      << BORDER_V
        << setw(INDEX_W)    << "So dien moi"       << BORDER_V
        << setw(INDEX_W)    << "So nuoc cu"      << BORDER_V
        << setw(INDEX_W)    << "So nuoc moi"        << BORDER_V;

    costCell = Repeat(INDEX_W, BORDER_H) + BORDER_CROSS;
    Console::Goto(x, y+2);
    cout << BORDER_RT 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_CROSS
        << Repeat(3, costCell.c_str())
        << Repeat(INDEX_W, BORDER_H) << BORDER_LT;
}
void IndexTableEnd(int x, int y)
{
    string costCell = Repeat(INDEX_W, BORDER_H) + BORDER_UT;
    Console::Goto(x, y);
    cout << BORDER_BL 
        << Repeat(ROOM_ID_W, BORDER_H)     << BORDER_UT
        << Repeat(3, costCell.c_str())
        << Repeat(INDEX_W, BORDER_H) << BORDER_BR;
}
int IndexTableWidth()
{
    return ROOM_ID_W + 4 * INDEX_W + 6;
}
void IndexTableShow(Room* const& room)
{
    cout << left << BORDER_V
        << setw(ROOM_ID_W)      << room->GetId()       << BORDER_V
       << center(to_string(room->GetOldElectricIndex()), INDEX_W)      << BORDER_V
       << center(to_string(room->GetNewElectricIndex()), INDEX_W)      << BORDER_V
       << center(to_string(room->GetOldWaterIndex()), INDEX_W)      << BORDER_V
       << center(to_string(room->GetNewWaterIndex()), INDEX_W)      << BORDER_V;
}