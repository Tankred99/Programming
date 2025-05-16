#include "Date.h"
#include "DateTester.h"
#include <iostream>
#include <cwchar>
#include <clocale>
#include <fcntl.h>
#include <exception>
#include <string>
#ifdef _WIN32
    #include <io.h>
#endif

void runManualTests() {
    std::wcout << L"\n========= Ручные тесты =========\n" << std::endl;
    
    // Создание даты различными способами
    Date date1(15, 5, 2023);
    Date date2("23.11.2023", "DD.MM.YYYY");
    Date date3("2023-12-31", "YYYY-MM-DD");
    
    // Вывод дат в разных форматах
    std::wcout << L"Дата 1: " << date1 << std::endl;
    std::wcout << L"Дата 1 (формат MM/DD/YYYY): " << date1.toWString("MM/DD/YYYY") << std::endl;
    std::wcout << L"Дата 1 (формат YYYY-MM-DD): " << date1.toWString("YYYY-MM-DD") << std::endl;
    
    std::wcout << L"Дата 2: " << date2 << std::endl;
    std::wcout << L"Дата 3: " << date3 << std::endl;
    
    // Демонстрация работы с методами доступа
    std::wcout << L"День даты 1: " << date1.getDay() << std::endl;
    std::wcout << L"Месяц даты 1: " << date1.getMonth() << std::endl;
    std::wcout << L"Год даты 1: " << date1.getYear() << std::endl;
    
    // Изменение даты на заданное количество дней
    Date date4 = date1;
    date4.addDays(10);
    std::wcout << L"Дата 1 + 10 дней: " << date4 << std::endl;
    
    date4.addDays(-20);
    std::wcout << L"Дата 1 + 10 - 20 дней: " << date4 << std::endl;
    
    // Вычисление разности дат
    long long diff = date3 - date1;
    std::wcout << L"Количество дней между датами 3 и 1: " << diff << std::endl;
    
    // Демонстрация отсчета от 1.1.1900
    Date baseDate(1, 1, 1900);
    long long daysFromBase1 = date1 - baseDate;
    long long daysFromBase3 = date3 - baseDate;
    std::wcout << L"Дней от 01.01.1900 до даты 1: " << daysFromBase1 << std::endl;
    std::wcout << L"Дней от 01.01.1900 до даты 3: " << daysFromBase3 << std::endl;
    
    // Проверка операторов сравнения
    std::wcout << L"Дата 1 < Дата 2: " << (date1 < date2 ? L"Да" : L"Нет") << std::endl;
    std::wcout << L"Дата 2 > Дата 3: " << (date2 > date3 ? L"Да" : L"Нет") << std::endl;
    
    // Демонстрация обработки ошибок
    try {
        Date invalidDate("32.13.2023", "DD.MM.YYYY");
    } catch (const std::exception& e) {
        std::wcout << L"Перехвачено исключение: " << e.what() << std::endl;
    }
    
    // Демонстрация работы с датами до 1900 года
    Date historicalDate(1, 1, 1800);
    std::wcout << L"Историческая дата: " << historicalDate << std::endl;
    long long daysBeforeBase = historicalDate - baseDate;
    std::wcout << L"Дней от исторической даты до 01.01.1900: " << daysBeforeBase << std::endl;
}

void runAutomatedTests(const std::string& csvFilename) {
    std::wcout << L"\n========= Автоматизированные тесты =========\n" << std::endl;
    
    DateTester tester(csvFilename);
    tester.runTests();
}

int main(int argc, char* argv[]) {
    try {
        // Настраиваем локаль для корректного отображения кириллицы
        #ifdef _WIN32
            _setmode(_fileno(stdout), _O_U16TEXT); // Для Windows
        #else
            std::setlocale(LC_ALL, ""); // Для UNIX-подобных систем
        #endif
        
        std::string csvFilename = "date_tests.csv";  // Имя файла по умолчанию
        
        // Обработка аргументов командной строки
        if (argc > 1) {
            csvFilename = argv[1];
        }
        
        // Определяем режим тестирования
        bool runManual = true;
        bool runAutomated = true;
        
        if (argc > 2) {
            std::string mode = argv[2];
            if (mode == "manual") {
                runAutomated = false;
            }
            else if (mode == "auto") {
                runManual = false;
            }
        }
        
        std::wcout << L"====== Тестирование класса Date ======" << std::endl;
        
        if (runManual) {
            runManualTests();
        }
        
        if (runAutomated) {
            runAutomatedTests(csvFilename);
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        #ifdef _WIN32
            // Переключимся обратно на обычный текстовый режим для вывода ошибки
            _setmode(_fileno(stderr), _O_TEXT);
        #endif
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
        return 1;
    }
}