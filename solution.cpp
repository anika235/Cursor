#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n, m;
        cin >> n >> m;
        
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> a[i][j];
            }
        }
        
        int min_max = INT_MAX;
        
        // Try all possible combinations of row r and column c
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < m; c++) {
                int current_max = 0;
                
                // Calculate the maximum value after operation (r+1, c+1)
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < m; j++) {
                        int new_val = a[i][j];
                        
                        // If in row r OR column c, decrease by 1
                        if (i == r || j == c) {
                            new_val--;
                        }
                        
                        current_max = max(current_max, new_val);
                    }
                }
                
                min_max = min(min_max, current_max);
            }
        }
        
        cout << min_max << "\n";
    }
    
    return 0;
}