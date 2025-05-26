// CString.h
#pragma once
#ifndef CSTRING_H
#define CSTRING_H

#include <iostream>
#include <string>
#include <stdexcept>

class CString {
public:
    CString(int len = 0);
    CString(const char* str);
    CString(const CString& other);
    ~CString() = default; // Use default destructor

    std::string GetString() const;
    void SetString(const char* str);
    int GetLength() const;
    std::string::size_type FindSubStr(const char* substr) const;
    CString StrCopy(int start, int byte) const;
    void ReplaceSubStr(const char* substr1, const char* substr2);
    void RemoveSubStr(const char* substr);

    CString operator+(const CString& other) const;
    CString& operator=(const CString& other);
    CString& operator=(const char* str);
    char& operator[](int index);
    const char& operator[](int index) const;

    friend std::ostream& operator<<(std::ostream& os, const CString& str);
    friend std::istream& operator>>(std::istream& is, CString& str);

private:
    std::string str;
};

#endif