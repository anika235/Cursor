#include <bits/stdc++.h>
using namespace std;

void check(int n, int k, string s, string expected) {
    cout << "n=" << n << ", k=" << k << ", s=" << s << ", expected=" << expected << endl;
    
    int ones = 0;
    for (char c : s) if (c == '1') ones++;
    
    if (ones <= k) {
        cout << "Alice wins immediately" << endl;
        return;
    }
    
    int maxZeros = 0;
    for (int i = 0; i <= n - k; i++) {
        int zeros = 0;
        for (int j = i; j < i + k; j++) {
            if (s[j] == '0') zeros++;
        }
        maxZeros = max(maxZeros, zeros);
    }
    
    bool condition = (maxZeros * 2 <= k);
    cout << "maxZeros=" << maxZeros << ", k=" << k << ", maxZeros*2=" << (maxZeros*2) << ", condition=" << condition << endl;
    cout << "My answer: " << (condition ? "Alice" : "Bob") << ", Expected: " << expected << endl;
    cout << "------" << endl;
}

int main() {
    check(5, 2, "11011", "Bob");    // maxZeros=1, k=2, 1*2=2 <= 2 -> Alice, but expected Bob
    check(7, 4, "1011011", "Alice"); // maxZeros=2, k=4, 2*2=4 <= 4 -> Alice, expected Alice ✓
    check(6, 1, "010000", "Alice");  // ones=1 <= k=1 -> Alice immediately ✓
    check(4, 1, "1111", "Bob");      // k=1 case
    check(8, 3, "10110110", "Bob");  // maxZeros=1, k=3, 1*2=2 <= 3 -> Alice, but expected Bob
    check(6, 4, "111111", "Alice");  // maxZeros=0, k=4, 0*2=0 <= 4 -> Alice, expected Alice ✓
    
    return 0;
}