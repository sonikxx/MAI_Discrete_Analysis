#include <string>
#include <iostream>
#include <set>
#include <chrono>
#include <random>

using namespace std;

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

string gen_string() {
    string s;
    string t = "a";
    int len = rng() % 5 + 1;
    for (int i = 0; i < len; ++i) {
        t[0] = ('a' + rng()%10);
        s += t;
    }
    return s;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./a.out <Tests count>\n");
        return 1;
    }
    size_t n = atoi(argv[1]);
    int q; 
    for (size_t i = 1; i < n; ++i) {
        q = rng() % 100;
        if (q >= 0) {
            if (q < 33) {
                cout << gen_string() << '\n';
            } else if (q < 44) {
                cout << "- " << gen_string() << '\n';
            } else if (q < 100) {
                cout << "+ " << gen_string() << " " << rng() << '\n';
            } else {
                cout << gen_string() << '\n';
            }
        }
    }
    return 0;
}

