#pragma once
#include <iostream>
#include <Nhatro.h>

void LoadInvoiceData(std::string&, Nhatro&);
void SaveInvoiceData(std::string&, Nhatro&);

void ManageBills(Nhatro&);
void CreateBill(Nhatro&);
void UpdateBillStatus(Nhatro&);
void UpdateGeneralPrice(Nhatro&);
void UpdateUtilityIndices(Nhatro&);
void ShowFinanceReport(Nhatro&);
void ShowInvoiceHistory(Nhatro&);