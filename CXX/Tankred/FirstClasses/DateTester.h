#ifndef DATETESTER_H
#define DATETESTER_H

#include "Date.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>


class DateTester {
public:
    static std::vector<std::vector<std::string>> readCSV(const std::string& filename);
    void runTests(const std::vector<std::vector<std::string>>& testCases);
    void testAdd(const std::vector<std::string>& row);
    void testFormat(const std::vector<std::string>& row);
    void testParse(const std::vector<std::string>& row);

};

#endif // DATETESTER_H
