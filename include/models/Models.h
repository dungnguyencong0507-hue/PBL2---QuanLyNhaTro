#pragma once
#include <string>
#include <header.h>
using std::endl;
using std::left;
using std::ostream;
using std::string;
using std::setw;
using std::invalid_argument;
using ull = unsigned long long;

struct Month
{
    int month;
    int year;

    bool operator==(const Month& other) const {
        return month == other.month && year == other.year;
    }
    bool operator!=(const Month& other) const {
        return !(*this == other);
    }
    bool operator<(const Month& other) const {
        return (year < other.year) || (year == other.year && month < other.month);
    }
    bool operator>(const Month& other) const {
        return (year > other.year) || (year == other.year && month > other.month);
    }
    bool operator<=(const Month& other) const {
        return !(*this > other);
    }
    bool operator>=(const Month& other) const {
        return !(*this < other);
    }
    Month operator-(int months) const {
        Month result = *this;
        result.month -= months;
        while (result.month < 1) {
            result.month += 12;
            result.year--;
        }
        return result;
    }
    Month operator+(int months) const {
        Month result = *this;
        result.month += months;
        while (result.month > 12) {
            result.month -= 12;
            result.year++;
        }
        return result;
    }

    Month() : month(1), year(2000) {}
    Month(int m, int y) : month(m), year(y) {}

    Month(const std::string& str) {
        size_t pos = str.find('/');
        if (pos == std::string::npos) {
            throw std::invalid_argument("Dinh dang thang khong hop le: " + str);
        }
        month = std::stoi(str.substr(0, pos));
        year = std::stoi(str.substr(pos + 1));
    }
};

inline int DaysInMonth(int month, int year)
{
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) throw invalid_argument("So thang khong hop le");
    if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
        return 29;
    return days[month - 1];
}

struct Date : public Month
{
    int day;

    Date() : Month(), day(1) {}
    Date(int d, int m, int y) : Month(m, y), day(d) {}

    Date(const std::string& str) : Month(str) {
        size_t firstSlash = str.find('/');
        size_t secondSlash = str.find('/', firstSlash + 1);
        if (firstSlash == std::string::npos || secondSlash == std::string::npos) {
            throw std::invalid_argument("Dinh dang thang khong hop le: " + str);
        }
        this->day = std::stoi(str.substr(0, firstSlash));
        this->month = std::stoi(str.substr(firstSlash + 1, secondSlash - firstSlash - 1));
        this->year = std::stoi(str.substr(secondSlash + 1));
    }

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    bool operator!=(const Date& other) const {
        return !(*this == other);
    }
    bool operator<(const Date& other) const {
        return (year < other.year) || (year == other.year && month < other.month)
        || (year == other.year && month == other.month && day < other.day);
    }
    bool operator>(const Date& other) const {
        return (year > other.year) || (year == other.year && month > other.month)
        || (year == other.year && month == other.month && day > other.day);
    }
    bool operator<=(const Date& other) const {
        return !(*this > other);
    }
    bool operator>=(const Date& other) const {
        return !(*this < other);
    }
    Date operator-(int days) const {
        Date result = *this;
        result.day -= days;
        while (result.day < 1) {
            result.month--;
            while (result.month < 1) {
                result.month += 12;
                result.year--;
            }
            result.day += DaysInMonth(result.month, result.year);
        }
        return result;
    }
    Date operator+(int days) const {
        Date result = *this;
        result.day += days;
        while (result.day > DaysInMonth(result.month, result.year)) {
            result.day -= DaysInMonth(result.month, result.year);
            result.month++;
            while (result.month > 12) {
                result.month -= 12;
                result.year++;
            }
        }
        return result;
    }
};

inline string to_string(const Month& m) {
    return std::to_string(m.month) + "/" + std::to_string(m.year);
}

inline string to_string(const Date& d) {
    return std::to_string(d.day) + "/" + std::to_string(d.month) + "/" + std::to_string(d.year);
}

inline ostream& operator<<(ostream& o, const Month& m) {
    o << to_string(m);
    return o;
}

inline ostream& operator<<(ostream& o, const Date& d) {
    o << to_string(d);
    return o;
}

class Room;
struct MonthlyBill
{
    Month month;
    Room* room;
    ull rentCost;
    ull trashCost;
    ull wifiCost;
    ull serviceCost;
    ull damageCost;

    ull oldElectricIndex;
    ull newElectricIndex;
    ull electricCost;

    ull oldWaterIndex;
    ull newWaterIndex;
    ull waterCost;
    
    ull totalCost;
    bool isPaid;
    Date paymentDate;

    MonthlyBill() : month(1, 2024), room(nullptr), rentCost(0), trashCost(0), wifiCost(0),
                    serviceCost(0), damageCost(0),
                    oldElectricIndex(0), newElectricIndex(0), electricCost(0),
                    oldWaterIndex(0), newWaterIndex(0), waterCost(0),
                    totalCost(0), isPaid(0), paymentDate(1, 1, 2024)  {}

    MonthlyBill(Month m, Room* room, ull rent, ull trash, ull wifi, ull service, ull damage,
        ull oldElec, ull newElec, ull elecCost,
        ull oldWater, ull newWater, ull waterCost,
        ull total, bool paid, Date payment = Date(1, 1, 2024))
        : month(m), room(room), rentCost(rent), trashCost(trash), wifiCost(wifi),
          serviceCost(service), damageCost(damage),
          oldElectricIndex(oldElec), newElectricIndex(newElec), electricCost(elecCost),
          oldWaterIndex(oldWater), newWaterIndex(newWater), waterCost(waterCost),
          totalCost(total), isPaid(paid), paymentDate(payment) {}
};

ostream& operator<<(ostream& o, const MonthlyBill& bill);

struct Furnishing
{
    std::string id;
    std::string name;
    ull price;

    Furnishing(string id = "", string name = "", ull price = 0)
        : id(id), name(name), price(price) {}
};

struct Damage
{
    Date date;
    const Furnishing* furnishing;
    std::string description;
    ull compen;

    Damage() : date(1, 1, 2000), furnishing(nullptr), description(""), compen(0) {}
    Damage(Date date, const Furnishing* furn = nullptr, string descrip = "", ull cost = 0)
     : date(date), furnishing(furn), description(descrip), compen(cost) {}
};
