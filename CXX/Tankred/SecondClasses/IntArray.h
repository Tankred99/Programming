#ifndef INTARRAY_H
#define INTARRAY_H

#include <algorithm> // For std::sort function
#include <optional>  // For std::optional
#include <iostream> //For stream operators
#include <stdexcept> //For exception handling


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
    std::optional<int> find(const T& value) const;

    // Overloading of addition operator to add two arrays element-wise
    IntArray operator+(const IntArray& other) const;

    // Overloading of subtraction operator to subtract two arrays element-wise
    IntArray operator-(const IntArray& other) const;

    int getSize() const { return size; }

    // Add a public method to access elements safely
    const T* getData() const { return data; }
};

// Overload the output stream operator <<
template <typename T>
std::ostream& operator<<(std::ostream& os, const IntArray<T>& arr) {
    os << "[" << arr.getBaseIndex() << "]: ";
    for (int i = 0; i < arr.getSize(); ++i) {
        os << arr.getData()[i] << (i < arr.getSize() - 1 ? ", " : "");
    }
    return os;
}

// Overload the input stream operator >> (this requires more design consideration, possibly by expecting a specific format)
template <typename T>
std::istream& operator>>(std::istream& is, IntArray<T>& arr) {
    throw std::runtime_error("Input stream operator not yet implemented.");
}


#endif // INTARRAY_H
