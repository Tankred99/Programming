#include "date.h"
#include <locale>
#include <codecvt>
#include <iomanip> 
#include <string>

bool Date::isLeap(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int year, int month) const {
    if (month == 2) {
        return isLeap(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

long long Date::daysSinceEpoch(int year, int month, int day) const { 
    long long days = 0;
    for (int y = 1; y < year; ++y) {
        days += isLeap(y) ? 366 : 365;
    }
    for (int m = 1; m < month; ++m) {
        days += daysInMonth(year, m);
    }
    days += day;
    return days;
}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {
    if (month < 1 || month > 12 || day < 1 || day > daysInMonth(year, month)) {
        std::wstringstream ss;
        ss << L"Invalid date: " << year << L"-" << std::setw(2) << std::setfill(L'0') << month << L"-" << std::setw(2) << std::setfill(L'0') << day;
        throw std::invalid_argument(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(ss.str()));
    }
}

Date::Date(const std::wstring& dateStr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string dateStrUTF8 = converter.to_bytes(dateStr);

    std::regex dateRegex("(\\d{4})-(\\d{2})-(\\d{2})"); 
    std::smatch match;
    if (!std::regex_match(dateStrUTF8, match, dateRegex)) {
        std::wstring err_msg = L"Invalid date format: " + dateStr;
        throw std::invalid_argument(converter.from_bytes(err_msg)); 
    }
    year = std::stoi(match[1]);
    month = std::stoi(match[2]);
    day = std::stoi(match[3]);
    if (month < 1 || month > 12 || day < 1 || day > daysInMonth(year, month)) {
        std::wstringstream ss;
        ss << L"Invalid date: " << year << L"-" << std::setw(2) << std::setfill(L'0') << month << L"-" << std::setw(2) << std::setfill(L'0') << day;
        throw std::invalid_argument(converter.from_bytes(ss.str())); 
    }
}

void Date::addDays(long long days) { 
    long long currentDays = daysSinceEpoch(year, month, day);
    long long newDays = currentDays + days;
    int y = year;

    while (newDays <= 0) {
      int daysInYear = isLeap(y - 1) ? 366 : 365;
      newDays += daysInYear;
      y--;
    }
    while (true) {
        int daysInYear = isLeap(y) ? 366 : 365;
        if (newDays <= daysInYear) break;
        newDays -= daysInYear;
        y++;
    }
    year = y;

    int m = month;
    while (true) {
        int daysInMonth = this->daysInMonth(year, m);
        if (newDays <= daysInMonth) break;
        newDays -= daysInMonth;
        m++;
    }
    month = m;
    day = newDays;

}


std::wstring Date::format(const std::wstring& formatStr) const {
    std::wstringstream ss;
    ss << formatStr;

    std::wstring formattedDate;
    wchar_t c;
    while (ss >> c) {
        if (c == L'Y') {
            formattedDate += std::to_wstring(year);
        } else if (c == L'M') {
            formattedDate += (month < 10 ? L"0" : L"") + std::to_wstring(month);
        } else if (c == L'D') {
            formattedDate += (day < 10 ? L"0" : L"") + std::to_wstring(day);
        } else {
            formattedDate += c;
        }
    }

    return formattedDate;
}

Date Date::operator-(const Date& other) const {
    long long diff = daysSinceEpoch(year, month, day) - other.daysSinceEpoch(other.year, other.month, other.day);
    return Date(1, 1, diff); 
}

bool Date::operator<(const Date& other) const {
    return daysSinceEpoch(year, month, day) < other.daysSinceEpoch(other.year, other.month, other.day);
}

bool Date::operator>(const Date& other) const {
    return daysSinceEpoch(year, month, day) > other.daysSinceEpoch(other.year, other.month, other.day);
}

bool Date::operator==(const Date& other) const {
    return daysSinceEpoch(year, month, day) == other.daysSinceEpoch(other.year, other.month, other.day);
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>=(const Date& other) const {
    return *this > other || *this == other;
}

std::wostream& operator<<(std::wostream& os, const Date& date) {
    os << date.format(L"YYYY-MM-DD");
    return os;
}
