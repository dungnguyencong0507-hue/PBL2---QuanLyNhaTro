#include <iostream>
#include <stdexcept>
#include <string>
#include <header.h>
#include <Nhatro.h>
#include <BillManager.h>
#include <RoomManager.h>
#include <TenantManager.h>
#include <ConsoleUI.h>
using namespace std;

Nhatro* database = nullptr;

void SaveData()
{
    database->SaveGeneralData("General.txt");
    database->SaveRoomData("Room.txt", "Bill.txt", "Furnishing.txt", "Damage.txt");
    database->SaveTenantData("Tenant.txt");
}

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT) {
        
        int choice = MessageBoxA(
            NULL, 
            "Ban co muon luu lai du lieu truoc khi thoat khong?", 
            "Xac nhan luu", 
            MB_YESNO | MB_ICONQUESTION
        );

        if (choice == IDYES && database != nullptr) {
            SaveData();
        }

        return TRUE; 
    }
    return FALSE;
}

void ShowInformation(int x, int y)
{
    Console::SetColor(Console::YELLOW);
    Console::Goto(x, y-6);
    cout << "PL2 - Do an co so lap trinh";
    Console::Goto(x, y-5);
    cout << "De tai: Chuong trinh quan ly nha tro";
    Console::Goto(x, y-3);
    cout << "Sinh vien thuc hien:";
    Console::Goto(x, y-2);
    cout << "  - Nguyen Cong Dung   MSSV: 102250113    Lop: 25T_DT3";
    Console::Goto(x, y-1);
    cout << "  - Nguyen Van Tai     MSSV: 102250143    Lop: 25T_DT3";
    Console::Goto(x, y);
    cout << "Giang vien huong dan: Do Thi Tuyet Hoa";
}

int main()
{
    try {
        if (!SetConsoleOutputCP(CP_UTF8) || !SetConsoleCP(CP_UTF8))
            throw runtime_error("Khong the cai dat ma hoa UTF-8 cho console!");
        SetConsoleCtrlHandler(ConsoleHandler, TRUE);

        Nhatro& nhatro = Nhatro::GetInstance();
        database = &nhatro;

        nhatro.LoadGeneralData("General.txt");
        nhatro.LoadRoomData("Room.txt", "Bill.txt", "Furnishing.txt", "Damage.txt");
        nhatro.LoadTenantData("Tenant.txt");
        Sleep(1000);
        system("cls");

        MenuBox menu(2, 2, 40, "QUAN LY NHA TRO");
        menu.AddOption("Quan ly phong");
        menu.AddOption("Quan ly khach thue");
        menu.AddOption("Quan ly hoa don va dien nuoc");
        string dateStr = to_string(Nhatro::today);
        SingleLineBox dateBox(menu.GetX(), menu.GetY() - 3, 40, "Ngay hien tai", dateStr);

        bool func = 1;
        int choice;
        Console::Key key;
        while (true){
        try {
            system("cls");
            menu.GoToCenter(0, 13);
            dateBox.ResetPosition(menu.GetX(), menu.GetY() - 3);
            ShowInformation(dateBox.GetX(), dateBox.GetY() - 2);
            dateBox.Draw();
            menu.Draw();

            if (func == 0) {
                dateBox(key);
                if (key == Console::ESC) break;
                if (key == Console::DOWN) func = 1;
                if (key == Console::ENTER) {
                    Nhatro::today = SToDate(dateStr);
                    func = 1;
                    dateBox.Quit();
                }
            }
            if (func == 1) {
                choice = menu(key);
                if (key == Console::ESC) func = 0;
                switch (choice){
                    case 0:
                        ManageRooms(nhatro);
                        break;
                    case 1:
                        ManageTenants(nhatro);
                        break;
                    case 2:
                        ManageBills(nhatro);
                        break;
                    default:
                        break;
                }
            }
        }
        catch (const exception& error){
            Console::Goto(menu.GetX(), menu.GetY() + menu.GetH());
            Console::SetColor(Console::RED);
            cerr << "\nLoi: " << error.what();
        }
        catch (const char* error){
            Console::Goto(menu.GetX(), menu.GetY() + menu.GetH());
            Console::SetColor(Console::RED);
            cerr << "\nLoi: " << error;
        }
        }
        ConsoleHandler(CTRL_CLOSE_EVENT);
    }
    catch (const exception& error){
        cerr << "\nLoi: " << error.what();
        return 1;
    }
    catch (const char* error){
        cerr << "\nLoi: " << error;
        return 1;
    }
    return 0;
}