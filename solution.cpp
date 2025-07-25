#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n, k;
        cin >> n >> k;
        
        vector<long long> p(n);  // positions of traffic lights
        vector<int> d(n);        // delays of traffic lights
        
        for (int i = 0; i < n; i++) {
            cin >> p[i];
        }
        
        for (int i = 0; i < n; i++) {
            cin >> d[i];
        }
        
        int q;
        cin >> q;
        
        while (q--) {
            long long start;
            cin >> start;
            
            long long pos = start;
            int dir = 1; // 1 for right, -1 for left
            bool escaped = false;
            
            // Simulate movement for a sufficient number of steps
            // Traffic light patterns repeat every k seconds
            // In worst case, we need at most O(k + n) steps to determine the outcome
            for (int time = 0; time < 4 * k + 2 * n; time++) {
                // Check if we've escaped the strip
                if (pos <= 0 || pos > 1e15) {
                    escaped = true;
                    break;
                }
                
                // Check if current position has a red traffic light
                bool red = false;
                for (int i = 0; i < n; i++) {
                    if (pos == p[i] && (time % k) == d[i]) {
                        red = true;
                        break;
                    }
                }
                
                // If red light encountered, turn around
                if (red) {
                    dir = -dir;
                }
                
                // Move one step in current direction
                pos += dir;
                
                // Early termination: if we're far from all lights and moving away
                if (n > 0) {
                    if (pos < p[0] - k && dir == -1) {
                        escaped = true;
                        break;
                    }
                    if (pos > p[n-1] + k && dir == 1) {
                        escaped = true;
                        break;
                    }
                }
            }
            
            cout << (escaped ? "YES" : "NO") << "\n";
        }
    }
    
    return 0;
}