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
            long long t_mod = 0;
            set<tuple<int, int, int>> visited; // (light_index, dir, t_mod)
            
            while (true) {
                // Check if we've escaped the strip
                if (pos < 1 || pos > 1000000000000000LL) {
                    cout << "YES\n";
                    break;
                }
                
                // Find if we're at a traffic light
                auto it = lower_bound(p.begin(), p.end(), pos);
                bool at_light = (it != p.end() && *it == pos);
                
                if (at_light) {
                    // We're at a traffic light
                    int idx = it - p.begin();
                    auto state = make_tuple(idx, dir, (int)t_mod);
                    
                    // Check for cycle
                    if (visited.count(state)) {
                        cout << "NO\n";
                        break;
                    }
                    visited.insert(state);
                    
                    // Check if light is red and turn around if needed
                    if (t_mod % k == d[idx]) {
                        dir = -dir;
                    }
                    
                    // Move one step and update time
                    pos += dir;
                    t_mod = (t_mod + 1) % k;
                } else {
                    // We're not at a traffic light, jump to the next one
                    if (dir == 1) {
                        // Moving right
                        if (it == p.end()) {
                            // No lights to the right, we'll escape
                            cout << "YES\n";
                            break;
                        }
                        long long next_p = *it;
                        long long dist = next_p - pos;
                        t_mod = (t_mod + dist) % k;
                        pos = next_p;
                    } else {
                        // Moving left
                        if (it == p.begin()) {
                            // No lights to the left, we'll escape
                            cout << "YES\n";
                            break;
                        }
                        --it;
                        long long next_p = *it;
                        long long dist = pos - next_p;
                        t_mod = (t_mod + dist) % k;
                        pos = next_p;
                    }
                }
            }
        }
    }
    
    return 0;
}