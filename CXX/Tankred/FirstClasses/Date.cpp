#include "Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <limits>

// Инициализация статического массива дней в месяцах
const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static int getDaysInMonth(int m, int y) {
    if (m < 1 || m > 12) {
        throw std::invalid_argument("Invalid month value: " + std::to_string(m));
    }
    if (m == 2 && isLeapYear(y)) {
        return 29;
    }
    return daysInMonth[m - 1];
}


bool Date::isValidDate() const {
    if (year < MIN_YEAR || year > MAX_YEAR) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (day < 1 || day > getDaysInMonth(month, year)) {
        return false;
    }
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
    totalDays += day - 1;
    return totalDays;
}

Date Date::daysToDate(long long days) {
    int y = MIN_YEAR;
    while (days >= (isLeapYear(y) ? 366 : 365)) {
        days -= (isLeapYear(y) ? 366 : 365);
        y++;
    }
    int m = 1;
    while (days >= getDaysInMonth(m, y)) {
        days -= getDaysInMonth(m, y);
        m++;
    }
    int d = days + 1;
    if (!Date::isValidDateHelper(d, m, y)) {
        throw std::out_of_range("Date out of range: " + std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y));
    }
    return Date(d, m, y);
}

bool Date::isValidDateHelper(int d, int m, int y) {
    if (y < MIN_YEAR || y > MAX_YEAR) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > getDaysInMonth(m, y)) return false;
    return true;
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (!isValidDate()) {
        throw std::out_of_range("Invalid date: " + std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y));
    }
    normalize();
}


Date::Date(const std::string& dateStr, const std::string& format) {
    day = 1;
    month = 1;
    year = MIN_YEAR;
    std::stringstream ss(dateStr);
    int d, m, y;
    char delim1, delim2;


    try {
        if (format == "DD.MM.YYYY") {
            ss >> d >> delim1 >> m >> delim2 >> y;
        } else if (format == "MM/DD/YYYY") {
            ss >> m >> delim1 >> d >> delim2 >> y;
        } else if (format == "YYYY-MM-DD") {
            ss >> y >> delim1 >> m >> delim2 >> d;
        } else {
            throw std::invalid_argument("Unsupported date format: " + format);
        }

    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Error parsing date string: " + std::string(e.what()));
    }

    if (!isValidDate()) {
        throw std::out_of_range("Date out of range: " + std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y));
    }
    day = d;
    month = m;
    year = y;

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
        ss << std::setfill('0') << std::setw(2) << day << "."
           << std::setfill('0') << std::setw(2) << month << "."
           << std::setfill('0') << std::setw(4) << year;
    }
    return ss.str();
}

void Date::normalize() {
    *this = daysToDate(toDays());
}

Date& Date::change(int days) {
    long long totalDays = toDays() + days;
    if (totalDays < 0 || totalDays > std::numeric_limits<long long>::max()) {
        throw std::out_of_range("Resulting date out of range.");
    }
    *this = daysToDate(totalDays);
    return *this;
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


int Date::getYear() const {
    return year;
}

int Date::getMonth() const {
    return month;
}

int Date::getDay() const {
    return day;
}


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
    os << date.toFormat("DD.MM.YYYY");
    return os;
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
