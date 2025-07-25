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
        cout << "Alice wins (ones <= k)" << endl;
        return;
    }
    
    if (k == 1) {
        bool hasIsolated = false;
        for (int i = 0; i < n; i++) {
            if (s[i] == '1') {
                bool isolated = true;
                if (i > 0 && s[i-1] == '1') isolated = false;
                if (i < n-1 && s[i+1] == '1') isolated = false;
                if (isolated) hasIsolated = true;
            }
        }
        cout << "k=1, hasIsolated=" << hasIsolated << endl;
        cout << "My answer: " << (hasIsolated ? "Alice" : "Bob") << endl;
    } else {
        int hits = (n - 1) / k + 1;
        bool aliceWins = (hits < k);
        cout << "k>1, hits=" << hits << ", k=" << k << ", hits<k=" << aliceWins << endl;
        cout << "My answer: " << (aliceWins ? "Alice" : "Bob") << endl;
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