#include <Nhatro.h>
using namespace std;

Nhatro::Nhatro(const string& name, const string& address, const string& phone)
    : name(name), address(address), phone(phone)
{}

Nhatro::~Nhatro()
{}

void Nhatro::SetAddress(const std::string& newAddress)
{
    this->address = newAddress;
}

void Nhatro::SetPhone(const std::string& newPhone)
{
    this->phone = newPhone;
}

const std::string& Nhatro::GetAddress() const
{
    return this->address;
}

const std::string& Nhatro::GetPhone() const
{
    return this->phone;
}

int Nhatro::GetNumRoom() const
{
    return this->rooms.size();
}

Room* Nhatro::AddRoom(std::unique_ptr<Room> newRoom)
{
    this->rooms.push_back(newRoom);
}

bool Nhatro::RemoveRoom(Room* targetRoom)
{
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        if (it->get() == targetRoom){
            this->rooms.erase(it);
            return true;
        }
    }
    return false;
}

void Nhatro::ShowAvailableRoom() const
{
    for (auto it=this->rooms.begin(); it!=this->rooms.end(); it++){
        if (!(it->get()->IsFull())){
            cout << it->get();
            cout << endl;
        }
    }
}