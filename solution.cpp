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
        
        // Process all rows from 1 to n:
        // - For rows 1 to n-1: reverse [1,i] then [i+1,n]
        // - For row n: reverse entire row [1,n]
        // This ensures each column becomes a permutation
        
        for (int i = 1; i <= n; i++) {
            if (i < n) {
                // For rows 1 to n-1: two operations
                operations.push_back(make_tuple(i, 1, i));
                operations.push_back(make_tuple(i, i + 1, n));
            } else {
                // For row n: one operation (reverse entire row)
                operations.push_back(make_tuple(i, 1, n));
            }
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << "\n";
        }
    }
    
    return 0;
}