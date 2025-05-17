#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

class Date {
private:
    int day;
    int month;
    int year;

    static const int MIN_YEAR = 1900;
    static const int MAX_YEAR = 9999;

    bool isValidDate() const;
    static bool isValidDateHelper(int d, int m, int y);
    long long toDays() const;
    static Date daysToDate(long long days);

public:
    Date();
    Date(int d, int m, int y);
    Date(const std::string& dateStr, const std::string& format = "DD.MM.YYYY");
    std::string toFormat(const std::string& format = "DD.MM.YYYY") const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    Date& change(int days);
    long long totalDays() const;
    long long operator-(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
    void normalize();
};

#endif // DATE_H
