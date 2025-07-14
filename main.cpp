#include <bits/stdc++.h>
using namespace std;

static const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        // hCnt : #ways where current person (i) is honest
        // lCnt : #ways where current person is liar
        long long hCnt = 1;     // empty prefix behaves like an "honest" virtual person with k=0
        long long lCnt = 0;
        int hK = 0;             // number of liars so far when current person honest (always 0 initially)
        int lK = -1;            // undefined when lCnt == 0

        for (int Ai : a) {
            long long newHCnt = 0;
            // Become honest from a previous honest state
            if (hCnt && Ai == hK) {
                newHCnt += hCnt;
            }
            // Become honest from a previous liar state
            if (lCnt && Ai == lK) {
                newHCnt += lCnt;
            }
            newHCnt %= MOD;

            // Become liar (only possible if previous person was honest)
            long long newLCnt = hCnt % MOD;
            int newLK = hK + 1;     // number of liars increases by 1
            if (hCnt == 0) {
                // no previous honest state, so this transition is impossible
                newLCnt = 0;
                newLK = -1;
            }

            // update states
            hCnt = newHCnt;
            hK = (newHCnt ? Ai : -1);
            lCnt = newLCnt;
            lK = (newLCnt ? newLK : -1);
        }

        long long ans = (hCnt + lCnt) % MOD;
        cout << ans << '\n';
    }
    return 0;
}