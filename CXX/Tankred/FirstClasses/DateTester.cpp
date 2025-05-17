#include "DateTester.h"
#include <cassert>

std::vector<std::vector<std::string>> DateTester::readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open CSV file");
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
    resultDate.addDays(daysToAdd);
    Date expectedDate(expectedDateStr, "DD.MM.YYYY");

    assert(resultDate == expectedDate);
    std::wcout << L"✓ Тест сложения: " << row[1] << L" + " << daysToAdd << L" = " << expectedDateStr << std::endl;
}

void DateTester::testFormat(const std::vector<std::string>& row) {
    std::string operation = row[0];
    std::string dateStr = row[1];
    std::string inputFormat = row[3];
    std::string outputFormat = row[4];
    std::string expectedOutput = row[5];

    Date date(dateStr, inputFormat);
    std::string formattedDate = date.toFormat(outputFormat);

    assert(formattedDate == expectedOutput);
    std::wcout << L"✓ Тест форматирования: " << date.toFormat(inputFormat) << L" (" << inputFormat << L") => " << formattedDate << L" (" << outputFormat << L")" << std::endl;
}

void DateTester::testParse(const std::vector<std::string>& row) {
    std::string operation = row[0];
    std::string dateStr = row[1];
    std::string inputFormat = row[3];
    std::string expectedOutput = row[4];

    Date date(dateStr, inputFormat);
    std::string formattedDate = date.toFormat("DD.MM.YYYY");

    assert(formattedDate == expectedOutput);
    std::wcout << L"✓ Тест парсинга: " << dateStr << L" (" << inputFormat << L") => " << formattedDate << std::endl;
}

void DateTester::runTests(const std::vector<std::vector<std::string>>& testCases) {
    for (const auto& row : testCases) {
        std::string operation = row[0];
        if (operation == "compare") {
            Date date1(row[1], "DD.MM.YYYY");
            Date date2(row[2], "DD.MM.YYYY");
            std::string comparison = row[4];

            if (comparison == "<" ) assert(date1 < date2);
            else if (comparison == ">") assert(date1 > date2);
            else if (comparison == "=") assert(date1 == date2);

            std::wcout << L"✓ Тест сравнения: " << row[1] << L" " << row[4] << L" " << row[2] << std::endl;
        } else if (operation == "diff") {
            Date date1(row[1], "DD.MM.YYYY");
            Date date2(row[2], "DD.MM.YYYY");
            long long expectedDiff = std::stoll(row[4]);
            long long actualDiff = date2 - date1;
            assert(actualDiff == expectedDiff);
            std::wcout << L"✓ Тест разности: " << row[2] << L" - " << row[1] << L" = " << actualDiff << std::endl;
        } else if (operation == "add") {
            testAdd(row);
        } else if (operation == "format") {
            testFormat(row);
        } else if (operation == "parse") {
            testParse(row);
        }
    }
}
