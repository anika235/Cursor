#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        long long n, m;
        cin >> n >> m;
        
        bool found = false;
        
        for (int root = 1; root <= n && !found; root++) {
            // Try to construct a tree with this root that achieves sum = m
            
            // First, calculate bounds
            long long min_possible = 0, max_possible = 0;
            for (int i = 1; i <= n; i++) {
                min_possible += min((long long)root, (long long)i);
                max_possible += root;
            }
            
            if (m < min_possible || m > max_possible) continue;
            
            // Try to construct the tree
            // Use a greedy approach: build chains to reach exactly m
            
            vector<pair<int, int>> edges;
            long long current_sum = root; // root's contribution
            
            // Process other nodes in ascending order
            vector<int> others;
            for (int i = 1; i <= n; i++) {
                if (i != root) others.push_back(i);
            }
            
            // Build tree greedily
            int chain_end = root;
            
            for (int node : others) {
                long long target_remaining = m - current_sum;
                
                // Option 1: connect to root
                long long contrib_root = min((long long)root, (long long)node);
                
                // Option 2: connect to chain end
                long long contrib_chain = min((long long)chain_end, (long long)node);
                if (chain_end != root) {
                    contrib_chain = min(contrib_chain, (long long)root);
                }
                
                // Choose the better option
                if (target_remaining == contrib_chain) {
                    // Perfect match with chain
                    edges.push_back({chain_end, node});
                    current_sum += contrib_chain;
                    chain_end = node;
                } else if (target_remaining == contrib_root) {
                    // Perfect match with root
                    edges.push_back({root, node});
                    current_sum += contrib_root;
                } else if (target_remaining > contrib_chain) {
                    // Use chain (allows for more flexibility later)
                    edges.push_back({chain_end, node});
                    current_sum += contrib_chain;
                    chain_end = node;
                } else if (target_remaining > contrib_root) {
                    // Use root
                    edges.push_back({root, node});
                    current_sum += contrib_root;
                } else {
                    // Neither works - this root won't work
                    break;
                }
            }
            
            if (current_sum == m && edges.size() == n - 1) {
                found = true;
                cout << root << "\n";
                for (auto& edge : edges) {
                    cout << edge.first << " " << edge.second << "\n";
                }
            }
        }
        
        if (!found) {
            cout << "-1\n";
        }
    }
    
    return 0;
}