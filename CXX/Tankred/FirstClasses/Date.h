#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <cwchar>

class Date {
private:
    int day;
    int month;
    int year;
    
    // Массив дней в месяцах (учитывая не високосный год)
    static const int daysInMonth[];
    
    // Проверка на високосный год
    bool isLeapYear(int y) const;
    
    // Получить количество дней в указанном месяце
    int getDaysInMonth(int m, int y) const;
    
    // Нормализовать дату (корректировать неправильные значения)
    void normalize();
    
public:
    // Конструктор по умолчанию (текущая дата - условно 1.1.2000)
    Date();
    
    // Конструктор с параметрами
    Date(int d, int m, int y);
    
    // Конструктор из строки (поддерживает разные форматы)
    Date(const std::string& dateStr, const std::string& format = "DD.MM.YYYY");
    
    // Получить строковое представление даты в разных форматах
    std::string toString(const std::string& format = "DD.MM.YYYY") const;
    
    // Получить представление даты в формате широких символов
    std::wstring toWString(const std::string& format = "DD.MM.YYYY") const;
    
    // Изменить дату на заданное количество дней
    Date& addDays(int days);
    
    // Получить количество дней от 1.1.0 года до текущей даты
    long long totalDays() const;
    
    // Перегрузка оператора вычитания для нахождения разности дат
    int operator-(const Date& other) const;
    
    // Перегрузка операторов сравнения
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    
    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    
    // Перегрузка оператора вывода для широких потоков
    friend std::wostream& operator<<(std::wostream& wos, const Date& date);
};

#endif // DATE_H