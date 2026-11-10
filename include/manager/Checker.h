#pragma once
#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>
#include <header.h>
#include <Models.h>
using std::invalid_argument;
using std::size_t;
using std::string;
using std::to_string;
using std::stoi;
using std::stoull;
using std::isalnum;
using std::isalpha;
using std::isdigit;

inline bool isNum(const string& value)
{
    for (unsigned char c : value)
        if (!isdigit(c)) return false;
    return !value.empty();
}

inline int SToI(const string& value)
{
    if (!isNum(value)) throw invalid_argument("Dinh dang so nguyen khong hop le: " + value);
    if (int(value.size()) > 8) throw invalid_argument("Gia tri vuot qua gioi han: " + value);
    return stoi(value);
}

inline ull SToULL(const string& value)
{
    if (!isNum(value)) throw invalid_argument("Dinh dang so nguyen khong hop le: " + value);
    if (value.size() > 18) throw invalid_argument("Gia tri vuot qua gioi qua han: " + value);
    return stoull(value);
}

inline void CheckRoomName(string& value)
{
    if (value.empty()) throw invalid_argument("Ten khong duoc de trong");
    if (value.length() > ROOM_NAME_W) throw invalid_argument("Ten khong duoc qua 20 ky tu: " + value);

    for (unsigned char c : value) {
        if (isalnum(c) || c == ' ' || c == '.' || c == '-' || c == '_') continue;
        throw invalid_argument("Ten chi duoc chua chu cai, so, ki tu .-_ va khoang trang: " + value);
    }

    string tmp;
    for (size_t i = 0; i < value.size(); i++) {
        if (value[i] == ' ' && (i == 0 || value[i-1] == ' ')) continue;
        tmp += value[i];
    }
    if (!tmp.empty()) value = tmp;
}

inline unsigned char ToUpper(unsigned char c)
{
    if (c >= 'A' && c <= 'Z') return c;
    if (c >= 'a' && c <= 'z') return c + 'A' - 'a';
    throw invalid_argument("Khong phai chu cai!");
}

inline unsigned char ToLower(unsigned char c)
{
    if (c >= 'a' && c <= 'z') return c;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
    throw invalid_argument("Khong phai chu cai!");
}

inline void CheckPersonName(string& value)
{
    if (value.empty()) throw invalid_argument("Ten khong duoc de trong");
    if (value.length() > TENANT_NAME_W) throw invalid_argument("Ten khong duoc qua 25 ky tu: " + value);

    for (unsigned char c : value) {
        if (isalpha(c) || c == ' ') continue;
        throw invalid_argument("Ten chi duoc chua chu cai va khoang trang: " + value);
    }

    string tmp;
    for (size_t i = 0; i < value.size(); i++) {
        if (value[i] == ' ' && (i == 0 || value[i-1] == ' ')) continue;
        if (isalpha(value[i])) {
            if (i == 0 || value[i-1] == ' ') tmp += ToUpper(value[i]);
            else tmp += ToLower(value[i]);
        }
        else tmp += value[i];
    }
    if (!tmp.empty()) value = tmp;
}

inline void CheckCCCD(const string& value)
{
    if (!isNum(value)) throw invalid_argument("Dinh dang can cuoc khong hop le: " + value);
    if (value.size() != 12) throw invalid_argument("Dinh dang can cuoc khong dung 12 so: " + value);
}

inline void CheckPhoneNum(const string& value)
{
    if (!isNum(value)) throw invalid_argument("Dinh dang so dien thoai khong hop le: " + value);
    if (value.size() != 10) throw invalid_argument("Dinh dang so dien thoai khong dung 10 so: " + value);
    if (value[0] != '0' || (value[1] != '3' && value[1] != '5' && value[1] != '7' && value[1] != '8' && value[1] != '9'))
        throw invalid_argument("Dinh dang so dien thoai khong hop le: " + value);
}

inline Month SToMonth(const string str)
{
    size_t slash = str.find('/');
    if (slash == string::npos || slash == 0 || slash > 2) 
        throw invalid_argument("Dinh dang thang khong hop le: " + str);
    string month, year;
    if (!isNum(month = str.substr(0, slash))) 
        throw invalid_argument("Dinh dang thang chi duoc chua so: " + month);
    if (!isNum(year = str.substr(slash + 1))) 
        throw invalid_argument("Dinh dang nam chi duoc chua so: " + year);
    int m = stoi(month), y = stoi(year);
    if (m < 1 || m > 12) throw invalid_argument("So thang khong hop le: " + month);
    return Month(m, y);
}

inline Date SToDate(const string str)
{
    size_t firstSlash = str.find('/');
    size_t secondSlash = str.find('/', firstSlash + 1);
    if (firstSlash == string::npos || secondSlash == string::npos
    || firstSlash == 0 || firstSlash > 2
    || secondSlash - firstSlash <= 1 || secondSlash - firstSlash > 3
    || secondSlash == str.size() - 1)
        throw invalid_argument("Dinh dang ngay khong hop le: " + str);
    string day, month, year;
    if (!isNum(day = str.substr(0, firstSlash))) 
        throw invalid_argument("Dinh dang ngay chi duoc chua so: " + day);
    if (!isNum(month = str.substr(firstSlash + 1, secondSlash - firstSlash - 1))) 
        throw invalid_argument("Dinh dang thang chi duoc chua so: " + month);
    if (!isNum(year = str.substr(secondSlash + 1))) 
        throw invalid_argument("Dinh dang nam chi duoc chua so: " + year);
    int d = stoi(day), m = stoi(month), y = stoi(year);
    if (m < 1 || m > 12) throw invalid_argument("So thang khong hop le: " + month);
    if (d < 1 || d > DaysInMonth(m, y)) throw invalid_argument("So thang khong hop le: " + month);
    return Date(d, m, y);
}

inline void CheckId(const string& value, int limit) {
    if (value.empty()) throw invalid_argument("ID khong duoc bo trong");
    if (int(value.length()) > limit) 
        throw invalid_argument("ID khong duoc dai hon " + to_string(limit) + " ki tu: " + value);
    for (unsigned char c : value) {
        if (!isalnum(c)) throw invalid_argument("ID chi duoc chua ki tu chu va so: " + value);
    }
}