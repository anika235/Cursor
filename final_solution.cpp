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
        
        // Count ones
        int ones = 0;
        for (char c : s) {
            if (c == '1') ones++;
        }
        
        // If no ones, Alice wins
        if (ones == 0) {
            cout << "Alice\n";
            continue;
        }
        
        // If Alice can win in one move
        if (ones <= k) {
            cout << "Alice\n";
            continue;
        }
        
        // For this specific problem, the key insight is:
        // Alice wins if and only if she can guarantee to make progress
        // faster than Bob can counteract her moves
        
        bool aliceWins = false;
        
        if (k == 1) {
            // Special case: k=1
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
            // For k > 1, the winning condition is based on the maximum
            // number of zeros Bob can turn into ones in a single move
            
            int maxZerosInWindow = 0;
            for (int i = 0; i <= n - k; i++) {
                int zeros = 0;
                for (int j = i; j < i + k; j++) {
                    if (s[j] == '0') zeros++;
                }
                maxZerosInWindow = max(maxZerosInWindow, zeros);
            }
            
            // The key game theory result:
            // Alice wins if the maximum zeros Bob can create is
            // less than half of what Alice can remove
            // This is equivalent to: maxZerosInWindow < k/2
            // Or more precisely: maxZerosInWindow * 2 < k
            
            aliceWins = (maxZerosInWindow * 2 < k);
        }
        
        cout << (aliceWins ? "Alice" : "Bob") << "\n";
    }
    
    return 0;
}