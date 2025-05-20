#include "DateTester.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<std::vector<std::string>> DateTester::readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

void DateTester::testAdd(const std::vector<std::string>& row) {
    std::string operation = row[0];
    Date date1(row[1], "DD.MM.YYYY");
    int daysToAdd = std::stoi(row[2]);
    std::string expectedDateStr = row[4];

    Date resultDate = date1;
    try {
        resultDate.change(daysToAdd);
    } catch (const std::exception& e) {
        std::cerr << "Exception during testAdd: " << e.what() << std::endl;
        assert(false); 
        return;
    }

    Date expectedDate(expectedDateStr, "DD.MM.YYYY");

    assert(resultDate == expectedDate);
    std::cout << "✓ Тест сложения: " << row[1] << " + " << daysToAdd << " = " << expectedDateStr << std::endl;
}

void DateTester::testFormat(const std::vector<std::string>& row) {
    std::string operation = row[0];
    std::string dateStr = row[1];
    std::string inputFormat = row[3];
    std::string outputFormat = row[4];
    std::string expectedOutput = row[5];

    try {
        Date date(dateStr, inputFormat);
        std::string formattedDate = date.toFormat(outputFormat);
        assert(formattedDate == expectedOutput);
        std::cout << "✓ Тест форматирования: " << date.toFormat(inputFormat) << " (" << inputFormat << ") => " << formattedDate << " (" << outputFormat << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception during testFormat: " << e.what() << std::endl;
        assert(false); 
        return;
    }
}

void DateTester::testParse(const std::vector<std::string>& row) {
    std::string operation = row[0];
    std::string dateStr = row[1];
    std::string inputFormat = row[3];
    std::string expectedOutput = row[4];

    try {
        Date date(dateStr, inputFormat);
        std::string formattedDate = date.toFormat("DD.MM.YYYY");
        assert(formattedDate == expectedOutput);
        std::cout << "✓ Тест парсинга: " << dateStr << " (" << inputFormat << ") => " << formattedDate << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception during testParse: " << e.what() << std::endl;
        assert(false); 
        return;
    }
}

void DateTester::runTests(const std::vector<std::vector<std::string>>& testCases) {
    try {
        for (size_t i = 0; i < testCases.size(); ++i) {
            const auto& row = testCases[i];
            std::cout << "\nRunning test case " << i + 1 << ": " << row[0] << std::endl; 
            std::string operation = row[0];
            if (operation == "compare") {
                Date date1(row[1], "DD.MM.YYYY");
                Date date2(row[2], "DD.MM.YYYY");
                std::string comparison = row[4];
                std::cout << "Comparing " << date1 << " and " << date2 << std::endl;
                if (comparison == "<" ) assert(date1 < date2);
                else if (comparison == ">") assert(date1 > date2);
                else if (comparison == "=") assert(date1 == date2);
                std::cout << "Comparison test completed." << std::endl;
            } else if (operation == "diff") {
                Date date1(row[1], "DD.MM.YYYY");
                Date date2(row[2], "DD.MM.YYYY");
                long long expectedDiff = std::stoll(row[4]);
                long long actualDiff = date2 - date1;
                std::cout << "Calculating difference between " << date1 << " and " << date2 << std::endl;
                assert(actualDiff == expectedDiff);
                std::cout << "Difference test completed." << std::endl;
            } else if (operation == "add") {
                std::cout << "Starting add test." << std::endl;
                testAdd(row);
                std::cout << "Add test completed." << std::endl;
            } else if (operation == "format") {
                std::cout << "Starting format test." << std::endl;
                testFormat(row);
                std::cout << "Format test completed." << std::endl;
            } else if (operation == "parse") {
                std::cout << "Starting parse test." << std::endl;
                testParse(row);
                std::cout << "Parse test completed." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
    }
}
