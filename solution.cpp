#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        vector<tuple<int, int, int>> operations;
        
        // For each row i (starting from row 2), we use exactly 2 operations:
        // - For row 2: reverse [2, n] then [1, n]
        // - For other rows: reverse [1, i] then [2, n]
        // This creates a Latin square where each column is a permutation
        
        for (int i = 1; i < n; i++) {
            if (i == 1) {
                // Row 2: reverse [2, n] then [1, n]
                operations.push_back({i + 1, 2, n});
                operations.push_back({i + 1, 1, n});
            } else {
                // Other rows: reverse [1, i+1] then [2, n]
                operations.push_back({i + 1, 1, i + 1});
                operations.push_back({i + 1, 2, n});
            }
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << "\n";
        }
    }
    
    return 0;
}