#include <iostream>
#include <vector>

using namespace std;

int check(int64_t i, int64_t n) {
    string s_i = to_string(i);
    string s_n = to_string(n);
    if (s_n.compare(s_i) <= 0)
        return 1;
    else
        return 0;
}

int main() {
    int64_t n;
    int64_t m;
    cin >> n >> m;
    vector<int64_t> storage;
    for (int i = 1; i < n; ++i) {
        if (i % m == 0)
            storage.push_back(i);
    }
    vector<int64_t> res;
    for (int i = 0; i < storage.size(); ++i) {
        if (check(storage[i], n) == 0)
            res.push_back(storage[i]);
    }
    cout << res.size();
    return 0;
}