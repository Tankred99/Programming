// main.cpp
#include <iostream>
#include <string>
#include "CString.h"
#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    try {
        // Тестирование конструктора по умолчанию
        CString str1(0);
        std::cout << "Пустая строка: " << str1 << std::endl;

        // Тестирование конструктора с параметром
        str1 = "I am going to the mall ";
        std::cout << "Строка после присваивания: " << str1 << std::endl;

        // Тестирование метода ReplaceSubStr
        str1.ReplaceSubStr("mall", "school");
        std::cout << "Строка после замены подстроки: " << str1 << std::endl;

        // Тестирование метода FindSubStr
        std::string::size_type pos = str1.FindSubStr("school");
        if (pos != std::string::npos) {
            // Тестирование метода RemoveSubStr
            CString str2 = str1.GetString().c_str();
            str1.RemoveSubStr("school");
            std::cout << "Строка после удаления подстроки: " << str1 << std::endl;
        }
        else {
            std::cout << "Подстрока 'school' не найдена." << std::endl;
        }


        // Тестирование оператора +
        str1 = str1 + "home";
        std::cout << "Строка после конкатенации: " << str1 << std::endl;

        // Тестирование конструктора копирования
        CString str3(str1);
        std::cout << "Копия строки: " << str3 << std::endl;

        // Тестирование оператора =
        CString str4;
        str4 = str1;
        std::cout << "Строка после присваивания: " << str4 << std::endl;

        // Тестирование метода RemoveSubStr
        str1.RemoveSubStr(" anyone");
        std::cout << "Строка после удаления подстроки: " << str1 << std::endl;

        // Тестирование оператора []
        std::cout << "Четвертый символ строки: " << str1[3] << std::endl;

        // Тестирование метода StrCopy
        CString str5 = str1.StrCopy(2, 8);
        std::cout << "Копия подстроки: " << str5 << std::endl;

        // Тестирование метода getString
        std::string arr = str1.GetString();
        std::cout << "Полученная строка: " << arr << std::endl;

        // Тестирование метода GetLength
        std::cout << "Длина строки: " << str1.GetLength() << std::endl;

        // Тестирование оператора >>
        CString j;
        std::cout << "Введите строку: ";
        std::cin >> j;
        std::cout << "Введенная строка: " << j << std::endl;

        // Тестирование динамического создания объекта
        CString* h = new CString;
        *h = "papapapypypypupuptpp";
        std::cout << "Динамически созданная строка: " << *h << std::endl;
        delete h;

        // Тестирование исключения
        try{
            str1[100];
        }
        catch (const std::out_of_range& e){
            std::cout << "Поймано исключение: " << e.what() << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}