#include <iostream>
#include <algorithm> // For std::sort function
#include <optional>  // For std::optional
using namespace std;

template<typename T>
class IntArray {
    T* data;  // Pointer to the array data
    int size;   // Size of the array
    int baseIndex; // Base index for the array (can be negative)
public:
    // Constructor that initializes the array with a given size and base index
    IntArray(int n, int base = 0) : size(n), baseIndex(base) {
        data = new T[size];
    }

    // Copy constructor
    IntArray(const IntArray& other) : size(other.size), baseIndex(other.baseIndex) {
        data = new T[size];
        copy(other.data, other.data + size, data);
    }

    // Assignment operator
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            baseIndex = other.baseIndex;
            data = new T[size];
            copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Destructor to free the memory allocated for the array
    ~IntArray() {
        delete[] data;
    }

    // Overloading of index operator with bounds checking and support for negative base indices
    T& operator[](int index) {
        if (index < baseIndex || index >= size + baseIndex) {
            throw out_of_range("Index out of range");
        }
        return data[index - baseIndex];
    }

    // Method to get the base index of the array
    int getBaseIndex() const {
        return baseIndex;
    }

    // Method to sort the array using std::sort function
    void sort() {
        std::sort(data, data + size);
    }

    // Method to find a value in the array and return an optional index
    optional<int> find(const T& value) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return i + baseIndex;
        }
        return nullopt; // Return nullopt if value is not found
    }

    // Overloading of addition operator to add two arrays element-wise
    IntArray operator+(const IntArray& other) {
        if (size != other.size || baseIndex != other.baseIndex) {
            throw invalid_argument("Arrays must be the same size and have the same base index");
        }
        IntArray result(size, baseIndex);
        for (int i = 0; i < size; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Overloading of subtraction operator to subtract two arrays element-wise
    IntArray operator-(const IntArray& other) {
        if (size != other.size || baseIndex != other.baseIndex) {
            throw invalid_argument("Arrays must be the same size and have the same base index");
        }
        IntArray result(size, baseIndex);
        for (int i = 0; i < size; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }
};

int main() {
    // Declare IntArray objects outside the try block
    IntArray<int> arr1(5, 3);
    IntArray<int> arr2(5, 3);

    try {
        // Initialize arr1 with values 44, 45, 46, 47, 48
        int values1[] = {44, 45, 46, 47, 48};
        for (int i = arr1.getBaseIndex(), j = 0; i < arr1.getBaseIndex() + 5; ++i, ++j) {
            arr1[i] = values1[j];
        }

        // Initialize arr2 with values 1, 2, 3, 4, 5
        int values2[] = {1, 2, 3, 4, 5};
        for (int i = arr2.getBaseIndex(), j = 0; i < arr2.getBaseIndex() + 5; ++i, ++j) {
            arr2[i] = values2[j];
        }
        cout << "Array contents (arr1): ";
        for (int i = arr1.getBaseIndex(); i < arr1.getBaseIndex() + 5; ++i) {
            cout << arr1[i] << ' ';
        }
        cout << endl;
        
        cout << "Array contents (arr2): ";
        for (int i = arr2.getBaseIndex(); i < arr2.getBaseIndex() + 5; ++i) {
            cout << arr2[i] << ' ';
        }
        cout << endl;
        
        
        // Subtract arr2 from arr1 and print the result
        IntArray<int> diff = arr1 - arr2;
        cout << "Difference: ";
        for (int i = diff.getBaseIndex(); i < diff.getBaseIndex() + 5; ++i) {
            cout << diff[i] << ' ';
        }
        cout << endl;

        // Find the index of value 46 in arr1 and print the result
        optional<int> index = arr1.find(46);
        if (index) {
            cout << "Value 46 found at index: " << *index << endl;
        } else {
            cout << "Value 46 not found" << endl;
        }

    } catch (const out_of_range& e) {
        cout << "Error: " << e.what() << endl;
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }

    // Output the base indices of both arrays at the end
    cout << "Base index of arr1: " << arr1.getBaseIndex() << endl;
    cout << "Base index of arr2: " << arr2.getBaseIndex() << endl;

    return 0;
}
