#include <bits/stdc++.h>
using namespace std;

static constexpr int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; if(!(cin >> T)) return 0;
    while (T--) {
        int n; cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        // overall gcd
        int G = 0;
        for (int x : a) G = std::gcd(G, x);

        int cntG = 0;
        for (int x : a) if (x == G) ++cntG;

        if (cntG) {
            cout << n - cntG << '\n';
            continue;
        }

        // Compress the numbers by G
        const int MAXV = 5000;               //  given in constraints
        vector<int> dp(MAXV + 1, INF);       // dp[g] = minimal subset size with gcd == g
        vector<int> present;                 // list of gcds that currently have finite dp
        vector<char> in_list(MAXV + 1, 0);   // helper to avoid many duplicates

        for (int x_orig : a) {
            int x = x_orig / G;              // b_i

            // snapshot of current states to iterate over (so we do not reuse x twice)
            vector<int> states = present;    // copy is cheap (size <= 5000)
            for (int g : states) {
                int h = std::gcd(g, x);
                if (dp[g] + 1 < dp[h]) {
                    dp[h] = dp[g] + 1;
                    if (!in_list[h]) {
                        present.push_back(h);
                        in_list[h] = 1;
                    }
                }
            }
            // single element subset {x}
            if (1 < dp[x]) {
                dp[x] = 1;
                if (!in_list[x]) {
                    present.push_back(x);
                    in_list[x] = 1;
                }
            }
        }

        int k = dp[1];                       // minimal subset size whose gcd is 1
        // According to the proven formula
        cout << n + k - 2 << '\n';
    }
    return 0;
}