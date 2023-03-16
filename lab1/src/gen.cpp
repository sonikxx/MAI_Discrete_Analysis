#include <random>
#include <iostream>
#include <string>

using namespace std;

string gen(size_t len) {
    string s;
    string p = "0123456789abcdef";
    for (size_t i = 0; i < len; i++) {
        s+=(p[random()%16]);
    }
    return s;
    
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "error\n";
        return 1;
    }
    long long N = atoi(argv[1]);
    for (size_t i = 0; i < N; i++) {
        cout << gen(32) << '\t' << gen(1 + random()%2047) << '\n';
    }
    return 0;
}