#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BigInteger {
private:
    static const int64_t DIGIT_SIZE = 3;
    static const int64_t DIGIT = 1e3;

    vector<int64_t> data;

    void RemoveLeadingZeros() {
        reverse(data.begin(), data.end());
        for (int64_t i = data.size() - 1; i > 0; --i) {
            if (data[i] == 0)
                data.pop_back();
            else
                break;
        }
        reverse(data.begin(), data.end());
    }

    void CarrySum() {
        reverse(data.begin(), data.end());
        for (int i = 0; i < data.size() - 1; ++i) {
            if (data[i] > DIGIT - 1) {
                data[i + 1] += data[i] / DIGIT;
                data[i] = data[i] % DIGIT;
            }
        }
        if (data[data.size() - 1] > DIGIT - 1) {
            int64_t tmp = data[data.size() - 1] / DIGIT;
            data[data.size() - 1] = data[data.size() - 1] % DIGIT;
            data.push_back(tmp);
        }
        reverse(data.begin(), data.end());
    }

    void CarryDif() {
        for (int i = data.size() - 1; i > 0; --i) {
            if (data[i] < 0) {
                int64_t tmp = abs(data[i]) + DIGIT - 1;
                data[i - 1] -= tmp / DIGIT;
                data[i] += (tmp / DIGIT) * DIGIT;
            }
        }
    }

    static BigInteger Mul(BigInteger num, int64_t c) {
        for (int64_t i = num.data.size() - 1; i >= 0; --i) {
            num.data[i] = num.data[i] * c;
        }
        num.CarrySum();
        num.RemoveLeadingZeros();
        return num;
    }

    static void EqualazingDigits(BigInteger &num1, BigInteger &num2) {
        int64_t size1 = num1.data.size();
        int64_t size2 = num2.data.size();
        int64_t dif = abs(size1 - size2);
        if (size1 > size2) {
            reverse(num2.data.begin(), num2.data.end());
            while (dif > 0) {
                num2.data.push_back(0);
                --dif;
            }
            reverse(num2.data.begin(), num2.data.end());
        }
        if (size1 < size2) {
            reverse(num1.data.begin(), num1.data.end());
            while (dif > 0) {
                num1.data.push_back(0);
                --dif;
            }
            reverse(num1.data.begin(), num1.data.end());
        }
    }

    static int64_t BinarySearch(const BigInteger &num2, const BigInteger &local_divisble) {
        int64_t left = 0;
        int64_t right = DIGIT + 1;
        while (right - left > 1) {
            int64_t mid = (right + left) / 2;
            BigInteger tmp = Mul(num2, mid);
            if ((local_divisble > tmp) || (tmp == local_divisble)) {
                left = mid;
            } else {
                right = mid;
            }
        }
        return left;
    }

    static void DivMod(BigInteger &num1, BigInteger &num2, BigInteger &div, BigInteger &mod) {
        if (num2 > num1) {
            div.CreateIntFromString("0");
            mod = num1;
        } else {
            div.data.clear();
            mod.data.clear();
            BigInteger local_divisble;
            int64_t size_num2 = num2.data.size();
            int64_t local_res;
            int64_t i = 0;
            while (i < num1.data.size()) {
                local_divisble.data.push_back(num1.data[i]);
                ++i;
                local_divisble.RemoveLeadingZeros();
                if (num2 > local_divisble) {
                    div.data.push_back(0);
                    continue;
                }
                local_res = BinarySearch(num2, local_divisble);
                div.data.push_back(local_res);
                BigInteger dif = Mul(num2, local_res);
                local_divisble = local_divisble - dif;
            }
            mod = local_divisble;
        }
        mod.RemoveLeadingZeros();
        div.RemoveLeadingZeros();
    }

public:
    bool IsZero() const {
        if (data[0] == 0)
            return true;
        else
            return false;
    }

    void CreateIntFromString(const string &number) {
        if (number.empty()) {
            data.clear();
            return;
        }
        data.resize((number.size() + DIGIT_SIZE - 1) / DIGIT_SIZE);
        int64_t length_first_block = number.size() % DIGIT_SIZE;
        if (length_first_block == 0)
            length_first_block = DIGIT_SIZE;
        data[0] = stoi(number.substr(0, length_first_block));
        for (int64_t i = data.size() - 1; i > 0; --i) {
            data[i] = stoi(number.substr(i * DIGIT_SIZE - (DIGIT_SIZE - length_first_block), DIGIT_SIZE));
        }
    }

    friend bool operator==(const BigInteger &num1, const BigInteger &num2) {
        return (num1.data == num2.data);
    }

    friend bool operator>(const BigInteger &num1, const BigInteger &num2) {
        if (num1.data.size() > num2.data.size())
            return true;
        else if (num1.data.size() < num2.data.size())
            return false;
        else {
            for (int64_t i = 0; i < num1.data.size(); ++i) {
                if (num1.data[i] > num2.data[i])
                    return true;
                if (num1.data[i] < num2.data[i])
                    return false;
            }
            return false;
        }
    }

    friend BigInteger operator+(BigInteger &num1, BigInteger &num2) {
        BigInteger result;
        EqualazingDigits(num1, num2);
        result.data.resize(num1.data.size());
        for (int64_t i = 0; i < num1.data.size(); ++i) {
            result.data[i] = num1.data[i] + num2.data[i];
        }
        result.CarrySum();
        return result;
    }

    friend BigInteger operator-(BigInteger &num1, BigInteger &num2) {
        BigInteger result;
        EqualazingDigits(num1, num2);
        result.data.resize(num1.data.size());
        for (int64_t i = 0; i < num1.data.size(); ++i) {
            result.data[i] = num1.data[i] - num2.data[i];
        }
        result.CarryDif();
        result.RemoveLeadingZeros();
        return result;
    }

    friend BigInteger operator*(BigInteger &num1, BigInteger &num2) {
        BigInteger result;
        result.data.resize(num1.data.size() + num2.data.size() - 1);
        for (int64_t j = 0; j < result.data.size(); ++j) {
            result.data[j] = 0;
        }
        for (int64_t i = num2.data.size() - 1; i >= 0; --i) {
            for (int64_t k = num1.data.size() - 1; k >= 0; --k) {
                result.data[result.data.size() - (num1.data.size() - 1 - i + num2.data.size() - 1 - k) - 1] += num1.data[k] * num2.data[i];
            }
        }
        result.CarrySum();
        result.RemoveLeadingZeros();
        return result;
    }

    friend BigInteger operator/(BigInteger &num1, BigInteger &num2) {
        BigInteger div, mod;
        DivMod(num1, num2, div, mod);
        return div;
    }

    friend BigInteger operator%(BigInteger &num1, BigInteger &num2) {
        BigInteger div, mod;
        DivMod(num1, num2, div, mod);
        return mod;
    }

    friend BigInteger operator^(BigInteger &num, BigInteger &exp) {
        BigInteger ZERO;
        ZERO.CreateIntFromString("0");
        BigInteger ONE;
        ONE.CreateIntFromString("1");
        BigInteger TWO;
        TWO.CreateIntFromString("2");
        BigInteger res;
        res.CreateIntFromString("1");
        while (exp > ZERO) {
            BigInteger mod = exp % TWO;
            if (mod.IsZero()) {
                num = num * num;
                exp = exp / TWO;
            } else {
                res = res * num;
                exp = exp - ONE;
            }
        }
        return res;
    }

    BigInteger &operator=(const BigInteger &num1) {
        data = num1.data;
        return *this;
    }

    // friend ostream &operator<<(ostream &out, const BigInteger &num) {
    //     out << "|";
    //     for (int64_t i = 0; i < num.data.size(); ++i) {
    //         out << num.data[i] << "|";
    //     }
    //     out << endl;
    //     return out;
    // }

    friend ostream &operator<<(ostream &out, const BigInteger &num) {
        out << num.data[0];
        for (int64_t i = 1; i < num.data.size(); ++i) {
            if (num.data[i] < 10)
                out << "00" << num.data[i];
            else if (num.data[i] < 100)
                out << "0" << num.data[i];
            else
                out << num.data[i];
        }
        out << endl;
        return out;
    }

    friend istream &operator>>(istream &in, BigInteger &num) {
        string tmp;
        in >> tmp;
        num.CreateIntFromString(tmp);
        num.RemoveLeadingZeros();
        return in;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    BigInteger num1, num2;
    char op;
    while (cin >> num1 >> num2 >> op) {
        switch (op) {
        case '=':
            if ((num1 == num2) == false)
                cout << "false\n";
            else
                cout << "true\n";
            break;
        case '>':
            if ((num1 > num2) == false)
                cout << "false\n";
            else
                cout << "true\n";
            break;
        case '<':
            if ((num2 > num1) == false)
                cout << "false\n";
            else
                cout << "true\n";
            break;
        case '+':
            cout << (num1 + num2);
            break;
        case '-':
            if (num2 > num1)
                cout << "Error\n";
            else
                cout << (num1 - num2);
            break;
        case '*':
            cout << (num1 * num2);
            break;
        case '/':
            if (num2.IsZero())
                cout << "Error\n";
            else
                cout << (num1 / num2);
            break;
        case '%':
            if (num2.IsZero())
                cout << "Error\n";
            else
                cout << (num1 % num2);
            break;
        case '^':
            if ((num1.IsZero()) && (num2.IsZero()))
                cout << "Error\n";
            else
                cout << (num1 ^ num2);
            break;
        }
    }
    return 0;
}