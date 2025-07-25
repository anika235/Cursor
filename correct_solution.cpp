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
            // Check if we can achieve sum m with this root
            // Key insight: nodes can be arranged in a chain to control the sum
            
            // If we arrange nodes 1,2,...,root-1,root+1,...,n in a chain starting from root,
            // we can control how much each contributes
            
            long long sum = root; // root contributes itself
            vector<pair<int, int>> edges;
            
            // Collect all non-root nodes
            vector<int> others;
            for (int i = 1; i <= n; i++) {
                if (i != root) {
                    others.push_back(i);
                }
            }
            
            // Strategy: build a chain to get exactly the target sum
            // Start from root and add nodes one by one
            
            long long target = m;
            long long current_sum = root;
            int parent = root;
            
            bool possible = true;
            
            for (int i = 0; i < others.size() && possible; i++) {
                int node = others[i];
                long long remaining = target - current_sum;
                long long remaining_nodes = others.size() - i;
                
                // Calculate what this node contributes if added to chain
                long long contrib = min(parent, node);
                if (parent != root) {
                    contrib = min(contrib, (long long)root);
                }
                
                // Check if we can still achieve the target
                if (remaining >= contrib && remaining - contrib <= (remaining_nodes - 1) * root) {
                    // Add to chain
                    edges.push_back({parent, node});
                    current_sum += contrib;
                    parent = node;
                } else {
                    // Try connecting to root instead
                    contrib = min((long long)root, (long long)node);
                    if (remaining >= contrib && remaining - contrib <= (remaining_nodes - 1) * root) {
                        edges.push_back({root, node});
                        current_sum += contrib;
                        parent = root;
                    } else {
                        possible = false;
                    }
                }
            }
            
            if (possible && current_sum == target) {
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