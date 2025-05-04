#include "IntArray.h"
#include <stdexcept>
template<typename T>
IntArray<T>::IntArray(int n, int base) : size(n), baseIndex(base) {
    data = new T[size];
}

template<typename T>
IntArray<T>::IntArray(const IntArray& other) : size(other.size), baseIndex(other.baseIndex) {
    data = new T[size];
    std::copy(other.data, other.data + size, data);
}

template<typename T>
IntArray<T>& IntArray<T>::operator=(const IntArray& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        baseIndex = other.baseIndex;
        data = new T[size];
        std::copy(other.data, other.data + size, data);
    }
    return *this;
}

template<typename T>
IntArray<T>::~IntArray() {
    delete[] data;
}

template<typename T>
T& IntArray<T>::operator[](int index) {
    if (index < baseIndex || index >= size + baseIndex) {
        throw std::out_of_range("Index out of range");
    }
    return data[index - baseIndex];
}

template<typename T>
int IntArray<T>::getBaseIndex() const {
    return baseIndex;
}

template<typename T>
void IntArray<T>::sort() {
    std::sort(data, data + size);
}

template<typename T>
std::optional<int> IntArray<T>::find(const T& value) {
    for (int i = 0; i < size; ++i) {
        if (data[i] == value) return i + baseIndex;
    }
    return std::nullopt; // Return nullopt if value is not found
}

template<typename T>
IntArray<T> IntArray<T>::operator+(const IntArray& other) {
    if (size != other.size || baseIndex != other.baseIndex) {
        throw std::invalid_argument("Arrays must be the same size and have the same base index");
    }
    IntArray result(size, baseIndex);
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

template<typename T>
IntArray<T> IntArray<T>::operator-(const IntArray& other) {
    if (size != other.size || baseIndex != other.baseIndex) {
        throw std::invalid_argument("Arrays must be the same size and have the same base index");
    }
    IntArray result(size, baseIndex);
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}