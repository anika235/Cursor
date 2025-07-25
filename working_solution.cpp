#include <iostream>
#include <vector>
#include <algorithm>
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
            // For this root, try different arrangements of other nodes
            vector<int> others;
            for (int i = 1; i <= n; i++) {
                if (i != root) {
                    others.push_back(i);
                }
            }
            
            // Try all permutations of others to find one that works
            sort(others.begin(), others.end());
            
            do {
                vector<pair<int, int>> edges;
                long long total_sum = root;
                
                int current_parent = root;
                bool valid = true;
                
                for (int node : others) {
                    long long need = m - total_sum;
                    
                    // Try adding to chain
                    long long contrib_chain = min(current_parent, node);
                    if (current_parent != root) {
                        contrib_chain = min(contrib_chain, (long long)root);
                    }
                    
                    // Try connecting to root
                    long long contrib_root = min((long long)root, (long long)node);
                    
                    if (contrib_chain == need) {
                        // Perfect match with chain
                        edges.push_back({current_parent, node});
                        total_sum += contrib_chain;
                        current_parent = node;
                    } else if (contrib_root == need) {
                        // Perfect match with root
                        edges.push_back({root, node});
                        total_sum += contrib_root;
                        break; // All remaining nodes should connect to root
                    } else if (contrib_chain < need) {
                        // Use chain and continue
                        edges.push_back({current_parent, node});
                        total_sum += contrib_chain;
                        current_parent = node;
                    } else if (contrib_root < need) {
                        // Use root connection
                        edges.push_back({root, node});
                        total_sum += contrib_root;
                    } else {
                        // Neither works
                        valid = false;
                        break;
                    }
                }
                
                // Connect any remaining nodes to root
                for (int i = edges.size(); i < others.size(); i++) {
                    int node = others[i];
                    long long need = m - total_sum;
                    long long contrib = min((long long)root, (long long)node);
                    
                    if (contrib == need) {
                        edges.push_back({root, node});
                        total_sum += contrib;
                        break;
                    } else if (contrib < need) {
                        edges.push_back({root, node});
                        total_sum += contrib;
                    } else {
                        valid = false;
                        break;
                    }
                }
                
                if (valid && total_sum == m && edges.size() == n - 1) {
                    found = true;
                    cout << root << "\n";
                    for (auto& edge : edges) {
                        cout << edge.first << " " << edge.second << "\n";
                    }
                    break;
                }
                
            } while (next_permutation(others.begin(), others.end()));
        }
        
        if (!found) {
            cout << "-1\n";
        }
    }
    
    return 0;
}