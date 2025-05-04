#include "Date.h"
#include <iostream>
#include <cwchar>
#include <clocale>
#include <fcntl.h>
#ifdef _WIN32
    #include <io.h>
#endif

int main() {
    // Настраиваем локаль для корректного отображения кириллицы
    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT); // Для Windows
    #else
        std::setlocale(LC_ALL, ""); // Для UNIX-подобных систем
    #endif
    
    // Создание даты различными способами
    Date date1(15, 5, 2023);
    Date date2("23.11.2023", "DD.MM.YYYY");
    Date date3("2023-12-31", "YYYY-MM-DD");
    
    // Вывод дат в разных форматах с использованием wprintf
    wprintf(L"Дата 1: %ls\n", date1.toWString().c_str());
    wprintf(L"Дата 1 (формат MM/DD/YYYY): %ls\n", date1.toWString("MM/DD/YYYY").c_str());
    wprintf(L"Дата 1 (формат YYYY-MM-DD): %ls\n", date1.toWString("YYYY-MM-DD").c_str());
    
    wprintf(L"Дата 2: %ls\n", date2.toWString().c_str());
    wprintf(L"Дата 3: %ls\n", date3.toWString().c_str());
    
    // Изменение даты на заданное количество дней
    Date date4 = date1;
    date4.addDays(10);
    wprintf(L"Дата 1 + 10 дней: %ls\n", date4.toWString().c_str());
    
    date4.addDays(-20);
    wprintf(L"Дата 1 + 10 - 20 дней: %ls\n", date4.toWString().c_str());
    
    // Вычисление разности дат
    int diff = date3 - date1;
    wprintf(L"Количество дней между датами 3 и 1: %d\n", diff);
    
    // Проверка операторов сравнения
    wprintf(L"Дата 1 < Дата 2: %ls\n", (date1 < date2 ? L"Да" : L"Нет"));
    wprintf(L"Дата 2 > Дата 3: %ls\n", (date2 > date3 ? L"Да" : L"Нет"));
    
    return 0;
}