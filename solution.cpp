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
        
        bool aliceWins = false;
        
        if (k == 1) {
            // Special case: k = 1
            // Alice wins if there exists at least one isolated 1
            for (int i = 0; i < n; i++) {
                if (s[i] == '1') {
                    bool isolated = true;
                    if (i > 0 && s[i-1] == '1') isolated = false;
                    if (i < n-1 && s[i+1] == '1') isolated = false;
                    if (isolated) {
                        aliceWins = true;
                        break;
                    }
                }
            }
        } else {
            // For k > 1, find the maximum number of 0s in any substring of length k
            int maxZerosInSubstring = 0;
            for (int i = 0; i <= n - k; i++) {
                int zeros = 0;
                for (int j = i; j < i + k; j++) {
                    if (s[j] == '0') zeros++;
                }
                maxZerosInSubstring = max(maxZerosInSubstring, zeros);
            }
            
            // Alice wins if she can remove 1s faster than Bob can add them
            // This happens when maxZerosInSubstring * 2 < k
            aliceWins = (maxZerosInSubstring * 2 < k);
        }
        
        cout << (aliceWins ? "Alice" : "Bob") << "\n";
    }
    
    return 0;
}