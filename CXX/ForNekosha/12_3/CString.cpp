// CString.cpp
#include "CString.h"
#include <iostream>
#include <string>
#include <stdexcept>

CString::CString(int len) {
    if (len < 0) {
        throw std::invalid_argument("Длина не может быть отрицательной.");
    }
    str.resize(len, '\0');
}

CString::CString(const char* _str) {
    if (!_str) {
        throw std::invalid_argument("Строка не может быть nullptr.");
    }
    str = _str;
}

CString::CString(const CString& other) : str(other.str) {}

void CString::SetString(const char* _str) {
    if (!_str) {
        throw std::invalid_argument("Строка не может быть nullptr.");
    }
    str = _str;
}

std::string CString::GetString() const {
    return str;
}

void CString::SetLength(int length) {
    if (length < 0) {
        throw std::invalid_argument("Длина не может быть отрицательной.");
    }
    str.resize(length);
}

int CString::GetLength() const {
    return str.length();
}

std::string::size_type CString::FindSubStr(const char* substr) const {
    if (!substr) {
        return std::string::npos;
    }
    return str.find(substr);
}

CString CString::StrCopy(int start, int byte) const {
    if (start < 0 || start >= GetLength() || byte <= 0) {
        throw std::out_of_range("Неверные параметры копирования.");
    }

    return CString(str.substr(start, byte).c_str());
}

void CString::ReplaceSubStr(const char* substr1, const char* substr2) {
    if (!substr1 || !substr2) {
        throw std::invalid_argument("Подстроки не могут быть nullptr.");
    }
    std::string::size_type pos = str.find(substr1);
    if (pos != std::string::npos) {
        str.replace(pos, strlen(substr1), substr2);
    }
}

void CString::RemoveSubStr(const char* substr) {
    if (substr == nullptr) return; // Handle null pointer
    ReplaceSubStr(substr, "");
}

CString CString::operator+(const CString& other) const {
    return CString(str + other.str);
}

CString& CString::operator=(const CString& other) {
    if (this != &other) {
        str = other.str;
    }
    return *this;
}

CString& CString::operator=(const char* _str) {
    if (!_str) {
        throw std::invalid_argument("Строка не может быть nullptr.");
    }
    str = _str;
    return *this;
}

char& CString::operator[](int index) {
    if (index < 0 || index >= GetLength()) {
        throw std::out_of_range("Индекс вне диапазона.");
    }
    return str[index];
}

const char& CString::operator[](int index) const {
    if (index < 0 || index >= GetLength()) {
        throw std::out_of_range("Индекс вне диапазона.");
    }
    return str[index];
}

std::ostream& operator<<(std::ostream& os, const CString& string) {
    os << string.str;
    return os;
}

std::istream& operator>>(std::istream& is, CString& string) {
    std::string temp;
    std::getline(is, temp);
    string.str = temp;
    return is;
}