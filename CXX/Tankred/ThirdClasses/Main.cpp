//Main.cpp

#include "MyString.h"
#include <iostream>
#include <clocale>
#include <cwchar> // For wide character functions

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

using namespace std;

int main() {
    try {
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_U8TEXT);
#else
        setlocale(LC_ALL, "");
#endif

        wchar_t inputStr[256];
        wcout << L"Введите строку: ";
        wcin.getline(inputStr, 256);

        MyString myString(inputStr);

        while (true) {
            wcout << L"\nВыберите действие:" << endl;
            wcout << L"1. Вывести длину строки" << endl;
            wcout << L"2. Найти подстроку" << endl;
            wcout << L"3. Скопировать часть строки" << endl;
            wcout << L"4. Заменить подстроку" << endl;
            wcout << L"5. Удалить подстроку" << endl;
            wcout << L"6. Вывести строку" << endl;
            wcout << L"7. Выход" << endl;

            int choice;
            wcin >> choice;
            wcin.ignore(); // Consume the newline character

            switch (choice) {
                case 1: {
                    wcout << L"Длина строки: " << myString.Length() << endl;
                    break;
                }
                case 2: {
                    wchar_t subStr[256];
                    wcout << L"Введите подстроку для поиска: ";
                    wcin.getline(subStr, 256);
                    int index = myString.FindSub(subStr);
                    if (index != -1) {
                        wcout << L"Подстрока найдена по индексу: " << index << endl;
                    } else {
                        wcout << L"Подстрока не найдена." << endl;
                    }
                    break;
                }
                case 3: {
                    int start, count;
                    wcout << L"Введите начальный индекс: ";
                    wcin >> start;
                    wcout << L"Введите количество символов для копирования: ";
                    wcin >> count;
                    wcin.ignore();
                    try {
                        MyString subStr = myString.CopyLine(start, count);
                        wcout << L"Скопированная подстрока: " << subStr << endl;
                    } catch (const std::out_of_range& e) {
                        wcout << L"Ошибка: " << e.what() << endl;
                    }
                    break;
                }
                case 4: {
                    int start, count;
                    wchar_t newSubStr[256];
                    wcout << L"Введите начальный индекс: ";
                    wcin >> start;
                    wcout << L"Введите количество символов для замены: ";
                    wcin >> count;
                    wcin.ignore();
                    wcout << L"Введите новую подстроку: ";
                    wcin.getline(newSubStr, 256);
                    try {
                        myString.ReplaceSub(MyString(newSubStr), start, count);
                        wcout << L"Строка после замены: " << myString << endl;
                    } catch (const std::out_of_range& e) {
                        wcout << L"Ошибка: " << e.what() << endl;
                    }
                    break;
                }
                case 5: {
                    int start, count;
                    wcout << L"Введите начальный индекс: ";
                    wcin >> start;
                    wcout << L"Введите количество символов для удаления: ";
                    wcin >> count;
                    wcin.ignore();
                    try {
                        myString.DeleteSub(start, count);
                        wcout << L"Строка после удаления: " << myString << endl;
                    } catch (const std::out_of_range& e) {
                        wcout << L"Ошибка: " << e.what() << endl;
                    }
                    break;
                }
                case 6: {
                    wcout << L"Текущая строка: " << myString << endl;
                    break;
                }
                case 7: {
                    wcout << L"Выход из программы." << endl;
                    return 0;
                }
                default: {
                    wcout << L"Неверный выбор. Попробуйте еще раз." << endl;
                    break;
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
}