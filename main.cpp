#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;                          // number of test cases
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        const int HALF = n / 2;

        vector<int> freq(n + 2, 0); // frequencies for values 1..n (n+1 is dummy)
        for (int i = 0, x; i < n; ++i) {
            cin >> x;
            ++freq[x];
        }

        /*
           dp[carry][used] == 1  if it is possible to have currently
           - "carry"  copies waiting to be increased to (v+1)
           - exactly "used" numbers already moved into the second bag
        */
        vector<vector<char>> dp(n + 1, vector<char>(HALF + 1, 0));
        dp[0][0] = 1;

        for (int v = 1; v <= n + 1; ++v) {
            vector<vector<char>> ndp(n + 1, vector<char>(HALF + 1, 0));
            const int fv = (v <= n ? freq[v] : 0);               // freq for current value

            for (int carry = 0; carry <= n; ++carry) {
                for (int used = 0; used <= HALF; ++used) if (dp[carry][used]) {
                    int total = carry + fv;                      // total v's in first bag now

                    // upper bound for r: cannot move more than fv, total/2, or capacity left in bag2
                    int maxR = min({fv, total / 2, HALF - used});

                    for (int r = 0; r <= maxR; ++r) {
                        int newCarry = total - 2 * r;            // remaining copies that will be incremented
                        if (newCarry > 0 && r == 0) continue;    // need at least one witness in bag2

                        int newUsed = used + r;
                        ndp[newCarry][newUsed] = 1;
                    }
                }
            }
            dp.swap(ndp);
        }

        cout << (dp[0][HALF] ? "YES\n" : "NO\n");
    }
    return 0;
}