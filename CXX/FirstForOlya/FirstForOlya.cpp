#include <iostream>
#include <algorithm> // For std::sort function
using namespace std;

template<typename T>
class IntArray {
    T* data;  // Pointer to the array data
    int size;   // Size of the array
    int startIndex; // Starting index for the array (0 or 1)
public:
    // Constructor that initializes the array with a given size and starting index
    IntArray(int n, int start = 0) : size(n), startIndex(start) {
        data = new T[size];
    }

    // Copy constructor
    IntArray(const IntArray& other) : size(other.size), startIndex(other.startIndex) {
        data = new T[size];
        copy(other.data, other.data + size, data);
    }

    // Assignment operator
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            startIndex = other.startIndex;
            data = new T[size];
            copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Destructor to free the memory allocated for the array
    ~IntArray() {
        delete[] data;
    }

    // Overloading of index operator with bounds checking and support for 0-based and 1-based indexing
    T& operator[](int index) {
        if (index < startIndex || index >= size + startIndex) {
            throw out_of_range("Index out of range");
        }
        return data[index - startIndex];
    }

    // Method to sort the array using std::sort function
    void sort() {
        std::sort(data, data + size);
    }

    // Method to find a value in the array and return its index or -1 if not found
    int find(const T& value) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return i + startIndex;
        }
        return -1;
    }

    // Overloading of addition operator to add two arrays element-wise
    IntArray operator+(const IntArray& other) {
        if (size != other.size || startIndex != other.startIndex) throw invalid_argument("Arrays must be the same size and have the same starting index");
        IntArray result(size, startIndex);
        for (int i = 0; i < size; ++i) {
            result[i + startIndex] = data[i] + other.data[i];
        }
        return result;
    }

    // Overloading of subtraction operator to subtract two arrays element-wise
    IntArray operator-(const IntArray& other) {
        if (size != other.size || startIndex != other.startIndex) throw invalid_argument("Arrays must be the same size and have the same starting index");
        IntArray result(size, startIndex);
        for (int i = 0; i < size; ++i) {
            result[i + startIndex] = data[i] - other.data[i];
        }
        return result;
    }
};

int main() {
    // Create IntArray objects of size 5 with starting index 1
    IntArray<int> arr1(5, 1), arr2(5, 1);

    // Initialize arr1 with values from 0 to 4
    for (int i = 1; i <= 5; ++i) {
        arr1[i] = i - 1;
    }

    // Initialize arr2 with values from 5 to 9
    for (int i = 1; i <= 5; ++i) {
        arr2[i] = i + 4;
    }

    // Add arr1 and arr2 and print the result
    IntArray<int> sum = arr1 + arr2;
    cout << "Sum: ";
    for (int i = 1; i <= 5; ++i) {
        cout << sum[i] << ' ';
    }
    cout << endl;

    // Subtract arr2 from arr1 and print the result
    IntArray<int> diff = arr1 - arr2;
    cout << "Difference: ";
    for (int i = 1; i <= 5; ++i) {
        cout << diff[i] << ' ';
    }
    cout << endl;

    // Sort arr1 and print the result
    arr1.sort();
    cout << "Sorted arr1: ";
    for (int i = 1; i <= 5; ++i) {
        cout << arr1[i] << ' ';
    }
    cout << endl;

    // Find the index of value 3 in arr1 and print the result
    int index = arr1.find(3);
    if (index != -1) {
        cout << "Value 3 found at index: " << index << endl;
    } else {
        cout << "Value 3 not found" << endl;
    }

    return 0;
}