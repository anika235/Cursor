#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<int64> a(n), b(m);
        for (auto &x : a) cin >> x;
        for (auto &x : b) cin >> x;

        // ------------------------------------------------------------
        // 1. Quick rejection: total sums must match.
        // ------------------------------------------------------------
        int64 sum_a = accumulate(a.begin(), a.end(), int64(0));
        int64 sum_b = accumulate(b.begin(), b.end(), int64(0));
        if (sum_a != sum_b) {
            cout << "No\n";
            continue;
        }

        // ------------------------------------------------------------
        // 2. Frequency map for required numbers (sequence a)
        // ------------------------------------------------------------
        unordered_map<int64, int> need;
        need.reserve(n * 2);
        for (int64 x : a) ++need[x];

        // ------------------------------------------------------------
        // 3. Max-heap containing the current board (starts with b)
        // ------------------------------------------------------------
        priority_queue<int64> pq;
        for (int64 x : b) pq.push(x);

        // We can perform at most (n - m) splits, otherwise the board
        // would contain more than n numbers and could never equal a.
        int max_splits = n - m;
        int splits = 0;
        bool ok = true;

        while (!need.empty() && !pq.empty() && splits <= max_splits) {
            int64 x = pq.top();
            pq.pop();

            auto it = need.find(x);
            if (it != need.end()) {
                // Match found
                if (--(it->second) == 0) need.erase(it);
                continue;
            }

            if (x == 1) { // Cannot split further but still not needed
                ok = false;
                break;
            }

            if (splits == max_splits) {
                ok = false;
                break;
            }

            // Split x into two parts differing by at most 1
            int64 left = x / 2;          // floor(x/2)
            int64 right = x - left;      // ceil(x/2)
            pq.push(left);
            pq.push(right);
            ++splits;
        }

        if (!need.empty()) ok = false;      // unmatched numbers remain
        if (splits > max_splits) ok = false; // exceeded split budget

        cout << (ok ? "Yes\n" : "No\n");
    }
    return 0;
}