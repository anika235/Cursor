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
        
        // Try each root from 1 to n
        for (int root = 1; root <= n && !found; root++) {
            // Calculate achievable sum range with this root
            long long min_sum = 0;
            for (int i = 1; i <= n; i++) {
                min_sum += min((long long)root, (long long)i);
            }
            
            long long max_sum = (long long)root * n;
            
            if (m < min_sum || m > max_sum) continue;
            
            // We can potentially achieve m with this root
            // Try to construct the tree by building a specific chain
            
            long long sum_needed = m;
            vector<pair<int, int>> edges;
            
            // Root contributes root to the sum
            sum_needed -= root;
            
            // Strategy: Create a chain starting from root and going through nodes
            // to achieve exactly the remaining sum
            
            vector<int> chain_nodes;
            for (int i = 1; i <= n; i++) {
                if (i != root) {
                    chain_nodes.push_back(i);
                }
            }
            
            // Try to build a chain to get exactly sum_needed
            long long current_remaining = sum_needed;
            int current_parent = root;
            bool success = true;
            
            for (int node : chain_nodes) {
                if (current_remaining <= 0) {
                    // Connect remaining nodes to root with 0 contribution if possible
                    long long contrib = min((long long)root, (long long)node);
                    if (contrib == 0) {
                        edges.push_back({root, node});
                    } else {
                        success = false;
                        break;
                    }
                } else {
                    // Calculate contribution if connected to current parent
                    long long contrib = min((long long)current_parent, (long long)node);
                    if (current_parent != root) {
                        contrib = min(contrib, (long long)root);
                    }
                    
                    if (contrib <= current_remaining) {
                        edges.push_back({current_parent, node});
                        current_remaining -= contrib;
                        current_parent = node;
                    } else {
                        // Try connecting to root instead
                        contrib = min((long long)root, (long long)node);
                        if (contrib <= current_remaining) {
                            edges.push_back({root, node});
                            current_remaining -= contrib;
                        } else {
                            success = false;
                            break;
                        }
                    }
                }
            }
            
            if (success && current_remaining == 0) {
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