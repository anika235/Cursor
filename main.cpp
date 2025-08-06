// We'll replace the entire file with new implementation
#include <bits/stdc++.h>
using namespace std;

using u128 = unsigned __int128;

// Convert unsigned __int128 to string
static string to_string_u128(u128 x) {
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        int digit = x % 10;
        s.push_back('0' + digit);
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

// Count overlapping occurrences of pattern in text (text length <= 2*(m-1))
static int count_inside(const string &text, const string &pat) {
    int n = (int)text.size();
    int m = (int)pat.size();
    if (m == 0 || m > n) return 0;
    int cnt = 0;
    for (int i = 0; i + m <= n; ++i) {
        if (memcmp(text.data() + i, pat.data(), m) == 0) ++cnt;
    }
    return cnt;
}

// Count occurrences crossing boundary between suffix (len sLen) and prefix
static int count_cross(const string &joined, int sLen, const string &pat) {
    int m = (int)pat.size();
    if (m <= 1) return 0; // cannot cross
    int tot = (int)joined.size();
    int start = max(0, sLen - m + 1);
    int cnt = 0;
    for (int i = start; i + m <= tot && i < sLen; ++i) {
        if (memcmp(joined.data() + i, pat.data(), m) == 0 && i + m > sLen) ++cnt;
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n; string P;
    int caseNo = 1;
    const int MAX_N = 100;

    while (cin >> n >> P) {
        int m = (int)P.size();
        int keep = max(0, m - 1);

        static u128 cnt[MAX_N + 1];
        static u128 len[MAX_N + 1];
        static string pref[MAX_N + 1];
        static string suf[MAX_N + 1];

        // Base cases
        string F0 = "0", F1 = "1";
        len[0] = 1; len[1] = 1;
        cnt[0] = count_inside(F0, P);
        cnt[1] = count_inside(F1, P);
        pref[0] = F0.substr(0, min(keep, 1));
        suf[0] = pref[0];
        pref[1] = F1.substr(0, min(keep, 1));
        suf[1] = pref[1];

        for (int i = 2; i <= n; ++i) len[i] = len[i-1] + len[i-2];

        for (int i = 2; i <= n; ++i) {
            // build prefix
            if (len[i-1] >= (u128)keep) {
                pref[i] = pref[i-1];
            } else {
                pref[i] = pref[i-1] + pref[i-2];
                if ((int)pref[i].size() > keep) pref[i].erase(keep);
            }
            // build suffix
            if (len[i-2] >= (u128)keep) {
                suf[i] = suf[i-2];
            } else {
                string tmp = suf[i-1] + suf[i-2];
                if ((int)tmp.size() > keep) tmp.erase(0, tmp.size() - keep);
                suf[i] = std::move(tmp);
            }
            int cross = 0;
            if (m > 1 && keep) {
                string joined = suf[i-1] + pref[i-2];
                cross = count_cross(joined, (int)suf[i-1].size(), P);
            }
            cnt[i] = cnt[i-1] + cnt[i-2] + (u128)cross;
        }

        cout << "Case " << caseNo++ << ": " << to_string_u128(cnt[n]) << '\n';
    }
    return 0;
}