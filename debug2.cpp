#include <bits/stdc++.h>
using namespace std;

int main() {
    string s = "11011";
    int n = 5, k = 2;
    
    cout << "String: " << s << endl;
    cout << "n=" << n << ", k=" << k << endl;
    
    // Check maxZerosInWindow
    int maxZerosInWindow = 0;
    for (int i = 0; i <= n - k; i++) {
        int zeros = 0;
        for (int j = i; j < i + k; j++) {
            if (s[j] == '0') zeros++;
        }
        cout << "Window [" << i << "," << (i+k-1) << "]: ";
        for (int j = i; j < i + k; j++) {
            cout << s[j];
        }
        cout << " has " << zeros << " zeros" << endl;
        maxZerosInWindow = max(maxZerosInWindow, zeros);
    }
    cout << "maxZerosInWindow = " << maxZerosInWindow << endl;
    cout << "k = " << k << endl;
    cout << "maxZerosInWindow < k? " << (maxZerosInWindow < k) << endl;
    cout << "So Alice wins: " << (maxZerosInWindow < k ? "Yes" : "No") << endl;
    cout << "Expected: Bob wins" << endl;
    
    return 0;
}