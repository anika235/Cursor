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

        vector<long long> x(n);
        for (int i = 0; i < n; ++i) cin >> x[i];

        // pick indices with minimum and maximum x-values
        int idxMin = 0, idxMax = 0;
        for (int i = 1; i < n; ++i) {
            if (x[i] < x[idxMin]) idxMin = i;
            if (x[i] > x[idxMax]) idxMax = i;
        }
        // Fallback in degenerate case (all x equal)
        if (idxMin == idxMax) {
            idxMin = 0;
            idxMax = 1 % n; // n >= 3 per statement, but guard anyway
        }
        int i = idxMin + 1; // convert to 1-based
        int j = idxMax + 1;

        long long d1, d2;

        // First query: distance i -> j
        cout << "? " << i << ' ' << j << endl;
        if (!(cin >> d1) || d1 == -1) return 0;

        // Second query: distance j -> i
        cout << "? " << j << ' ' << i << endl;
        if (!(cin >> d2) || d2 == -1) return 0;

        bool isGraph = false;
        if (d1 == 0 || d2 == 0 || d1 != d2) {
            isGraph = true;
        } else {
            // d1 == d2 > 0 : compare with |x_i - x_j|
            long long dx = llabs(x[idxMin] - x[idxMax]);
            if (dx > 0 && d1 < dx) isGraph = true;
        }
        cout << "! " << (isGraph ? 'A' : 'B') << endl;
    }
    return 0;
}