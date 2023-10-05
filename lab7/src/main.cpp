#include <iostream>
#include <vector>

using namespace std;

int64_t log(int64_t number) {
    if (number < 10)
        return 0;
    return log(number / 10) + 1;
}

int64_t power(int64_t exp) {
    if (exp == 0)
        return 1;
    return 10 * power(exp - 1);
}

int main() {
    int64_t n;
    int64_t m;
    cin >> n >> m;
    int64_t length_n = log(n) + 1;
    vector<int64_t> dp(length_n + 1);
    int64_t first_symbol;
    int64_t tmp_symbol = n / power(length_n - 1); // first_symbol
    int64_t pre_symbol = 1;
    dp[0] = tmp_symbol / m - pre_symbol / m;
    if (pre_symbol % m == 0) // m = 1
        ++dp[0];
    pre_symbol = 10;
    for (int64_t i = 1; i < length_n; ++i) {
        tmp_symbol = n / power(length_n - i - 1);
        dp[i] = dp[i - 1] + tmp_symbol / m - pre_symbol / m;
        if (pre_symbol % m == 0)
            ++dp[i];
        pre_symbol = power(i + 1);
    }
    if ((n % m == 0) && (dp[length_n - 1] > 0))
        dp[length_n - 1] -= 1;
    cout << dp[length_n - 1];
    return 0;
}