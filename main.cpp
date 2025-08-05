#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Round to nearest integer, ties away from zero, using long double precision
static int snap(long double v) {
    return static_cast<int>(llroundl(v));
}

/* Solve one coordinate axis for scaling k and constant c.
   order == 0 : Rotation -> Scaling -> Translation   (RST actually described as R S T): F = k*P + c  (c = t)
   order == 1 : Rotation -> Translation -> Scaling   (RTS):                         F = k*(P + t) = k*P + c (c = k*t)
   Returns false if impossible. When returns true, unique indicates whether the axis mapping is uniquely determined (true) or there are infinitely many valid integer (k,c) pairs (false).
*/
static bool solve_axis(const int P[3], const int F[3], int order, ll &k, ll &c, bool &unique) {
    // All P equal?
    if (P[0] == P[1] && P[0] == P[2]) {
        if (!(F[0] == F[1] && F[0] == F[2])) return false; // outputs unequal -> impossible
        // Any non-zero k and suitable c works -> infinite solutions
        unique = false;
        return true;
    }

    // pick indices with different P
    int i = 0, j = 1;
    if (P[i] == P[j]) j = 2;
    if (P[i] == P[j]) { i = 1; j = 2; }

    ll denom = (ll)P[i] - P[j];
    ll numer = (ll)F[i] - F[j];
    if (denom == 0) return false; // should not happen
    if (numer % denom) return false; // must divide exactly for integer k
    k = numer / denom;
    if (k == 0) return false; // scaling cannot be zero

    c = (ll)F[i] - k * P[i];

    // For RTS order, c must be divisible by k so that t = c / k is integer
    if (order == 1 && c % k) return false;

    // verify third point
    for (int t = 0; t < 3; ++t) {
        if ((ll)F[t] != k * P[t] + c) return false;
    }

    unique = true;
    return true;
}

struct Map {
    int rot; // index of rotation direction within dirs vector
    ll kx, ky, cx, cy; // affine parameters (F = k*P + c per axis after snapping)
};

// check whether two maps have the same overall effect on every integer point
static bool same_effect(const Map &a, const Map &b,
                        const vector<pair<int,int>> &dirs) {
    if (a.cx != b.cx || a.cy != b.cy) return false;

    if (a.kx == b.kx && a.ky == b.ky && a.rot == b.rot) return true;

    // check 180° ambiguity: directions opposite and scales negated
    auto d1 = dirs[a.rot];
    long long keyOpp = ((long long)(-d1.first + 100) << 32) | (long long)(-d1.second + 100000); // key generation but we can just compute index by map
    // But easier: use dirs[b.rot] compare
    auto d2 = dirs[b.rot];
    if (d2.first == -d1.first && d2.second == -d1.second && a.kx == -b.kx && a.ky == -b.ky) return true;

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Precompute 80 possible rotation directions (x,y) on perimeter of 20x20 square with integer coordinates)
    vector<pair<int,int>> dirs;
    dirs.reserve(80);
    for (int x = -10; x <= 10; ++x) {
        for (int y = -10; y <= 10; ++y) {
            if (x == 0 && y == 0) continue;
            if (abs(x) == 10 || abs(y) == 10) dirs.emplace_back(x, y);
        }
    }

    int caseNo = 1;
    while (true) {
        int a[12];
        bool allZero = true;
        for (int i = 0; i < 6; ++i) {
            if (!(cin >> a[i])) return 0; // EOF
            if (a[i] != 0) allZero = false;
        }
        if (allZero) break; // end sentinel
        for (int i = 6; i < 12; ++i) cin >> a[i];

        int X0[3], Y0[3], XF[3], YF[3];
        for (int i = 0; i < 3; ++i) {
            X0[i] = a[2 * i];
            Y0[i] = a[2 * i + 1];
        }
        for (int i = 0; i < 3; ++i) {
            XF[i] = a[6 + 2 * i];
            YF[i] = a[6 + 2 * i + 1];
        }

        bool foundOne = false;
        bool multiple = false;
        Map firstMap{};

        vector<int> perm = {0, 1, 2};

        for (int rotId = 0; rotId < (int)dirs.size() && !multiple; ++rotId) {
            auto [rx, ry] = dirs[rotId];
            long double len = hypotl((long double)rx, (long double)ry);
            long double cs = rx / len, sn = ry / len;

            int PRx[3], PRy[3];
            for (int i = 0; i < 3; ++i) {
                long double xr = (long double)X0[i] * cs - (long double)Y0[i] * sn;
                long double yr = (long double)X0[i] * sn + (long double)Y0[i] * cs;
                PRx[i] = snap(xr);
                PRy[i] = snap(yr);
            }

            sort(perm.begin(), perm.end());
            do {
                int FX[3], FY[3];
                for (int i = 0; i < 3; ++i) {
                    FX[i] = XF[perm[i]];
                    FY[i] = YF[perm[i]];
                }

                for (int order = 0; order < 2 && !multiple; ++order) {
                    ll kx = 0, cx = 0; bool ux = false;
                    if (!solve_axis(PRx, FX, order, kx, cx, ux)) continue;

                    ll ky = 0, cy = 0; bool uy = false;
                    if (!solve_axis(PRy, FY, order, ky, cy, uy)) continue;

                    // If either axis allows infinitely many choices, we already know there are multiple
                    if (!ux || !uy) {
                        if (!foundOne) foundOne = true; // at least one valid transformation exists
                        multiple = true;
                        break;
                    }

                    Map m{rotId, kx, ky, cx, cy};
                    if (!foundOne) {
                        firstMap = m;
                        foundOne = true;
                    } else if (!same_effect(m, firstMap, dirs)) {
                        // If two valid transformations have different effect, inconsistent
                        multiple = true;
                        break;
                    }
                }

            } while (next_permutation(perm.begin(), perm.end()) && !multiple);
        }

        cout << "Case " << caseNo++ << ": ";
        if (!foundOne) {
            cout << "no solution\n";
        } else if (multiple) {
            cout << "inconsistent solutions\n";
        } else {
            cout << "equivalent solutions\n";
        }
    }
    return 0;
}