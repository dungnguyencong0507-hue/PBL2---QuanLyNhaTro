#pragma once
#include <iostream>
#include <Nhatro.h>
#include <ConsoleUI.h>
#include <header.h>
#include <Checker.h>
#include <RoomManager.h>

#define BILL_MONTH_W 8
#define BILL_QUANT_W 8
#define BILL_PAYER_W 18
#define BILL_COST_W 10
#define BILL_Y_COST_W 8
#define BILL_STATUS_W 15

#define INDEX_W 15

struct BillInMonth {
    Room* room;
    MonthlyBill* bill;
};

struct BillInYear {
    Room* room;
    List<MonthlyBill*> bills;
};

void ManageBillsInMonth(Nhatro&, NavigationBox&, Console::Key&);
void ManageBillsInYear(Nhatro&, NavigationBox&, Console::Key&);
void ManageBills(Nhatro&);
void CreateBills(Nhatro&, Month);
void AddBill(Room*, Month = Nhatro::today);
void UpdateBill(MonthlyBill*);
void UpdateBill(Room*);
void UpdateIndex(Room*);
void ManageIndex(Nhatro&, NavigationBox&, Console::Key&);
void UpdateGeneralPrice(NavigationBox&, Console::Key&);
void ShowFinanceReport(Nhatro&);

List<MonthlyBill*> FilterBills(int, const List<MonthlyBill*>&);
void SortBills(int, List<MonthlyBill*>&);

const MonthlyBill* LowerBound(const List<MonthlyBill>&, Month);
const MonthlyBill* UpperBound(const List<MonthlyBill>&, Month);

List<BillInYear> GetBills(int, const List<unique_ptr<Room>>&);
List<BillInMonth> GetBills(Month, const List<unique_ptr<Room>>&);

List<BillInMonth> FilterBills(int, const List<BillInMonth>&);
bool FilterPaidBills(MonthlyBill*);
bool FilterUnPaidBills(MonthlyBill*);
bool FilterRoomWithBills(MonthlyBill*);
List<BillInYear> FilterBills(int, const List<BillInYear>&);
bool FilterRoomWithDebt(Room*);
bool FilterRoomWithDebtOverPrepaid(Room*);

void SortBills(int, List<BillInMonth>&);
bool CompareBillsRoom(Room*, Room*);
bool CompareBillsPaymentDate(MonthlyBill*, MonthlyBill*);
bool CompareBillsMonth(MonthlyBill*, MonthlyBill*);
bool CompareBillsTotal(MonthlyBill*, MonthlyBill*);
bool CompareBillsTotal1(MonthlyBill*, MonthlyBill*);
void SortBills(int, List<BillInYear>&);
bool CompareRevenue(List<MonthlyBill*>, List<MonthlyBill*>);
bool CompareRevenue1(List<MonthlyBill*>, List<MonthlyBill*>);
bool CompareDebt(Room*, Room*);
bool CompareDebt1(Room*, Room*);

List<BillInMonth> SearchBills(const std::string&, List<BillInMonth>&);
List<BillInYear> SearchBills(const std::string&, List<BillInYear>&);

void BillInMonthTableInit(int x, int y);
void BillInMonthTableEnd(int x, int y);
int BillInMonthTableWidth();
void BillInMonthTableShow(const BillInMonth&);

void BillInYearTableInit(int x, int y);
void BillInYearTableEnd(int x, int y);
int BillInYearTableWidth();
void BillInYearTableShow(const BillInYear&);

void RoomBillTableInit(int x, int y);
void RoomBillTableEnd(int x, int y);
int RoomBillTableWidth();
void RoomBillTableShow(MonthlyBill* const&);

void IndexTableInit(int x, int y);
void IndexTableEnd(int x, int y);
int IndexTableWidth();
void IndexTableShow(Room* const&);
