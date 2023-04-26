#include <iostream>
#include <map>

using namespace std;

const unsigned int KEY_LEN = 257;

int main() {
    char input[KEY_LEN] = {0};
    uint64_t value;
    std::map<std::string, uint64_t> m;
    while (cin >> input) {
        if (input[0] == '+') {
            cin >> input >> value;
            m.insert({input, value});
        } else if (input[0] == '-') {
            cin >> input;
            m.erase(input);
        } else {
            m.find(input);
        }
    }
    return 0;
}
