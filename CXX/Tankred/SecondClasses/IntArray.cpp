#include "IntArray.h"
#include <stdexcept>
#include <algorithm> //for std::copy
#include <cstring> //for memcpy


template<typename T>
IntArray<T>::IntArray(int n, int base) : size(n), baseIndex(base) {
    if (n < 0) throw std::invalid_argument("Size cannot be negative");
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
std::optional<int> IntArray<T>::find(const T& value) const {
    for (int i = 0; i < size; ++i) {
        if (data[i] == value) return i + baseIndex;
    }
    return std::nullopt; // Return nullopt if value is not found
}

template<typename T>
IntArray<T> IntArray<T>::operator+(const IntArray& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Arrays must have the same size");
    }
    IntArray result(size, 0);
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

template<typename T>
IntArray<T> IntArray<T>::operator-(const IntArray& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Arrays must have the same size");
    }
    IntArray result(size, 0);
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}

// Explicit instantiation for int
template class IntArray<int>;
template std::ostream& operator<<(std::ostream&, const IntArray<int>&);
