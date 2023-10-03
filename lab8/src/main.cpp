#include <iostream>
#include <vector>

using namespace std;

int main() {
    size_t N;
    cin >> N;
    vector<int> storage;
    size_t count_one = 0;
    size_t count_two = 0;
    size_t count_three = 0;
    for (int i = 0; i < N; ++i) {
        size_t tmp;
        cin >> tmp;
        storage.push_back(tmp);
        if (storage[i] == 1)
            ++count_one;
        if (storage[i] == 2)
            ++count_two;
        if (storage[i] == 3)
            ++count_three;
    }
    size_t count_one_in_one = 0;
    size_t count_one_in_two = 0;
    size_t count_one_in_three = 0;
    size_t count_two_in_one = 0;
    size_t count_two_in_two = 0;
    size_t count_two_in_three = 0;
    size_t count_three_in_one = 0;
    size_t count_three_in_two = 0;
    size_t count_three_in_three = 0;
    for (size_t i = 0; i < storage.size(); ++i) {
        if (storage[i] == 1) {
            if (i < count_one)
                ++count_one_in_one;
            else if ((i >= count_one) && (i < count_one + count_two))
                ++count_one_in_two;
            else
                ++count_one_in_three;
        }
        if (storage[i] == 2) {
            if (i < count_one)
                ++count_two_in_one;
            else if ((i >= count_one) && (i < count_one + count_two))
                ++count_two_in_two;
            else
                ++count_two_in_three;
        }
        if (storage[i] == 3) {
            if (i < count_one)
                ++count_three_in_one;
            else if ((i >= count_one) && (i < count_one + count_two))
                ++count_three_in_two;
            else
                ++count_three_in_three;
        }
    }
    size_t res = 0;
    // block one
    while (count_two_in_one > 0) {
        if (count_one_in_two > 0)
            --count_one_in_two;
        else {
            --count_one_in_three;
            ++count_two_in_three;
        }
        --count_two_in_one;
        ++res;
    }
    while (count_three_in_one > 0) {
        if (count_one_in_two > 0) {
            --count_one_in_two;
            ++count_three_in_two;
        } else
            --count_one_in_three;
        --count_three_in_one;
        ++res;
    }
    // block two
    while (count_three_in_two > 0) {
        --count_two_in_three;
        --count_three_in_two;
        ++res;
    }
    cout << res;
    return 0;
}