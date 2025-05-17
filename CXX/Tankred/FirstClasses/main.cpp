#include "Date.h"
#include "DateTester.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cwchar>
#include <clocale>
#include <exception>
#include <string>
#include <vector>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

// ... (runManualTests function remains unchanged) ...

int main(int argc, char* argv[]) {
    try {
        // ... (locale setup remains unchanged) ...

        std::string csvFilename = "date_tests.csv"; 

        if (argc > 1) {
            csvFilename = argv[1];
        }

        // ... (mode selection remains unchanged) ...

        std::wcout << L"====== Тестирование класса Date ======" << std::endl;

        if (runManual) {
            runManualTests();
        }

        if (runAutomated) {
            // Read CSV file into a 2D vector
            std::vector<std::vector<std::string>> testCases = DateTester::readCSV(csvFilename);
            DateTester tester;
            tester.runTests(testCases);
        }

        return 0;
    } catch (const std::exception& e) {
        // ... (error handling remains unchanged) ...
    }
}
