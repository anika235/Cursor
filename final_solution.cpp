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
        
        // Try each possible root
        for (int root = 1; root <= n && !found; root++) {
            // Calculate the range of possible sums with this root
            
            // Minimum sum: star topology where each node contributes min(root, node)
            long long min_sum = 0;
            for (int i = 1; i <= n; i++) {
                min_sum += min((long long)root, (long long)i);
            }
            
            // Maximum sum: chain topology where smaller nodes get divineness = root
            long long max_sum = (long long)root * n;
            
            if (m < min_sum || m > max_sum) continue;
            
            // Try to construct a solution with this root
            // We need to achieve exactly sum = m
            
            // Simple approach: use a chain from root through nodes 1,2,3,...
            // until we get the desired sum
            
            long long current_sum = root; // root contributes itself
            long long remaining = m - current_sum;
            
            vector<pair<int, int>> edges;
            int current_parent = root;
            
            // Process nodes in order 1, 2, ..., n (excluding root)
            for (int node = 1; node <= n; node++) {
                if (node == root) continue;
                
                // Option 1: connect to root -> contributes min(root, node)
                long long contrib_root = min((long long)root, (long long)node);
                
                // Option 2: connect to current chain -> might give different contribution
                long long contrib_chain = min((long long)current_parent, (long long)node);
                if (current_parent != root) {
                    contrib_chain = min(contrib_chain, (long long)root);
                }
                
                // Use the option that gets us closer to the target
                if (contrib_chain <= remaining) {
                    edges.push_back({current_parent, node});
                    remaining -= contrib_chain;
                    current_parent = node;
                } else if (contrib_root <= remaining) {
                    edges.push_back({root, node});
                    remaining -= contrib_root;
                    current_parent = root; // future nodes connect to root
                } else {
                    // Neither option works exactly, just connect to root
                    edges.push_back({root, node});
                    remaining -= contrib_root;
                }
            }
            
            if (remaining == 0) {
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