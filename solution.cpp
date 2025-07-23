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
        
        // We'll create a pattern where:
        // Row i should become a cyclic shift of [1,2,3,...,n] by i positions
        // Row 0: [1, 2, 3, ..., n]
        // Row 1: [2, 3, 4, ..., n, 1] 
        // Row 2: [3, 4, 5, ..., n, 1, 2]
        // etc.
        
        for (int i = 1; i < n; i++) {
            // For row i (1-indexed i+1), we want to shift left by i positions
            // Current state: [1, 2, 3, ..., n]
            // Target state: [i+1, i+2, ..., n, 1, 2, ..., i]
            
            // Method: Use three reversals to achieve left rotation by i positions
            // 1. Reverse first i elements: [1,2,...,i] -> [i,i-1,...,1]
            // 2. Reverse remaining elements: [i+1,i+2,...,n] -> [n,n-1,...,i+1]  
            // 3. Reverse entire array: [i,i-1,...,1,n,n-1,...,i+1] -> [i+1,i+2,...,n,1,2,...,i]
            
            if (i > 1) {
                operations.push_back({i + 1, 1, i});
            }
            if (n - i > 1) {
                operations.push_back({i + 1, i + 1, n});
            }
            operations.push_back({i + 1, 1, n});
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << "\n";
        }
    }
    
    return 0;
}