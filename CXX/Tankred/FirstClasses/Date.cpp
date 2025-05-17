// Date.cpp
#include "Date.h"
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cwchar>
#include <stdexcept>
#include <limits> // Added for numeric_limits

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

bool Date::isValidDate() const {
    if (year < MIN_YEAR || year > MAX_YEAR) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(month, year)) return false;
    return true;
}


long long Date::toDays() const {
    long long totalDays = 0;
    for (int y = MIN_YEAR; y < year; ++y) {
        totalDays += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < month; ++m) {
        totalDays += getDaysInMonth(m, year);
    }
    totalDays += day -1;
    return totalDays;
}

Date Date::daysToDate(long long days) {
    Date tempDate(1, 1, MIN_YEAR); // Create a temporary Date object
    int y = MIN_YEAR;
    while (days >= (tempDate.isLeapYear(y) ? 366 : 365)) {
        days -= (tempDate.isLeapYear(y) ? 366 : 365);
        y++;
    }
    int m = 1;
    while (days >= tempDate.getDaysInMonth(m, y)) {
        days -= tempDate.getDaysInMonth(m, y);
        m++;
    }
    int d = days + 1;
    if (!tempDate.isValidDateHelper(d, m, y)) {
        throw std::out_of_range("Date out of range");
    }
    return Date(d, m, y);
}
bool Date::isValidDateHelper(int d, int m, int y) const{
    if (y < MIN_YEAR || y > MAX_YEAR) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > getDaysInMonth(m, y)) return false;
    return true;
}

void Date::normalize() {
    long long totalDays = toDays();
    *this = daysToDate(totalDays);
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (!isValidDateHelper(d,m,y)){
        throw std::out_of_range("Date out of range");
    }
    normalize();
}

Date::Date(const std::string& dateStr, const std::string& format) {
    // По умолчанию устанавливаем дату 1.1.1900
    day = 1;
    month = 1;
    year = MIN_YEAR;

    try {
        // Парсим строку в зависимости от формата
        if (format == "DD.MM.YYYY" && dateStr.length() >= 10) {
            day = std::stoi(dateStr.substr(0, 2));
            month = std::stoi(dateStr.substr(3, 2));
            year = std::stoi(dateStr.substr(6, 4));
        } else if (format == "MM/DD/YYYY" && dateStr.length() >= 10) {
            month = std::stoi(dateStr.substr(0, 2));
            day = std::stoi(dateStr.substr(3, 2));
            year = std::stoi(dateStr.substr(6, 4));
        } else if (format == "YYYY-MM-DD" && dateStr.length() >= 10) {
            year = std::stoi(dateStr.substr(0, 4));
            month = std::stoi(dateStr.substr(5, 2));
            day = std::stoi(dateStr.substr(8, 2));
        } else {
            throw std::invalid_argument("Unsupported date format or invalid date string");
        }
    } catch (const std::exception& e) {
        throw std::invalid_argument("Error parsing date string: " + std::string(e.what()));
    }

    if (!isValidDate()) {
        throw std::out_of_range("Date out of range");
    }
    normalize();
}

std::string Date::toFormat(const std::string& format) const {
    std::stringstream ss;

    if (format == "DD.MM.YYYY") {
        ss << std::setfill('0') << std::setw(2) << day << "."
           << std::setfill('0') << std::setw(2) << month << "."
           << std::setfill('0') << std::setw(4) << year;
    } else if (format == "MM/DD/YYYY") {
        ss << std::setfill('0') << std::setw(2) << month << "/"
           << std::setfill('0') << std::setw(2) << day << "/"
           << std::setfill('0') << std::setw(4) << year;
    } else if (format == "YYYY-MM-DD") {
        ss << std::setfill('0') << std::setw(4) << year << "-"
           << std::setfill('0') << std::setw(2) << month << "-"
           << std::setfill('0') << std::setw(2) << day;
    } else {
        // По умолчанию используем формат DD.MM.YYYY если передан некорректный формат
        ss << std::setfill('0') << std::setw(2) << day << "."
           << std::setfill('0') << std::setw(2) << month << "."
           << std::setfill('0') << std::setw(4) << year;
    }

    return ss.str();
}

Date& Date::change(int days) {
    long long totalDays = toDays() + days;
    if (totalDays < 0 || totalDays > std::numeric_limits<long long>::max()){
        throw std::out_of_range("Resulting date out of range");
    }
    *this = daysToDate(totalDays);
    return *this;
}

Date& Date::addDays(int days) {
    return change(days); //Added addDays method.  It's just an alias for change.
}

long long Date::totalDays() const {
    return toDays();
}

long long Date::operator-(const Date& other) const {
    return toDays() - other.toDays();
}

bool Date::operator==(const Date& other) const {
    return (day == other.day && month == other.month && year == other.year);
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    return toDays() < other.toDays();
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
    std::string str = toFormat(format);
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
    os << date.toFormat("DD.MM.YYYY"); // Default format
    return os;
}

std::wostream& operator<<(std::wostream& wos, const Date& date) {
    wos << date.toWString("DD.MM.YYYY"); // Default format
    return wos;
}

std::istream& operator>>(std::istream& is, Date& date) {
    std::string dateStr;
    char delimiter;
    int d, m, y;

    is >> d >> delimiter >> m >> delimiter >> y;
    if (is) {
      try {
        date = Date(d, m, y);
      }
      catch (const std::out_of_range& e) {
          is.setstate(std::ios_base::failbit);
      }
    }
    return is;
}
