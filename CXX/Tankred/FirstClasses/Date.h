#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <cwchar>
#include <stdexcept>  // Для std::invalid_argument и std::out_of_range

class Date {
private:
    int day;
    int month;
    int year;

    // Минимальная допустимая дата
    static const int MIN_YEAR = 1900;
    static const int MIN_MONTH = 1;
    static const int MIN_DAY = 1;

    // Массив дней в месяцах (учитывая не високосный год)
    static const int daysInMonth[];

    // Проверка на високосный год
    bool isLeapYear(int y) const;

    // Получить количество дней в указанном месяце
    int getDaysInMonth(int m, int y) const;

    // Нормализовать дату (корректировать неправильные значения)
    void normalize();

    // Проверка, что дата больше или равна минимальной допустимой дате
    bool isValidDate() const;

public:
    // Конструктор по умолчанию (текущая дата - условно 1.1.2000)
    Date();

    // Конструктор с параметрами
    Date(int d, int m, int y);

    // Конструктор из строки (поддерживает разные форматы)
    // Выбрасывает std::invalid_argument при ошибке парсинга
    Date(const std::string& dateStr, const std::string& format = "DD.MM.YYYY");

    // Получить строковое представление даты в разных форматах
    std::string toString(const std::string& format = "DD.MM.YYYY") const;

    // Получить представление даты в формате широких символов
    std::wstring toWString(const std::string& format = "DD.MM.YYYY") const;

    // Методы доступа к полям
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Методы установки значений с нормализацией
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    // Изменить дату на заданное количество дней
    Date& addDays(int days); //+=

    // Получить количество дней от 1.1.1900 года до текущей даты
    long long totalDays() const;

    // Перегрузка оператора вычитания для нахождения разности дат
    long long operator-(const Date& other) const;

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