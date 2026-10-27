#pragma once
#include <Nhatro.h>
#include <Tenant.h>

void LoadTenantData(const std::string&, Nhatro&);
void SaveTenantData(const std::string&, Nhatro&);

void ManageTenants(Nhatro&);
void ShowTenants(Nhatro&);
void FindTenant(Nhatro&);
void AddTenant(Nhatro&);
void UpdateTenant(Nhatro&);
