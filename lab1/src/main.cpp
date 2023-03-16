#include <iostream>

const int MAX_KEY = 33;
const int MAX_VALUE = 2049;
const int LEN = 1e6;
const int STR_LEN = 1e8;

struct value_t {
    unsigned int l;
    unsigned int r;
};

struct pair {
    char key[MAX_KEY];
    struct value_t value;
};



void counting_sort(struct pair *inputed, int i, int index) {
    struct pair *res = (struct pair *) malloc(LEN*sizeof(struct pair));
    int count[16];
    for (int i = 0; i < 16; ++i) {
        count[i] = 0;
    }

    for (int j = 0; j < index; ++j) {
        if (inputed[j].key[i] - '0' >= 49) { //char
            ++count[inputed[j].key[i] - '0' - 39];
        }
        else {
            ++count[inputed[j].key[i] - '0'];
        }
    }

    for (int j = 1; j < 16; ++j) {
        count[j] += count[j - 1];
    }

    for (int j = index - 1; j >= 0; --j) {
        if (inputed[j].key[i] - '0' >= 49) {
            --count[inputed[j].key[i] - '0' - 39];
            res[count[inputed[j].key[i] - '0' - 39]] = inputed[j];
        }
        else {
            --count[inputed[j].key[i] - '0'];
            res[count[inputed[j].key[i] - '0']] = inputed[j];
        }
    }

    for (int i = 0; i < index; i++) {
        inputed[i] = res[i];
    }
    free(res);
}

void radix_sort(struct pair *inputed, int index) {
    for (int i = MAX_KEY - 2; i >= 0; --i) {
        counting_sort(inputed, i, index);
    }
}

int main() {
    char* allData = (char*) malloc(STR_LEN * sizeof(char));
    for (int i = 0; i < 10000; ++i) {
        allData[i] = 0;
    }
    struct pair *inputed = (struct pair *) malloc(LEN*sizeof(struct pair));
    int index = 0;
    int len = 0;
	struct pair elem;
    char tmp[MAX_VALUE] = {0};
    while (std::cin >> elem.key >> tmp) {
        unsigned int l = len;
        unsigned int r = l;
        for (int i = 0; i < MAX_VALUE && tmp[i] != 0; ++i) {
            allData[l+i] = tmp[i];
            ++r;
            tmp[i] = 0;
        }
        len += r - l + 1;
        elem.value = {l ,r};
        inputed[index] = elem;
        ++index;
    }
    radix_sort(inputed, index);
    for (int i = 0; i < index; ++i) {
        std::cout << inputed[i].key << "\t";
        for (int j = inputed[i].value.l; j < inputed[i].value.r; ++j) {
            std::cout << allData[j];
        }
        std::cout << "\n";
    }
    free(allData);
    free(inputed);
    return 0;
}
