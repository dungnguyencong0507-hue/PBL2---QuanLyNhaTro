#include <string.h>
#include <iostream>

class Phong
{
    public:
        static int stdPrice[10][5];
    private:
        std::string id;           //ma phong
        std::string name;         //ten phong
        std::string trpID;
        int capa;                 //so luong 
        int floor;
        int type;
        int hirePrice;                //gia thue (VND)
        int servivePrice;
        int *electricPrice;
        int *waterPrice;
        bool isHired;
        bool isFull;
    
    public:
        Phong();
        ~Phong();

        bool IsHired();
        bool IsFull();
        void Hired();
        void SetPrice(const int&);
        int GetPrice();
        int GetElectricPrice();
        int GetWaterPrice();
        void PayBill();
        void GetBill();
        void GetDebt();
        
};