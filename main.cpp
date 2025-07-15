#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;   // no input – safety guard

    while (t--) {
        int n;
        if (!(cin >> n)) return 0; // invalid format

        // read the known array x_1 .. x_n (unused by our 2-query strategy)
        for (int i = 0, tmp; i < n; ++i) cin >> tmp;

        // We will query the pair (1 , 2) and then (2 , 1).
        int i = 1, j = 2;
        long long d1, d2;

        // First query: distance from i to j
        cout << "? " << i << ' ' << j << endl;   // endl flushes the buffer
        if (!(cin >> d1) || d1 == -1) return 0;    // terminate on protocol error

        // Second query: distance from j to i
        cout << "? " << j << ' ' << i << endl;
        if (!(cin >> d2) || d2 == -1) return 0;

        bool isGraph = (d1 == 0 || d2 == 0 || d1 != d2);
        cout << "! " << (isGraph ? 'A' : 'B') << endl;  // announce the answer and flush
    }
    return 0;
}