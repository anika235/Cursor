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
        
        long long beauty = 0;
        
        if (k <= 5000) {
            // For small k, use greedy approach with priority queue
            // This ensures we always make the most beneficial single operation
            priority_queue<pair<int, int>> pq; // (beauty_gain, element_index)
            
            // Initialize with beauty gains for incrementing each element by 1
            for (int i = 0; i < n; i++) {
                int gain = countBits(a[i] + 1) - countBits(a[i]);
                if (gain > 0) {
                    pq.push({gain, i});
                }
            }
            
            // Apply operations greedily, always choosing the best single operation
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
            for (int i = 0; i < n; i++) {
                beauty += countBits(a[i]);
            }
        } else {
            // For large k, find optimal transformations directly
            // Focus on reaching numbers with maximum bit density (2^n - 1)
            
            // Calculate initial beauty
            for (int i = 0; i < n; i++) {
                beauty += countBits(a[i]);
            }
            
            // For each element, find the best transformation within budget k
            vector<pair<int, long long>> best_gains; // (max_gain, cost)
            
            for (int i = 0; i < n; i++) {
                long long x = a[i];
                int baseBits = countBits(x);
                int maxGain = 0;
                long long bestCost = 0;
                
                // Check numbers of the form 2^n - 1 (all bits set)
                // These have maximum bit density
                for (int bits = 1; bits <= 60; bits++) {
                    long long target = (1LL << bits) - 1;
                    if (target > x) {
                        long long cost = target - x;
                        if (cost <= k) {
                            int newBits = bits;
                            int gain = newBits - baseBits;
                            if (gain > maxGain) {
                                maxGain = gain;
                                bestCost = cost;
                            }
                        }
                    }
                }
                
                if (maxGain > 0) {
                    best_gains.push_back({maxGain, bestCost});
                }
            }
            
            // Sort by gain (descending), then by cost (ascending)
            sort(best_gains.begin(), best_gains.end(), [](const pair<int, long long>& a, const pair<int, long long>& b) {
                if (a.first != b.first) return a.first > b.first;
                return a.second < b.second;
            });
            
            // Apply transformations greedily
            long long remaining = k;
            for (auto [gain, cost] : best_gains) {
                if (cost <= remaining) {
                    beauty += gain;
                    remaining -= cost;
                }
            }
        }
        
        cout << beauty << "\n";
    }
    
    return 0;
}