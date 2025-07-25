#include <iostream>
#include <vector>
#include <functional>
using namespace std;

long long calculate_sum(int root, const vector<pair<int, int>>& edges, int n) {
    // Build adjacency list
    vector<vector<int>> adj(n + 1);
    for (auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
    }
    
    // Calculate divineness for each node
    long long total = 0;
    
    function<void(int, vector<int>&)> dfs = [&](int node, vector<int>& path) {
        path.push_back(node);
        
        // Calculate divineness as minimum in path
        int min_val = path[0];
        for (int x : path) {
            min_val = min(min_val, x);
        }
        total += min_val;
        
        // Recurse to children
        for (int child : adj[node]) {
            dfs(child, path);
        }
        
        path.pop_back();
    };
    
    vector<int> path;
    dfs(root, path);
    
    return total;
}

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
            // Try to build a simple chain: root -> others in some order
            vector<int> others;
            for (int i = 1; i <= n; i++) {
                if (i != root) {
                    others.push_back(i);
                }
            }
            
            // Try the chain in the natural order (excluding root)
            vector<pair<int, int>> edges;
            int parent = root;
            
            for (int node : others) {
                edges.push_back({parent, node});
                parent = node;
            }
            
            long long sum = calculate_sum(root, edges, n);
            if (sum == m) {
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