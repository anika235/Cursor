#include <bits/stdc++.h>
using namespace std;

const double PI  = acos(-1.0);
const double EPS = 1e-12;

// ---------- Quick area formulas for special cases ----------

double areaTriangle(double a, double b, double c) {
    double p = (a + b + c) * 0.5;
    double t = p * (p - a) * (p - b) * (p - c);
    return t <= 0 ? 0 : sqrt(t);
}

double areaQuad(double a, double b, double c, double d) {
    double p = (a + b + c + d) * 0.5;
    double t = (p - a) * (p - b) * (p - c) * (p - d);
    return t <= 0 ? 0 : sqrt(t);
}

// ---------- Circum-radius of maximal cyclic polygon (Newton) ----------

double radiusCyclic(const vector<double>& side) {
    int k = (int)side.size();
    double mx = *max_element(side.begin(), side.end());

    // reasonable initial guess (regular polygon approx.)
    double R = 0.55 * mx / sin(PI / k);

    for (int it = 0; it < 12; ++it) {
        double f = -2.0 * PI, df = 0.0;
        for (double a : side) {
            double s = a / (2.0 * R);          // 0 < s < 1
            double theta = 2.0 * asin(s);
            f += theta;
            df -= a / (R * R * sqrt(1.0 - s * s));
        }
        double step = f / df;
        R -= step;
        if (fabs(step) < 1e-12 * R) break;      // converged
        if (R <= mx * 0.5) R = mx * 0.5001;     // safeguard
    }
    return R;
}

// ---------- Maximal area of cyclic polygon with given side sequence ----------

double cyclicArea(const vector<double>& side) {
    int k = (int)side.size();
    if (k == 3) return areaTriangle(side[0], side[1], side[2]);
    if (k == 4) return areaQuad(side[0], side[1], side[2], side[3]);

    // regular polygon fast path (all sides equal, k >= 5)
    bool regular = true;
    for (int i = 1; i < k && regular; ++i)
        if (fabs(side[i] - side[0]) > EPS) regular = false;

    if (regular && k >= 5) {
        double a = side[0];
        return (k * a * a) / (4.0 * tan(PI / k));
    }

    double R = radiusCyclic(side);
    double sumSin = 0.0;
    for (double a : side) {
        double ang = 2.0 * asin(a / (2.0 * R));
        sumSin += sin(ang);
    }
    return 0.5 * R * R * sumSin;
}

// ---------- Main solver ----------

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, tc = 1;
    while (cin >> n && n) {
        vector<double> s(n);
        for (double &x : s) cin >> x;

        // Pre-compute area for every contiguous block
        vector<vector<double>> area(n, vector<double>(n, -1.0));
        for (int l = 0; l < n; ++l) {
            double sum = 0.0, mx = 0.0;
            vector<double> buf;
            buf.reserve(n - l);
            for (int r = l; r < n; ++r) {
                sum += s[r];
                mx = max(mx, s[r]);
                buf.push_back(s[r]);
                if (r - l + 1 >= 3 && mx < sum - mx + EPS) { // corrected condition
                    area[l][r] = cyclicArea(buf);
                }
            }
        }

        // 1-D DP over the sequence
        vector<double> dp(n + 1, 0.0);
        for (int i = 1; i <= n; ++i) {
            dp[i] = dp[i - 1];               // skip edge i-1
            for (int j = 0; j <= i - 3; ++j) {
                double a = area[j][i - 1];
                if (a < 0) continue;
                dp[i] = max(dp[i], dp[j] + a);
            }
        }

        cout.setf(ios::fixed);
        cout << setprecision(6) << "Case " << tc++ << ": " << dp[n] << '\n';
    }
    return 0;
}