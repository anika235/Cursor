#include <bits/stdc++.h>
using namespace std;

int main() {
    // Test case 1: n=5, k=2, s="11011"
    // Expected: Bob wins
    
    string s = "11011";
    int n = 5, k = 2;
    
    cout << "Initial string: " << s << endl;
    cout << "n=" << n << ", k=" << k << endl;
    
    // Count 1s
    int ones = 0;
    for (char c : s) {
        if (c == '1') ones++;
    }
    cout << "Number of 1s: " << ones << endl;
    
    // Alice can choose any 2 characters and set to 0
    // Bob can choose any 2 consecutive characters and set to 1
    
    cout << "\nAlice's possible moves (subsequences of length 2):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            string temp = s;
            temp[i] = '0';
            temp[j] = '0';
            cout << "Choose positions " << i << "," << j << ": " << temp << endl;
        }
    }
    
    cout << "\nBob's possible moves (substrings of length 2):" << endl;
    for (int i = 0; i <= n - k; i++) {
        string temp = s;
        for (int j = i; j < i + k; j++) {
            temp[j] = '1';
        }
        cout << "Choose substring [" << i << "," << (i+k-1) << "]: " << temp << endl;
    }
    
    // Let's analyze the maximum zeros Bob can turn to 1s
    int maxZeros = 0;
    for (int i = 0; i <= n - k; i++) {
        int zeros = 0;
        for (int j = i; j < i + k; j++) {
            if (s[j] == '0') zeros++;
        }
        cout << "Substring [" << i << "," << (i+k-1) << "] has " << zeros << " zeros" << endl;
        maxZeros = max(maxZeros, zeros);
    }
    cout << "Max zeros Bob can turn to 1s: " << maxZeros << endl;
    
    return 0;
}