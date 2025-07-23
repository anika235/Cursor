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
        
        // Calculate initial beauty
        long long beauty = 0;
        for (int i = 0; i < n; i++) {
            beauty += countBits(a[i]);
        }
        
        if (k == 0) {
            cout << beauty << "\n";
            continue;
        }
        
        // For each element, find the best improvement
        vector<pair<int, long long>> improvements; // (gain, cost)
        
        for (int i = 0; i < n; i++) {
            long long x = a[i];
            int currentBits = countBits(x);
            
            int bestGain = 0;
            long long bestCost = 0;
            
            // Check reaching numbers of the form 2^n - 1 (maximum bit density)
            for (int bits = currentBits + 1; bits <= 60; bits++) {
                long long target = (1LL << bits) - 1;
                if (target > x) {
                    long long cost = target - x;
                    if (cost <= k) {
                        int gain = bits - currentBits;
                        if (gain > bestGain) {
                            bestGain = gain;
                            bestCost = cost;
                        }
                    }
                }
            }
            
            // Check small incremental improvements
            for (long long add = 1; add <= min(k, 100LL); add++) {
                long long target = x + add;
                int newBits = countBits(target);
                int gain = newBits - currentBits;
                if (gain > bestGain) {
                    bestGain = gain;
                    bestCost = add;
                }
            }
            
            if (bestGain > 0) {
                improvements.push_back({bestGain, bestCost});
            }
        }
        
        // Sort by gain (descending), then by cost (ascending) for tie-breaking
        sort(improvements.begin(), improvements.end(), [](const pair<int, long long>& a, const pair<int, long long>& b) {
            if (a.first != b.first) return a.first > b.first;
            return a.second < b.second;
        });
        
        // Apply improvements greedily
        long long remainingK = k;
        for (auto [gain, cost] : improvements) {
            if (cost <= remainingK) {
                beauty += gain;
                remainingK -= cost;
            }
        }
        
        cout << beauty << "\n";
    }
    
    return 0;
}