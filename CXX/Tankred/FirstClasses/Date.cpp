// Date.cpp
#include "Date.h"
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cwchar>
#include <stdexcept>

// Инициализация статического массива дней в месяцах
const int Date::daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::getDaysInMonth(int m, int y) const {
    if (m < 1 || m > 12) {
        throw std::invalid_argument("Invalid month value");
    }
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
    
    try {
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
        else {
            throw std::invalid_argument("Unsupported date format or invalid date string");
        }
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Error parsing date string: " + std::string(e.what()));
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
    else {
        // По умолчанию используем формат DD.MM.YYYY если передан некорректный формат
        ss << std::setfill('0') << std::setw(2) << day << "." 
           << std::setfill('0') << std::setw(2) << month << "." 
           << std::setfill('0') << std::setw(4) << year;
    }
    
    return ss.str();
}

Date& Date::addDays(int days) {
    day += days;
    normalize();
    return *this;
}

long long Date::totalDays() const {
    // Используем алгоритм Джулианского дня для более точного вычисления
    // https://en.wikipedia.org/wiki/Julian_day
    
    int y = year;
    int m = month;
    int d = day;
    
    // Январь и февраль считаются как 13-й и 14-й месяцы предыдущего года
    if (m <= 2) {
        m += 12;
        y -= 1;
    }
    
    // Расчет Джулианского дня
    long long jd = static_cast<long long>(365.25 * (y + 4716)) + 
                  static_cast<long long>(30.6001 * (m + 1)) + 
                  d - 1524;
    
    // Коррекция для Григорианского календаря
    if (y > 1582 || (y == 1582 && m > 10) || (y == 1582 && m == 10 && d >= 15)) {
        int a = y / 100;
        int b = 2 - a + (a / 4);
        jd += b;
    }
    
    // Вычитаем базовую дату (1.1.1900)
    // Джулианский день для 1.1.1900: 2415021
    return jd - 2415021;
}

long long Date::operator-(const Date& other) const {
    return totalDays() - other.totalDays();
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

// Получение года, месяца и дня
int Date::getYear() const {
    return year;
}

int Date::getMonth() const {
    return month;
}

int Date::getDay() const {
    return day;
}

// Установка значений для года, месяца и дня
void Date::setYear(int y) {
    year = y;
    normalize();
}

void Date::setMonth(int m) {
    month = m;
    normalize();
}

void Date::setDay(int d) {
    day = d;
    normalize();
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}

std::wostream& operator<<(std::wostream& wos, const Date& date) {
    wos << date.toWString();
    return wos;
}