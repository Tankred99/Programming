#include <iostream>
#include "IntArray.h"

using namespace std;

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