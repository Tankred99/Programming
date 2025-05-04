#include "Date.h"
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cwchar>

// Инициализация статического массива дней в месяцах
const int Date::daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::getDaysInMonth(int m, int y) const {
    if (m == 2 && isLeapYear(y))
        return 29;
    return daysInMonth[m - 1];
}

void Date::normalize() {
    // Если день больше максимального для текущего месяца
    while (day > getDaysInMonth(month, year)) {
        day -= getDaysInMonth(month, year);
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    
    // Если день меньше минимального (отрицательный)
    while (day <= 0) {
        month--;
        if (month < 1) {
            month = 12;
            year--;
        }
        day += getDaysInMonth(month, year);
    }
    
    // Обработка переполнения месяцев
    while (month > 12) {
        month -= 12;
        year++;
    }
    while (month < 1) {
        month += 12;
        year--;
    }
}

Date::Date() : day(1), month(1), year(2000) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    normalize();
}

Date::Date(const std::string& dateStr, const std::string& format) {
    // По умолчанию устанавливаем дату 1.1.2000
    day = 1;
    month = 1;
    year = 2000;
    
    // Парсим строку в зависимости от формата
    if (format == "DD.MM.YYYY" && dateStr.length() >= 10) {
        day = std::stoi(dateStr.substr(0, 2));
        month = std::stoi(dateStr.substr(3, 2));
        year = std::stoi(dateStr.substr(6, 4));
    } 
    else if (format == "MM/DD/YYYY" && dateStr.length() >= 10) {
        month = std::stoi(dateStr.substr(0, 2));
        day = std::stoi(dateStr.substr(3, 2));
        year = std::stoi(dateStr.substr(6, 4));
    }
    else if (format == "YYYY-MM-DD" && dateStr.length() >= 10) {
        year = std::stoi(dateStr.substr(0, 4));
        month = std::stoi(dateStr.substr(5, 2));
        day = std::stoi(dateStr.substr(8, 2));
    }
    
    normalize();
}

std::string Date::toString(const std::string& format) const {
    std::stringstream ss;
    
    if (format == "DD.MM.YYYY") {
        ss << std::setfill('0') << std::setw(2) << day << "." 
           << std::setfill('0') << std::setw(2) << month << "." 
           << std::setfill('0') << std::setw(4) << year;
    } 
    else if (format == "MM/DD/YYYY") {
        ss << std::setfill('0') << std::setw(2) << month << "/" 
           << std::setfill('0') << std::setw(2) << day << "/" 
           << std::setfill('0') << std::setw(4) << year;
    }
    else if (format == "YYYY-MM-DD") {
        ss << std::setfill('0') << std::setw(4) << year << "-" 
           << std::setfill('0') << std::setw(2) << month << "-" 
           << std::setfill('0') << std::setw(2) << day;
    }
    
    return ss.str();
}

Date& Date::addDays(int days) {
    day += days;
    normalize();
    return *this;
}

long long Date::totalDays() const {
    long long totalDays = 0;
    
    // Добавляем дни за предыдущие годы
    for (int y = 0; y < year; y++) {
        totalDays += isLeapYear(y) ? 366 : 365;
    }
    
    // Добавляем дни за предыдущие месяцы текущего года
    for (int m = 1; m < month; m++) {
        totalDays += getDaysInMonth(m, year);
    }
    
    // Добавляем дни текущего месяца
    totalDays += day;
    
    return totalDays;
}

int Date::operator-(const Date& other) const {
    return static_cast<int>(totalDays() - other.totalDays());
}

bool Date::operator==(const Date& other) const {
    return (day == other.day && month == other.month && year == other.year);
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return (*this < other) || (*this == other);
}

bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

std::wstring Date::toWString(const std::string& format) const {
    // Сначала получаем строку в обычном формате
    std::string str = toString(format);
    
    // Конвертируем её в широкую строку
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}

std::wostream& operator<<(std::wostream& wos, const Date& date) {
    wos << date.toWString();
    return wos;
}