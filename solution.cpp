#include <bits/stdc++.h>
using namespace std;

int countBits(long long x) {
    return __builtin_popcountll(x);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        long long k;
        cin >> n >> k;
        
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // Use step-by-step greedy approach for all cases
        priority_queue<pair<int, int>> pq; // (beauty_gain, element_index)
        
        // Initialize with beauty gains for incrementing each element by 1
        for (int i = 0; i < n; i++) {
            int gain = countBits(a[i] + 1) - countBits(a[i]);
            if (gain > 0) {
                pq.push({gain, i});
            }
        }
        
        // Apply operations greedily
        for (long long ops = 0; ops < k && !pq.empty(); ops++) {
            auto [gain, idx] = pq.top();
            pq.pop();
            
            // Apply the operation
            a[idx]++;
            
            // Recalculate gain for this element and add back if positive
            int newGain = countBits(a[idx] + 1) - countBits(a[idx]);
            if (newGain > 0) {
                pq.push({newGain, idx});
            }
        }
        
        // Calculate final beauty
        long long beauty = 0;
        for (int i = 0; i < n; i++) {
            beauty += countBits(a[i]);
        }
        
        cout << beauty << "\n";
    }
    
    return 0;
}