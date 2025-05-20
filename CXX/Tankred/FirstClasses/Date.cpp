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
    std::cout << "isValidDate called for: " << *this << std::endl;
    if (year < MIN_YEAR || year > MAX_YEAR) {
        std::cerr << "Error: Year out of range: " << year << std::endl;
        return false;
    }
    try {
        if (month < 1 || month > 12) {
            std::cerr << "Error: Month out of range: " << month << std::endl;
            return false;
        }
        if (day < 1 || day > getDaysInMonth(month, year)) {
            std::cerr << "Error: Day out of range: " << day << " for month/year: " << month << "/" << year << std::endl;
            return false;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Exception in isValidDate: " << e.what() << std::endl;
        return false;
    }
    return true;
}

long long Date::toDays() const {
    std::cout << "toDays called for: " << *this << std::endl;
    if (!isValidDate()) {
        throw std::invalid_argument("Invalid date in toDays()"); 
    }
    long long totalDays = 0;
    long long daysSince1900 = (year - MIN_YEAR) * 365LL;
    for (int y = MIN_YEAR; y < year; ++y) {
        if (isLeapYear(y)) daysSince1900++;
    }
    totalDays = daysSince1900;
    for (int m = 1; m < month; ++m) {
        totalDays += getDaysInMonth(m, year);
    }
    totalDays += day - 1;
    std::cout << "toDays returning: " << totalDays << std::endl;
    return totalDays;
}


Date Date::daysToDate(long long days) {
    std::cout << "daysToDate called with days: " << days << std::endl;
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

    try {
        if (!Date::isValidDateHelper(d, m, y)) {
            throw std::out_of_range("Date out of range in daysToDate"); 
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Exception in daysToDate: " << e.what() << std::endl;
        throw; 
    }
    std::cout << "daysToDate returning: " << d << "/" << m << "/" << y << std::endl;
    return Date(d, m, y);
}

bool Date::isValidDateHelper(int d, int m, int y) {
    if (y < MIN_YEAR || y > MAX_YEAR) return false;
    if (m < 1 || m > 12) return false;
    try {
        if (d < 1 || d > getDaysInMonth(m, y)) return false;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Exception in isValidDateHelper: " << e.what() << std::endl;
        return false;
    }
    return true;
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    std::cout << "Date constructor called with: " << d << "/" << m << "/" << y << std::endl;
    if (!isValidDate()) {
        throw std::out_of_range("Invalid date: " + std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y));
    }
    normalize();
}


Date::Date(const std::string& dateStr, const std::string& format) {
    std::cout << "Date constructor (string) called with: " << dateStr << ", format: " << format << std::endl;
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
        std::cerr << "Error parsing date string: " << e.what() << std::endl;
        throw std::invalid_argument("Error parsing date string: " + std::string(e.what()));
    }

    if (!isValidDateHelper(d, m, y)) {
        std::cerr << "Error: Invalid date components: " << d << "/" << m << "/" << y << std::endl;
        throw std::out_of_range("Date out of range: " + std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y));
    }
    day = d;
    month = m;
    year = y;

    normalize();
}


std::string Date::toFormat(const std::string& format) const {
    std::cout << "toFormat called with format: " << format << std::endl;
    std::stringstream ss;
    if (!isValidDate()) {
        throw std::invalid_argument("Invalid date in toFormat()"); 
    }
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
    std::cout << "toFormat returning: " << ss.str() << std::endl;
    return ss.str();
}

void Date::normalize() {
    std::cout << "normalize called for: " << *this << std::endl;
    try {
        *this = daysToDate(toDays());
    } catch (const std::exception& e) {
        std::cerr << "Exception in normalize(): " << e.what() << std::endl;
    }
    std::cout << "normalize returning: " << *this << std::endl;
}

Date& Date::change(int days) {
    std::cout << "change called with days: " << days << ", current date: " << *this << std::endl;
    try {
        long long totalDays = toDays() + days;
        if (totalDays < 0 || totalDays > std::numeric_limits<long long>::max()) {
            throw std::out_of_range("Resulting date out of range.");
        }
        *this = daysToDate(totalDays);
    } catch (const std::exception& e) {
        std::cerr << "Exception in change(): " << e.what() << std::endl;
    }
    std::cout << "change returning: " << *this << std::endl;
    return *this;
}


long long Date::totalDays() const {
    return toDays();
}

long long Date::operator-(const Date& other) const {
    std::cout << "operator- called with: " << *this << " - " << other << std::endl;
    long long diff = toDays() - other.toDays();
    std::cout << "operator- returning: " << diff << std::endl;
    return diff;
}

bool Date::operator==(const Date& other) const {
    std::cout << "operator== called with: " << *this << " == " << other << std::endl;
    bool isEqual = (day == other.day && month == other.month && year == other.year);
    std::cout << "operator== returning: " << isEqual << std::endl;
    return isEqual;
}

bool Date::operator!=(const Date& other) const {
    bool isNotEqual = !(*this == other);
    std::cout << "operator!= returning: " << isNotEqual << std::endl;
    return isNotEqual;
}

bool Date::operator<(const Date& other) const {
    std::cout << "operator< called with: " << *this << " < " << other << std::endl;
    bool isLess = toDays() < other.toDays();
    std::cout << "operator< returning: " << isLess << std::endl;
    return isLess;
}

bool Date::operator<=(const Date& other) const {
    bool isLessOrEqual = (*this < other) || (*this == other);
    std::cout << "operator<= returning: " << isLessOrEqual << std::endl;
    return isLessOrEqual;
}

bool Date::operator>(const Date& other) const {
    bool isGreater = !(*this <= other);
    std::cout << "operator> returning: " << isGreater << std::endl;
    return isGreater;
}

bool Date::operator>=(const Date& other) const {
    bool isGreaterOrEqual = !(*this < other);
    std::cout << "operator>= returning: " << isGreaterOrEqual << std::endl;
    return isGreaterOrEqual;
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
