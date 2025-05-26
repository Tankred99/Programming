#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <string>

class Date {
private:
    int year;
    int month;
    int day;

    bool isLeap(int year) const;
    int daysInMonth(int year, int month) const;
    long long daysSinceEpoch(int year, int month, int day) const; 


public:
    //Constructors
    Date(int year, int month, int day);
    Date(const std::wstring& dateStr); 


    // Getters
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }


    // Methods
    void addDays(long long days);  
    std::wstring format(const std::wstring& formatStr) const; 


    // Operator Overloads
    Date operator-(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    friend std::wostream& operator<<(std::wostream& os, const Date& date); 
};

#endif
