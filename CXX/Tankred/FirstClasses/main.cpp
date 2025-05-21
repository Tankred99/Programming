#include "Date.h"
#include "DateTester.h"
#include <iostream>
//#include <fstream>
//#include <sstream>
#include <cwchar>
#include <clocale>
#include <exception>
#include <string>
#include <vector>
//#include <stdexcept>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

using namespace std;
void runManualTests() {
    Date d1(15, 5, 2023);
    Date d2(23, 11, 2023);
    Date d3(1,1,1900);


    cout << "Ручной тест: 15.05.2023: " << d1.toFormat() << endl;
    cout << "Ручной тест: 23.11.2023: " << d2.toFormat() << endl;
    cout << "Ручной тест: 01.01.1900: " << d3.toFormat() << endl;

    cout << "Ручной тест: Разница дат 15.05.2023 и 23.11.2023: " << d2 - d1 << " дней" << endl;


    d1.change(10);
    cout << "Ручной тест: Добавление 10 дней к 15.05.2023: " << d1.toFormat() << endl;

    d1.change(-10);
    cout << "Ручной тест: Вычитание 10 дней к 25.05.2023: " << d1.toFormat() << endl;
}


int main(int argc, char* argv[]) {
    try {
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_U8TEXT);
#else
        setlocale(LC_ALL, "");
#endif

        bool runManual = false;
        bool runAutomated = true;

        if (argc > 2) {
            string mode = argv[2];
            if (mode == "manual") {
                runManual = true;
                runAutomated = false;
            } else if (mode == "auto") {
                runManual = false;
                runAutomated = true;
            }
        }

        string csvFilename = "date_tests.csv";
        if (argc > 1) {
            csvFilename = argv[1];
        }

        cout << "====== Тестирование класса Date ======" << endl;

        if (runManual) {
            runManualTests();
        }

        if (runAutomated) {
            vector<vector<string>> testCases = DateTester::readCSV(csvFilename);
            DateTester tester;
            tester.runTests(testCases);
        }
        return 0;
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
}
