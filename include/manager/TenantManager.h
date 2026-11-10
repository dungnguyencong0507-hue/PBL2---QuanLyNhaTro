#pragma once
#include <header.h>
#include <Manager.h>
#include <Nhatro.h>
#include <Tenant.h>
#include <Checker.h>

void ManageTenants(Nhatro&);
void AddTenant(Nhatro&);
void UpdateTenant(Nhatro&, Tenant*);
bool SearchTenant(Tenant*, const std::string&); // Theo ten, cccd, sdt, que quan, phong
bool FilterRentingTenants(Tenant*);
bool FilterNotRentingTenants(Tenant*);
bool FilterRepresentativeTenants(Tenant*);
bool FilterTenantsGotTmpRsd(Tenant*);
bool FilterTenantsNotTmpRsd(Tenant*);
bool CompareTenantsName(Tenant*, Tenant*);
bool CompareTenantsName1(Tenant*, Tenant*);
bool CompareTenantsRoom(Tenant*, Tenant*);

void TenantTableInit(int x, int y);
void TenantTableEnd(int x, int y);
int TenantTableWidth();
void TenantTableShow(Tenant* const&);
