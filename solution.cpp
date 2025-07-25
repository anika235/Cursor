#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n, k;
        cin >> n >> k;
        string s;
        cin >> s;
        
        // Count the number of 1s
        int ones = 0;
        for (char c : s) {
            if (c == '1') ones++;
        }
        
        // If there are no 1s, Alice already wins
        if (ones == 0) {
            cout << "Alice\n";
            continue;
        }
        
        // If Alice can eliminate all 1s in one move
        if (ones <= k) {
            cout << "Alice\n";
            continue;
        }
        
        // Handle edge case for k = 0
        if (k == 0) {
            cout << (ones % 2 ? "Alice" : "Bob") << "\n";
            continue;
        }
        
        // Handle cases where ones > k
        if (k == 1) {
            if (ones % 2 == 0) {
                cout << "Bob\n";
            } else {
                cout << "Alice\n";
            }
        } else {
            int first = n, last = -1;
            for (int i = 0; i < n; i++) {
                if (s[i] == '1') {
                    first = min(first, i);
                    last = i;
                }
            }
            if (last - first + 1 <= 2 * k) {
                cout << "Alice\n";
            } else {
                cout << "Bob\n";
            }
        }
    }
    
    return 0;
}