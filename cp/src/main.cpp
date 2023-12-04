#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct fch {
    char symbol;
    int number_original;
    int number_sorted;
};

void bwt_transformer(const string &text, string &result) {
    vector<int> shift;
    shift.reserve(text.size());
    for (int i = 0; i < text.size(); ++i) {
        shift.push_back(i);
    }
    sort(shift.begin(), shift.end(), [&text](const int &element1, const int &element2) {
        int n = text.size();
        for (int i = 0; i < n; ++i) {
            if (text[(element1 + i) % n] < text[(element2 + i) % n])
                return true;
            if (text[(element1 + i) % n] > text[(element2 + i) % n])
                return false;
        }
        return false;
    });
    int n = text.size();
    for (int i = 0; i < shift.size(); ++i) {
        result.push_back(text[(shift[i] + n - 1) % n]);
    }
}

bool cmp(const fch &element1, const fch &element2) { // element1 < element2 ?
    if (element1.symbol < element2.symbol)
        return true;
    return false;
}

void bwt_undertransformer(const string &text, string &result) {
    vector<fch> left, tmp_right, right(text.size());
    left.reserve(text.size());
    tmp_right.reserve(text.size());
    string tmp_text = text;
    sort(tmp_text.begin(), tmp_text.end());
    for (int i = 0; i < text.size(); ++i) {
        left.push_back({tmp_text[i], 0, i});
        tmp_right.push_back({text[i], i, 0});
    }
    stable_sort(tmp_right.begin(), tmp_right.end(), cmp);
    for (int i = 0; i < tmp_right.size(); ++i) {
        int index = tmp_right[i].number_original;
        right[index] = {tmp_right[i].symbol, index, i};
    }
    for (int i = 0; i < right.size(); ++i) {
        int index = right[i].number_sorted;
        left[index].number_original = right[i].number_original;
    }
    result.push_back('0'); // любой символ
    int tmp_index = right.size() - 1;
    for (int i = 0; i < text.size() - 1; ++i) {
        result.push_back(right[tmp_index].symbol);
        tmp_index = left[tmp_index].number_original;
    }
    result[0] = right[tmp_index].symbol; // зациклили
    int index_dollar = 0;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] == '$') {
            index_dollar = i;
            break;
        }
    }
    string tmp_string;
    tmp_string.reserve(text.size());
    for (int j = index_dollar + 1; j < text.size(); ++j) {
        tmp_string.push_back(result[j]);
    }
    for (int k = 0; k < index_dollar; ++k) {
        tmp_string.push_back(result[k]);
    }
    result = tmp_string;
}

void mtf_transformer(const string &text, vector<int> &result) {
    vector<char> alphabet;
    alphabet.reserve(50);
    alphabet.push_back('$');
    for (int i = 1; i < 27; ++i) {
        alphabet.push_back((char)(96 + i));
    }
    for (int i = 0; i < text.size(); ++i) {
        char tmp_char = text[i];
        int index = 0;
        for (int j = 0; j < alphabet.size(); ++j) {
            if (alphabet[j] == tmp_char) {
                index = j;
                break;
            }
        }
        result.push_back(index);
        for (int i = index; i > 0; --i) {
            alphabet[i] = alphabet[i - 1];
        }
        alphabet[0] = tmp_char;
    }
}

void mtf_undertransformer(const vector<int> &text, string &result) {
    vector<char> alphabet;
    alphabet.reserve(50);
    alphabet.push_back('$');
    for (int i = 1; i < 27; ++i) {
        alphabet.push_back((char)(96 + i));
    }
    for (int i = 0; i < text.size(); ++i) {
        int index = text[i];
        result.push_back(alphabet[index]);
        char tmp_char = alphabet[index];
        for (int i = index; i > 0; --i) {
            alphabet[i] = alphabet[i - 1];
        }
        alphabet[0] = tmp_char;
    }
}

void rle_coder(const vector<int> &text, vector<pair<int, int>> &result) {
    int tmp_symbol = text[0];
    int counter = 1;
    for (int i = 1; i < text.size(); ++i) {
        if (text[i] == tmp_symbol) {
            ++counter;
        } else {
            result.push_back(make_pair(counter, tmp_symbol));
            tmp_symbol = text[i];
            counter = 1;
        }
    }
    result.push_back(make_pair(counter, tmp_symbol));
}

void rle_decoder(const vector<pair<int, int>> &text, vector<int> &result) {
    for (int i = 0; i < text.size(); ++i) {
        for (int j = 0; j < text[i].first; ++j) {
            result.push_back(text[i].second);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    string type, text, result_btw, result_mtf_undertransformer, result;
    vector<int> result_mtf, result_rle_decoder;
    vector<pair<int, int>> result_rle, input;
    cin >> type;
    if (type == "compress") {
        cin >> text;
        text.push_back('$');
        result_btw.reserve(text.size());
        result_mtf.reserve(text.size());
        result_rle.reserve(text.size());
        bwt_transformer(text, result_btw);
        mtf_transformer(result_btw, result_mtf);
        rle_coder(result_mtf, result_rle);
        for (int i = 0; i < result_rle.size(); ++i) {
            cout << result_rle[i].first << " " << result_rle[i].second << "\n";
        }
    }
    if (type == "decompress") {
        int tmp1, tmp2;
        while (cin >> tmp1 >> tmp2) {
            input.push_back(make_pair(tmp1, tmp2));
        }
        result_rle_decoder.reserve(input.size());
        result_mtf_undertransformer.reserve(input.size());
        result.reserve(input.size());
        rle_decoder(input, result_rle_decoder);
        mtf_undertransformer(result_rle_decoder, result_mtf_undertransformer);
        bwt_undertransformer(result_mtf_undertransformer, result);
        cout << result << "\n";
    }
    return 0;
}