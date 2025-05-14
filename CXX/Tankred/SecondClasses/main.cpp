#include <iostream>
#include "IntArray.h"

using namespace std;

int main() {
    IntArray<int> arr1(5, 50);
    IntArray<int> arr2(5, 100);

    try {
        int values1[] = {44, 45, 46, 47, 48};
        for (int i = 0; i < 5; ++i) {
            arr1[arr1.getBaseIndex() + i] = values1[i];
        }

        int values2[] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; ++i) {
            arr2[arr2.getBaseIndex() + i] = values2[i];
        }

        cout << "Array contents (arr1): " << arr1 << endl;
        cout << "Array contents (arr2): " << arr2 << endl;

        IntArray<int> diff = arr1 + arr2;
        cout << "Sum: " << diff << endl;

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

    cout << "Base index of arr1: " << arr1.getBaseIndex() << endl;
    cout << "Base index of arr2: " << arr2.getBaseIndex() << endl;

    return 0;
}
