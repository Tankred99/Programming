// MyString.h

#ifndef _MyString
#define _MyString
#include <iostream>
#include <stdexcept>
#include <string> //Use std::wstring

class MyString {
private:
    wchar_t* str; // Use wchar_t* for wide characters

public:
    MyString(); // Default constructor
    MyString(const wchar_t* s); // Constructor (wide char)
    MyString(const MyString& another); // Copy constructor
    ~MyString(); // Destructor

    int Length() const;
    int FindSub(const wchar_t* s) const; // Find substring (wide char)
    int FindSubLine(const MyString& s) const;
    MyString CopyLine(unsigned int index, unsigned int count) const;
    MyString CopyLeft(unsigned int count) const;
    MyString CopyRight(unsigned int count) const;
    void ReplaceSub(const MyString& s, int index, unsigned int count);
    void DeleteSub(unsigned int index, unsigned int count);

    // Operator overloading
    MyString operator=(const MyString& another);
    MyString operator+(const MyString& other) const;
    wchar_t& operator[](int index);

    // Output stream operator (wide char)
    friend std::wostream& operator<<(std::wostream& os, const MyString& obj);
};

#endif