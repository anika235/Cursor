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
        
        // Precompute row and column maxima
        vector<int> row_max(n, 0);
        vector<int> col_max(m, 0);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                row_max[i] = max(row_max[i], a[i][j]);
                col_max[j] = max(col_max[j], a[i][j]);
            }
        }
        
        // Precompute quadrant maxima
        // UL[i][j] = max in rectangle from (0,0) to (i,j)
        vector<vector<int>> UL(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                UL[i][j] = a[i][j];
                if (i > 0) UL[i][j] = max(UL[i][j], UL[i-1][j]);
                if (j > 0) UL[i][j] = max(UL[i][j], UL[i][j-1]);
                if (i > 0 && j > 0) UL[i][j] = max(UL[i][j], UL[i-1][j-1]);
            }
        }
        
        // UR[i][j] = max in rectangle from (0,j) to (i,m-1)
        vector<vector<int>> UR(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = m-1; j >= 0; j--) {
                UR[i][j] = a[i][j];
                if (i > 0) UR[i][j] = max(UR[i][j], UR[i-1][j]);
                if (j < m-1) UR[i][j] = max(UR[i][j], UR[i][j+1]);
                if (i > 0 && j < m-1) UR[i][j] = max(UR[i][j], UR[i-1][j+1]);
            }
        }
        
        // LL[i][j] = max in rectangle from (i,0) to (n-1,j)
        vector<vector<int>> LL(n, vector<int>(m, 0));
        for (int i = n-1; i >= 0; i--) {
            for (int j = 0; j < m; j++) {
                LL[i][j] = a[i][j];
                if (i < n-1) LL[i][j] = max(LL[i][j], LL[i+1][j]);
                if (j > 0) LL[i][j] = max(LL[i][j], LL[i][j-1]);
                if (i < n-1 && j > 0) LL[i][j] = max(LL[i][j], LL[i+1][j-1]);
            }
        }
        
        // LR[i][j] = max in rectangle from (i,j) to (n-1,m-1)
        vector<vector<int>> LR(n, vector<int>(m, 0));
        for (int i = n-1; i >= 0; i--) {
            for (int j = m-1; j >= 0; j--) {
                LR[i][j] = a[i][j];
                if (i < n-1) LR[i][j] = max(LR[i][j], LR[i+1][j]);
                if (j < m-1) LR[i][j] = max(LR[i][j], LR[i][j+1]);
                if (i < n-1 && j < m-1) LR[i][j] = max(LR[i][j], LR[i+1][j+1]);
            }
        }
        
        int min_max = INT_MAX;
        
        // Try all possible operations (r, c)
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < m; c++) {
                // Calculate maximum value outside row r and column c
                int rest_max = 0;
                
                // Upper-left quadrant (excludes row r and column c)
                if (r > 0 && c > 0) {
                    rest_max = max(rest_max, UL[r-1][c-1]);
                }
                
                // Upper-right quadrant (excludes row r and column c)
                if (r > 0 && c < m-1) {
                    rest_max = max(rest_max, UR[r-1][c+1]);
                }
                
                // Lower-left quadrant (excludes row r and column c)
                if (r < n-1 && c > 0) {
                    rest_max = max(rest_max, LL[r+1][c-1]);
                }
                
                // Lower-right quadrant (excludes row r and column c)
                if (r < n-1 && c < m-1) {
                    rest_max = max(rest_max, LR[r+1][c+1]);
                }
                
                // The maximum after operation (r, c) is the maximum of:
                // 1. Row r maximum - 1 (all cells in row r decrease by 1)
                // 2. Column c maximum - 1 (all cells in column c decrease by 1)
                // 3. Maximum in areas not affected by the operation
                int candidate = max({row_max[r] - 1, col_max[c] - 1, rest_max});
                
                min_max = min(min_max, candidate);
            }
        }
        
        cout << min_max << "\n";
    }
    
    return 0;
}