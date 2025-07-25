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
            // With ones >= 2, Alice cannot win
            aliceWins = false;
        } else {
            // For k > 1, use minimum hitting set size
            int hits = (n - 1) / k + 1;
            aliceWins = (hits < k);
        }
        
        cout << (aliceWins ? "Alice" : "Bob") << "\n";
    }
    
    return 0;
}