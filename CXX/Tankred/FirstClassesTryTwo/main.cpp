#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include "date.h"
#include <iomanip> 

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}


int main() {
    std::locale::global(std::locale("")); 

    try {
        Date d1(2024, 3, 10);
        Date d2(L"2024-04-15"); 
        Date d3 = d2 - d1;
        std::wcout << L"d1: " << d1.format(L"YYYY-MM-DD") << std::endl;
        std::wcout << L"d2: " << d2.format(L"YYYY-MM-DD") << std::endl;
        std::wcout << L"d2 - d1: " << d3.format(L"YYYY-MM-DD") << std::endl;

        d1.addDays(10);
        std::wcout << L"d1 after adding 10 days: " << d1.format(L"YYYY-MM-DD") << std::endl;

        std::wcout << L"d1 < d2: " << (d1 < d2) << std::endl;

        std::vector<std::vector<std::string>> testCases = readCSV("test_cases.csv");

        for (size_t i = 1; i < testCases.size(); i++) {
            std::wstring date1Str = std::wstring(testCases[i][0].begin(), testCases[i][0].end());
            std::wstring date2Str = std::wstring(testCases[i][1].begin(), testCases[i][1].end());
            std::wstring expectedResultStr = std::wstring(testCases[i][2].begin(), testCases[i][2].end());

            try {
                Date date1(date1Str);
                Date date2(date2Str);
                Date result = date2 - date1;
                std::wstring actualResultStr = result.format(L"YYYY-MM-DD");

                if (actualResultStr == expectedResultStr) {
                    std::wcout << L"Test case " << i << L": PASSED" << std::endl;
                } else {
                    std::wcout << L"Test case " << i << L": FAILED. Expected: " << expectedResultStr << L", Actual: " << actualResultStr << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::wcerr << L"Test case " << i << L": ERROR: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::wcerr << L"Test case " << i << L": Unexpected error: " << e.what() << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::wcerr << L"Error: " << e.what() << std::endl;
    }
    return 0;
}
