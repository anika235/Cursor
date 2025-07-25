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
        
        // Try each possible root r from 1 to n
        for (int r = 1; r <= n && !found; r++) {
            if (r == 1) {
                // Special case: root = 1, only achievable total is n (star tree)
                if (m == n) {
                    found = true;
                    cout << "1\n";
                    for (int i = 2; i <= n; i++) {
                        cout << "1 " << i << "\n";
                    }
                }
                continue;
            }
            
            // For r >= 2
            // Minimum total: r + (n-1) achieved by attaching all to node 1
            long long min_total = r + (n - 1);
            
            if (m < min_total) continue;
            
            // Calculate extra needed beyond minimum
            long long extra = m - min_total;
            
            // Strategy:
            // - Node 1 is attached to root (contributes 1)
            // - k nodes from {r+1, r+2, ..., n} attached to root (each gives extra r-1)
            // - Use nodes {2, 3, ..., r-1} to provide remaining extra
            
            long long k = min((long long)(n - r), extra / (r - 1));
            long long rem = extra - k * (r - 1);
            
            // Check if remainder can be achieved with nodes 2, 3, ..., r-1
            // Each node i in {2, 3, ..., r-1} can provide at most (r-1) extra
            // when attached to root instead of node 1
            
            // Maximum remainder we can achieve
            long long max_rem = (long long)(r - 2) * (r - 1);
            
            if (rem > max_rem) continue;
            
            // Check if we can represent rem as sum of distinct values from {1, 2, ..., r-2}
            // This corresponds to moving nodes {2, 3, ..., r-1} strategically
            
            // Use greedy approach: represent rem using largest possible values first
            vector<bool> use_node(r, false);
            long long temp_rem = rem;
            
            for (int i = r - 2; i >= 1 && temp_rem > 0; i--) {
                if (temp_rem >= i) {
                    use_node[i + 1] = true; // node (i+1) gets extra contribution i
                    temp_rem -= i;
                }
            }
            
            if (temp_rem > 0) continue; // Cannot represent exactly
            
            // We found a valid configuration
            found = true;
            cout << r << "\n";
            
            vector<pair<int, int>> edges;
            
            // Attach node 1 to root
            edges.push_back({r, 1});
            
            // Attach k largest nodes to root
            for (int i = 0; i < k; i++) {
                int node = n - i;
                if (node != r) {
                    edges.push_back({r, node});
                }
            }
            
            // Attach selected small nodes to root
            for (int i = 2; i < r; i++) {
                if (use_node[i]) {
                    edges.push_back({r, i});
                } else {
                    edges.push_back({1, i});
                }
            }
            
            // Attach remaining large nodes to node 1
            for (int i = r + 1; i <= n - k; i++) {
                edges.push_back({1, i});
            }
            
            // Output edges
            for (auto& edge : edges) {
                cout << edge.first << " " << edge.second << "\n";
            }
        }
        
        if (!found) {
            cout << "-1\n";
        }
    }
    
    return 0;
}