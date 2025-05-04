#ifndef INTARRAY_H
#define INTARRAY_H

#include <algorithm> // For std::sort function
#include <optional>  // For std::optional

template<typename T>
class IntArray {
    T* data;  // Pointer to the array data
    int size;   // Size of the array
    int baseIndex; // Base index for the array (can be negative)
public:
    // Constructor that initializes the array with a given size and base index
    IntArray(int n, int base = 0);

    // Copy constructor
    IntArray(const IntArray& other);

    // Assignment operator
    IntArray& operator=(const IntArray& other);

    // Destructor to free the memory allocated for the array
    ~IntArray();

    // Overloading of index operator with bounds checking and support for negative base indices
    T& operator[](int index);

    // Method to get the base index of the array
    int getBaseIndex() const;

    // Method to sort the array using std::sort function
    void sort();

    // Method to find a value in the array and return an optional index
    std::optional<int> find(const T& value);

    // Overloading of addition operator to add two arrays element-wise
    IntArray operator+(const IntArray& other);

    // Overloading of subtraction operator to subtract two arrays element-wise
    IntArray operator-(const IntArray& other);
};

#include "IntArray.cpp"

#endif // INTARRAY_H