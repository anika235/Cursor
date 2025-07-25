#include <bits/stdc++.h>
using namespace std;

void analyze(int n, int k, string s, string expected) {
    cout << "n=" << n << ", k=" << k << ", s=" << s << ", expected=" << expected << endl;
    
    int ones = 0;
    for (char c : s) if (c == '1') ones++;
    cout << "ones=" << ones << endl;
    
    if (ones <= k) {
        cout << "Alice wins immediately (ones <= k)" << endl;
    }
    
    // maxZerosInWindow
    int maxZeros = 0;
    for (int i = 0; i <= n - k; i++) {
        int zeros = 0;
        for (int j = i; j < i + k; j++) {
            if (s[j] == '0') zeros++;
        }
        maxZeros = max(maxZeros, zeros);
    }
    cout << "maxZerosInWindow=" << maxZeros << endl;
    
    // Check for isolated 1s (for k=1)
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
        cout << "hasIsolated=" << hasIsolated << endl;
    }
    
    cout << "------" << endl;
}

int main() {
    analyze(5, 2, "11011", "Bob");
    analyze(7, 4, "1011011", "Alice");
    analyze(6, 1, "010000", "Alice");
    analyze(4, 1, "1111", "Bob");
    analyze(8, 3, "10110110", "Bob");
    analyze(6, 4, "111111", "Alice");
    
    return 0;
}