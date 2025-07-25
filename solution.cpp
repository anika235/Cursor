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
        
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        for (int i = 0; i < n; i++) {
            cin >> b[i];
        }
        
        vector<pair<int, int>> operations;
        
        // Strategy: Iteratively apply operations until both conditions are satisfied
        // 1. Both arrays are sorted in ascending order
        // 2. a[i] < b[i] for all i
        
        bool solved = false;
        while (!solved) {
            bool changed = false;
            
            // Step 1: Move elements to appropriate arrays
            // Elements 1 to n should ideally be in array a
            // Elements n+1 to 2n should ideally be in array b
            for (int i = 0; i < n; i++) {
                if (a[i] > n && b[i] <= n) {
                    swap(a[i], b[i]);
                    operations.push_back({3, i + 1});
                    changed = true;
                }
            }
            
            // Step 2: One pass of bubble sort on array a
            for (int i = 0; i < n - 1; i++) {
                if (a[i] > a[i + 1]) {
                    swap(a[i], a[i + 1]);
                    operations.push_back({1, i + 1});
                    changed = true;
                }
            }
            
            // Step 3: One pass of bubble sort on array b
            for (int i = 0; i < n - 1; i++) {
                if (b[i] > b[i + 1]) {
                    swap(b[i], b[i + 1]);
                    operations.push_back({2, i + 1});
                    changed = true;
                }
            }
            
            // Step 4: Ensure a[i] < b[i] condition
            for (int i = 0; i < n; i++) {
                if (a[i] >= b[i]) {
                    swap(a[i], b[i]);
                    operations.push_back({3, i + 1});
                    changed = true;
                }
            }
            
            // Check if both conditions are satisfied
            bool condition1 = true, condition2 = true;
            
            // Check if arrays are sorted
            for (int i = 0; i < n - 1; i++) {
                if (a[i] >= a[i + 1] || b[i] >= b[i + 1]) {
                    condition1 = false;
                    break;
                }
            }
            
            // Check if a[i] < b[i] for all i
            for (int i = 0; i < n; i++) {
                if (a[i] >= b[i]) {
                    condition2 = false;
                    break;
                }
            }
            
            solved = condition1 && condition2;
            
            // Safety: break if no changes were made to avoid infinite loop
            if (!changed) break;
        }
        
        cout << operations.size() << "\n";
        for (const auto& op : operations) {
            cout << op.first << " " << op.second << "\n";
        }
    }
    
    return 0;
}