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

        nhatro.LoadRoomData("Room.txt", "Bill.txt", "Furnishing.txt", "Damage.txt");
        nhatro.LoadTenantData("Tenant.txt");
        Sleep(1000);
        system("cls");

        MenuBox menu(2, 1, 38, "QUAN LY NHA TRO");
        menu.AddOption("Quan ly phong");
        menu.AddOption("Quan ly khach thue");
        menu.AddOption("Quan ly hoa don va dien nuoc");
        menu.AddOption("Bao cao tai chinh");

        Console::Key k;
        int choice;
        menu.Draw();
        while ((choice = menu(k)) != -1){
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
                case 3:
                    break;
            }
            system("cls");
            menu.Draw();
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