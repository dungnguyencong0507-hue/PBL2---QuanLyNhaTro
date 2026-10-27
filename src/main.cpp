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

int main()
{
    try {
        Nhatro nhatro;
        string roomFile = "Room.txt";
        string tenantFile = "Tenant.txt";
        string billFile = "Bill.txt";

        LoadRoomData(roomFile, nhatro);
        LoadTenantData(tenantFile, nhatro);
        LoadInvoiceData(billFile, nhatro);

        MenuBox menu(2, 1, 38, "QUAN LY NHA TRO");
        menu.AddOption("Quan ly phong");
        menu.AddOption("Quan ly khach thue");
        menu.AddOption("Quan ly hoa don");
        menu.AddOption("Thoat");

        int choice;
        while ((choice = menu.Exec()) != -1){
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
                    return 0;
            }
        }
    }
    catch (const exception& error){
        cerr << "\nLoi: " << error.what() << '\n';
        return 1;
    }
    catch (const char* error){
        cerr << "\nLoi: " << error << '\n';
        return 1;
    }

    return 0;
}