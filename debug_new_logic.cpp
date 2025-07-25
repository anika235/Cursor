#include <bits/stdc++.h>
using namespace std;

void debug_case(int n, int k, string s, string expected) {
    cout << "n=" << n << ", k=" << k << ", s=" << s << ", expected=" << expected << endl;
    
    int ones = 0;
    for (char c : s) if (c == '1') ones++;
    cout << "ones=" << ones << endl;
    
    if (ones == 0) {
        cout << "Alice wins (no ones)" << endl;
        return;
    }
    
    if (ones <= k) {
        cout << "Alice wins (ones <= k): " << ones << " <= " << k << endl;
        return;
    }
    
    if (k == 0) {
        cout << "k=0, ones % 2 = " << (ones % 2) << " → " << (ones % 2 ? "Alice" : "Bob") << endl;
        return;
    }
    
    cout << "ones > k: " << ones << " > " << k << endl;
    
    if (k == 1) {
        cout << "k=1, ones % 2 = " << (ones % 2) << endl;
        if (ones % 2 == 0) {
            cout << "My answer: Bob" << endl;
        } else {
            cout << "My answer: Alice" << endl;
        }
    } else {
        int first = n, last = -1;
        for (int i = 0; i < n; i++) {
            if (s[i] == '1') {
                first = min(first, i);
                last = i;
            }
        }
        int span = last - first + 1;
        cout << "k>1, first=" << first << ", last=" << last << ", span=" << span << ", 2*k=" << (2*k) << endl;
        if (span <= 2 * k) {
            cout << "My answer: Alice (span <= 2*k)" << endl;
        } else {
            cout << "My answer: Bob (span > 2*k)" << endl;
        }
    }
    
    cout << "Expected: " << expected << endl;
    cout << "--------" << endl;
}

int main() {
    debug_case(5, 2, "11011", "Bob");
    debug_case(7, 4, "1011011", "Alice");
    debug_case(6, 1, "010000", "Alice");
    debug_case(4, 1, "1111", "Bob");
    debug_case(8, 3, "10110110", "Bob");
    debug_case(6, 4, "111111", "Alice");
    
    return 0;
}