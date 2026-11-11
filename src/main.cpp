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
    system("cls");
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

        string dateStr;
        SingleLineBox dateBox(10, 10, 40, "Ngay hien tai", dateStr);

        MenuBox menu(dateBox.GetX(), dateBox.GetY() + dateBox.GetH(), 40, "QUAN LY NHA TRO");
        menu.AddOption("Quan ly phong");
        menu.AddOption("Quan ly khach thue");
        menu.AddOption("Quan ly hoa don va dien nuoc");
        // menu.AddOption("Bao cao tai chinh");

        bool func = 0;
        int choice;
        menu.Draw();
        dateBox.Draw();
        Console::Key key;
        while (true){
        try {
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
            system("cls");
            dateBox.Draw();
            menu.Draw();
        }
        catch (const exception& error){
            Console::Goto(menu.GetX(), menu.GetY() + menu.GetH());
            Console::SetColor(Console::RED);
            cerr << "\nLoi: " << error.what() << '\n';
            system("pause");
        }
        catch (const char* error){
            Console::Goto(menu.GetX(), menu.GetY() + menu.GetH());
            Console::SetColor(Console::RED);
            cerr << "\nLoi: " << error << '\n';
            system("pause"); 
        }
        }
        ConsoleHandler(CTRL_CLOSE_EVENT);
    }
    catch (const exception& error){
        cerr << "\nLoi: " << error.what() << '\n';
        system("pause"); 
        return 1;
    }
    catch (const char* error){
        cerr << "\nLoi: " << error << '\n';
        system("pause"); 
        return 1;
    }
    return 0;
}