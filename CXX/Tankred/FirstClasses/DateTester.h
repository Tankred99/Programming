#ifndef DATE_TESTER_H
#define DATE_TESTER_H

#include "Date.h"
#include "CSVParser.h"
#include <string>
#include <vector>
#include <iostream>

class DateTester {
private:
    std::vector<std::vector<std::string>> testData;
    int passedTests;
    int failedTests;

public:
    DateTester(const std::string& csvFilename) : passedTests(0), failedTests(0) {
        CSVParser parser;
        try {
            testData = parser.parseFile(csvFilename);
            // Удаляем заголовок
            if (!testData.empty()) {
                testData.erase(testData.begin());
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading test data: " << e.what() << std::endl;
        }
    }

    void runTests() {
        std::wcout << L"Запуск тестирования даты..." << std::endl;
        
        for (size_t i = 0; i < testData.size(); ++i) {
            const auto& row = testData[i];
            
            if (row.size() < 6) {
                std::wcout << L"Пропуск некорректной строки теста #" << (i + 1) << std::endl;
                continue;
            }

            try {
                std::string operation = row[0];
                
                if (operation == "compare") {
                    testCompare(row);
                }
                else if (operation == "diff") {
                    testDiff(row);
                }
                else if (operation == "add") {
                    testAdd(row);
                }
                else if (operation == "format") {
                    testFormat(row);
                }
                else if (operation == "parse") {
                    testParse(row);
                }
                else {
                    std::wcout << L"Неизвестная операция в тесте #" << (i + 1) << ": " << operation.c_str() << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::wcout << L"Ошибка в тесте #" << (i + 1) << ": " << e.what() << std::endl;
                failedTests++;
            }
        }

        std::wcout << L"\nРезультаты тестирования:" << std::endl;
        std::wcout << L"Успешно: " << passedTests << L" тестов" << std::endl;
        std::wcout << L"Неудачно: " << failedTests << L" тестов" << std::endl;
        std::wcout << L"Всего: " << (passedTests + failedTests) << L" тестов" << std::endl;
    }

private:
    void testCompare(const std::vector<std::string>& row) {
        std::string date1Str = row[1];
        std::string date2Str = row[2];
        std::string format1 = row[3];
        std::string format2 = row[4];
        std::string expectedResult = row[5];

        Date date1(date1Str, format1);
        Date date2(date2Str, format2);
        
        bool result = false;
        if (expectedResult == "<") {
            result = (date1 < date2);
        }
        else if (expectedResult == ">") {
            result = (date1 > date2);
        }
        else if (expectedResult == "=") {
            result = (date1 == date2);
        }
        else {
            throw std::invalid_argument("Неизвестный оператор сравнения: " + expectedResult);
        }

        std::wstring compareOp;
        if (expectedResult == "<") compareOp = L"<";
        else if (expectedResult == ">") compareOp = L">";
        else if (expectedResult == "=") compareOp = L"=";

        if (result) {
            std::wcout << L"✓ Тест сравнения: " << date1.toWString(format1).c_str() 
                      << L" " << compareOp.c_str() << L" " 
                      << date2.toWString(format2).c_str() << std::endl;
            passedTests++;
        }
        else {
            std::wcout << L"✗ Тест сравнения провален: " << date1.toWString(format1).c_str() 
                      << L" " << compareOp.c_str() << L" " 
                      << date2.toWString(format2).c_str() << std::endl;
            failedTests++;
        }
    }

    void testDiff(const std::vector<std::string>& row) {
        std::string date1Str = row[1];
        std::string date2Str = row[2];
        std::string format1 = row[3];
        std::string format2 = row[4];
        long long expectedDiff = std::stoll(row[5]);

        Date date1(date1Str, format1);
        Date date2(date2Str, format2);
        
        long long diff = date2 - date1;
        
        if (diff == expectedDiff) {
            std::wcout << L"✓ Тест разности дат: " << date1.toWString(format1).c_str() 
                      << L" до " << date2.toWString(format2).c_str() 
                      << L" = " << diff << L" дней" << std::endl;
            passedTests++;
        }
        else {
            std::wcout << L"✗ Тест разности дат провален: " << date1.toWString(format1).c_str() 
                      << L" до " << date2.toWString(format2).c_str() 
                      << L" = " << diff << L" дней (ожидалось " << expectedDiff << L")" << std::endl;
            failedTests++;
        }
    }

    void testAdd(const std::vector<std::string>& row) {
        std::string date1Str = row[1];
        int daysToAdd = std::stoi(row[2]);
        std::string format1 = row[3];
        std::string expectedDateStr = row[5];

        Date date1(date1Str, format1);
        Date resultDate = date1;
        resultDate.addDays(daysToAdd);
        
        Date expectedDate(expectedDateStr, "DD.MM.YYYY");
        
        if (resultDate == expectedDate) {
            std::wcout << L"✓ Тест добавления дней: " << date1.toWString(format1).c_str() 
                      << L" + " << daysToAdd << L" = " << resultDate.toWString().c_str() << std::endl;
            passedTests++;
        }
        else {
            std::wcout << L"✗ Тест добавления дней провален: " << date1.toWString(format1).c_str() 
                      << L" + " << daysToAdd << L" = " << resultDate.toWString().c_str() 
                      << L" (ожидалось " << expectedDate.toWString().c_str() << L")" << std::endl;
            failedTests++;
        }
    }

    void testFormat(const std::vector<std::string>& row) {
        std::string dateStr = row[1];
        std::string inputFormat = row[3];
        std::string outputFormat = row[4];
        std::string expectedOutput = row[5];

        Date date(dateStr, inputFormat);
        std::string formattedDate = date.toString(outputFormat);
        
        if (formattedDate == expectedOutput) {
            std::wcout << L"✓ Тест форматирования: " << date.toWString(inputFormat).c_str() 
                      << L" в формате " << outputFormat.c_str() 
                      << L" = " << formattedDate.c_str() << std::endl;
            passedTests++;
        }
        else {
            std::wcout << L"✗ Тест форматирования провален: " << date.toWString(inputFormat).c_str() 
                      << L" в формате " << outputFormat.c_str() 
                      << L" = " << formattedDate.c_str() 
                      << L" (ожидалось " << expectedOutput.c_str() << L")" << std::endl;
            failedTests++;
        }
    }

    void testParse(const std::vector<std::string>& row) {
        std::string dateStr = row[1];
        std::string inputFormat = row[3];
        std::string expectedOutput = row[5];

        Date date(dateStr, inputFormat);
        std::string formattedDate = date.toString("DD.MM.YYYY");
        
        if (formattedDate == expectedOutput) {
            std::wcout << L"✓ Тест парсинга: " << dateStr.c_str() 
                      << L" в формате " << inputFormat.c_str() 
                      << L" = " << formattedDate.c_str() << std::endl;
            passedTests++;
        }
        else {
            std::wcout << L"✗ Тест парсинга провален: " << dateStr.c_str() 
                      << L" в формате " << inputFormat.c_str() 
                      << L" = " << formattedDate.c_str() 
                      << L" (ожидалось " << expectedOutput.c_str() << L")" << std::endl;
            failedTests++;
        }
    }
};

#endif // DATE_TESTER_H