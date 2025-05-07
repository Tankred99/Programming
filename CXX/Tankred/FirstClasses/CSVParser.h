#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

class CSVParser {
private:
    char delimiter;

public:
    CSVParser(char delim = ',') : delimiter(delim) {}

    std::vector<std::vector<std::string>> parseFile(const std::string& filename) {
        std::vector<std::vector<std::string>> data;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            data.push_back(parseLine(line));
        }

        file.close();
        return data;
    }

    std::vector<std::string> parseLine(const std::string& line) {
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            // Удаляем начальные и конечные пробелы
            item = trim(item);
            result.push_back(item);
        }

        return result;
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }
};

#endif // CSV_PARSER_H