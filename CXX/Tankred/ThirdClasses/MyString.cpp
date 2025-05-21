//MyString.cpp

#include "MyString.h"
#include <cstring> // Required for strlen, strcpy, strncpy, strstr (for narrow chars only!)
#include <cwchar>  // Required for wcslen, wcscpy, wcsncpy, wcsstr (for wide chars)
#include <cstdlib> // Required for calloc, free
#include <stdexcept> // For std::out_of_range

MyString::MyString() : str(nullptr) { //Corrected the default constructor - initialize str to nullptr
}

MyString::MyString(const wchar_t* s) {
    str = (wchar_t*)calloc(wcslen(s) + 1, sizeof(wchar_t));
    wcscpy(str, s);
}

MyString::MyString(const MyString& another) {
    if (another.str == nullptr) {
        str = nullptr;
        return;
    }
    str = (wchar_t*)calloc(wcslen(another.str) + 1, sizeof(wchar_t));
    wcscpy(str, another.str);
}

int MyString::Length() const {
  if (str == nullptr) return 0; //Added check for nullptr
    return wcslen(str);
}

MyString::~MyString() {
    free(str);
    str = nullptr;
}

int MyString::FindSub(const wchar_t* s) const {
  if (str == nullptr || s == nullptr) return -1;
    wchar_t* res = wcsstr(str, s);
    if (res) {
        return res - str;
    }
    return -1;
}

int MyString::FindSubLine(const MyString& s) const {
    if (str == nullptr || s.str == nullptr) {
        return -1;
    }
    return FindSub(s.str);
}

MyString MyString::CopyLine(unsigned int index, unsigned int count) const {
    if (str == nullptr) throw std::out_of_range("String is null");
    if (index >= wcslen(str)) {
        throw std::out_of_range("Index out of range");
    }
    if (count <= 0 || index + count > wcslen(str)) {
        throw std::out_of_range("Count out of range");
    }

    wchar_t* temp = (wchar_t*)calloc(count + 1, sizeof(wchar_t));
    wcsncpy(temp, &str[index], count);
    temp[count] = L'\0'; // Null-terminate the copied string

    MyString newLine(temp);
    free(temp);

    return newLine;
}

MyString MyString::CopyLeft(unsigned int count) const {
    if (str == nullptr) throw std::out_of_range("String is null");
    if (count > wcslen(str)) {
        throw std::out_of_range("Count out of range");
    }
    return CopyLine(0, count);
}

MyString MyString::CopyRight(unsigned int count) const {
  if (str == nullptr) throw std::out_of_range("String is null");
    if (count > wcslen(str)) {
        throw std::out_of_range("Count out of range");
    }
    return CopyLine(wcslen(str) - count, count);
}

void MyString::ReplaceSub(const MyString& s, int index, unsigned int count) {
  if (str == nullptr) throw std::out_of_range("String is null");
    if (index < 0 || index + count > wcslen(str)) {
        throw std::out_of_range("Index or count out of range");
    }

    MyString tempRight;
    if (index + count < wcslen(str)) {
        tempRight = CopyRight(wcslen(str) - (index + count));
    }
    else {
        tempRight = MyString(L""); // Empty wide string
    }

    MyString tempLeft = CopyLeft(index);

    MyString result = tempLeft + s + tempRight;

    free(str);
    str = (wchar_t*)calloc(result.Length() + 1, sizeof(wchar_t));
    wcscpy(str, result.str);
}

void MyString::DeleteSub(unsigned int index, unsigned int count) {
  if (str == nullptr) throw std::out_of_range("String is null");
    if (index < 0 || index + count > wcslen(str)) {
        throw std::out_of_range("Index or count out of range");
    }

    MyString empty(L""); // Empty wide string
    ReplaceSub(empty, index, count);
}

MyString MyString::operator=(const MyString& another) {
    if (this == &another) {
        return *this;
    }

    if (str != nullptr) {
        free(str);
    }

    if(another.str == nullptr) {
      str = nullptr;
    } else {
      str = (wchar_t*)calloc(wcslen(another.str) + 1, sizeof(wchar_t));
      wcscpy(str, another.str);
    }

    return *this;
}

MyString MyString::operator+(const MyString& other) const {
    int newSize = 0;
    if (str != nullptr) newSize += wcslen(str);
    if (other.str != nullptr) newSize += wcslen(other.str);
    newSize++;

    wchar_t* temp = (wchar_t*)calloc(newSize, sizeof(wchar_t));
    if (temp == nullptr) {
      throw std::bad_alloc();
    }

    if (str != nullptr) wcscpy(temp, str);
    else temp[0] = L'\0';

    if (other.str != nullptr) wcscat(temp, other.str);

    MyString newLine(temp);
    free(temp);

    return newLine;
}

wchar_t& MyString::operator[](int index) {
  if (str == nullptr) throw std::out_of_range("String is null");
    if (index < 0 || static_cast<unsigned int>(index) >= wcslen(str)) {
        throw std::out_of_range("Index out of range");
    }
    return str[index];
}

std::wostream& operator<<(std::wostream& os, const MyString& obj) {
    if(obj.str != nullptr)
      os << obj.str;
    return os;
}