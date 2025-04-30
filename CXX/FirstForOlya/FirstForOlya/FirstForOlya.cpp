#include <iostream>
#include <algorithm> // For std::sort function
using namespace std;

class IntArray {
    int* data;  // Pointer to the array data
    int size;   // Size of the array
public:
    // Constructor that initializes the array with a given size
    IntArray(int n) : size(n) {
        data = new int[size];
    }

    // Destructor to free the memory allocated for the array
    ~IntArray() {
        delete[] data;
    }

    // Overloading of index operator with bounds checking
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    // Method to sort the array using std::sort function
    void sort() {
        std::sort(data, data + size);
    }

    // Method to find a value in the array and return its index or -1 if not found
    int find(int value) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return i;
        }
        return -1;
    }

    // Overloading of addition operator to add two arrays element-wise
    IntArray operator+(const IntArray& other) {
        if (size != other.size) throw invalid_argument("Arrays must be the same size");
        IntArray result(size);
        for (int i = 0; i < size; ++i) {
            result[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Overloading of subtraction operator to subtract two arrays element-wise
    IntArray operator-(const IntArray& other) {
        if (size != other.size) throw invalid_argument("Arrays must be the same size");
        IntArray result(size);
        for (int i = 0; i < size; ++i) {
            result[i] = data[i] - other.data[i];
        }
        return result;
    }
};
int main() {
    // Create IntArray objects of size 5
    IntArray arr1(5), arr2(5);

    // Initialize arr1 with values from 0 to 4
    for (int i = 0; i < 5; ++i) {
        arr1[i] = i;
    }

    // Initialize arr2 with values from 5 to 9
    for (int i = 0; i < 5; ++i) {
        arr2[i] = i + 5;
    }

    // Add arr1 and arr2 and print the result
    IntArray sum = arr1 + arr2;
    cout << "Sum: ";
    for (int i = 0; i < 5; ++i) {
        cout << sum[i] << ' ';
    }
    cout << endl;

    // Subtract arr2 from arr1 and print the result
    IntArray diff = arr1 - arr2;
    cout << "Difference: ";
    for (int i = 0; i < 5; ++i) {
        cout << diff[i] << ' ';
    }
    cout << endl;

    // Sort arr1 and print the result
    arr1.sort();
    cout << "Sorted arr1: ";
    for (int i = 0; i < 5; ++i) {
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
};
