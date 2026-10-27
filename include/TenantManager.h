#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Tenant.h"

class TenantManager
{
private:
    std::vector<std::unique_ptr<Tenant>> tenants;
public:
    bool AddTenant(std::unique_ptr<Tenant>); //them khach tro
    bool RemoveTenant(Tenant*);                     
    Tenant* FindTenantByCCCD(const std::string&);
    bool LoadTenantData(std::string&);
    void SaveTenantData(std::string&);
    void ShowTenantDetail(Tenant*);
};