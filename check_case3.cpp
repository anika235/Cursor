#include <bits/stdc++.h>
using namespace std;

int main() {
    // Case 3: n=6, k=1, s="010000", expected=Alice
    string s = "010000";
    int n = 6, k = 1;
    
    cout << "String: " << s << endl;
    
    int ones = 0;
    for (char c : s) if (c == '1') ones++;
    cout << "ones = " << ones << endl;
    
    if (ones <= k) {
        cout << "Alice wins immediately (ones <= k)" << endl;
        return 0;
    }
    
    // Check isolated 1s
    bool hasIsolated = false;
    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            bool isolated = true;
            if (i > 0 && s[i-1] == '1') isolated = false;
            if (i < n-1 && s[i+1] == '1') isolated = false;
            cout << "Position " << i << " has 1, isolated = " << isolated << endl;
            if (isolated) hasIsolated = true;
        }
    }
    
    cout << "hasIsolated = " << hasIsolated << endl;
    cout << "My answer: " << (hasIsolated ? "Alice" : "Bob") << endl;
    cout << "Expected: Alice" << endl;
    
    return 0;
}