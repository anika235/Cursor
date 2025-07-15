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

                    if (total == 0) {
                        // nothing to do
                        ndp[0][used] = 1;
                        continue;
                    }

                    // choose r (called x in the proof) in [1 .. maxR]
                    int maxR = min(total / 2, HALF - used);
                    for (int r = 1; r <= maxR; ++r) {
                        int newCarry = total - 2 * r;            // copies that will be incremented to (v+1)
                        int newUsed  = used + r;
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