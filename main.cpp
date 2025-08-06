// main.cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF64 = (1LL << 62);

// ------------------------------------------------------------
// Geometry helpers used for the "never" (infinite survival) test
// ------------------------------------------------------------
struct Pt {
    ll x, y;
};

static inline ll cross(const Pt &a, const Pt &b, const Pt &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static vector<Pt> convex_hull(vector<Pt> p) {
    sort(p.begin(), p.end(), [](const Pt &a, const Pt &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });
    p.erase(unique(p.begin(), p.end(), [](const Pt &a, const Pt &b) {
        return a.x == b.x && a.y == b.y;
    }), p.end());

    if (p.size() <= 1) return p;

    vector<Pt> lower, upper;
    for (auto &pt : p) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), pt) <= 0)
            lower.pop_back();
        lower.push_back(pt);
    }
    for (int i = (int)p.size() - 1; i >= 0; --i) {
        auto &pt = p[i];
        while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper.back(), pt) <= 0)
            upper.pop_back();
        upper.push_back(pt);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

static bool origin_inside(const vector<Pt> &poly) {
    int m = (int)poly.size();
    if (m == 0) return false;
    if (m == 1) return poly[0].x == 0 && poly[0].y == 0;
    if (m == 2) {
        const Pt &a = poly[0], &b = poly[1];
        if (cross(a, b, {0, 0}) != 0) return false;
        return min(a.x, b.x) <= 0 && 0 <= max(a.x, b.x) && min(a.y, b.y) <= 0 && 0 <= max(a.y, b.y);
    }
    int sign = 0;
    for (int i = 0; i < m; ++i) {
        ll c = cross(poly[i], poly[(i + 1) % m], {0, 0});
        if (c == 0) continue;
        if (sign == 0)
            sign = (c > 0 ? 1 : -1);
        else if ((c > 0 && sign < 0) || (c < 0 && sign > 0))
            return false;
    }
    return true;
}

// ------------------------------------------------------------
// Interval Segment Tree used in surviveT
// ------------------------------------------------------------
struct SegTree {
    struct Node {
        int cover = 0;          // how many intervals fully cover this node's segment
        bool full = false;      // this segment completely covered?
        ll firstGap = 0;        // leftmost coordinate not covered (INF64 if none)
    };

    int n;                      // number of leaf segments (xs.size() - 1)
    vector<ll> xs;              // coordinate compression array
    vector<Node> st;            // segment tree

    explicit SegTree(const vector<ll> &coord) : xs(coord) {
        n = (int)coord.size() - 1;
        st.resize(n * 4);
        build(1, 0, n - 1);
    }

    void build(int v, int l, int r) {
        st[v] = {};
        st[v].firstGap = xs[l];
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(v << 1, l, mid);
        build(v << 1 | 1, mid + 1, r);
    }

    void pull(int v, int l, int r) {
        if (st[v].cover > 0) {
            st[v].full = true;
            st[v].firstGap = INF64;
            return;
        }
        if (l == r) {
            st[v].full = false;
            st[v].firstGap = xs[l];
            return;
        }
        st[v].full = st[v << 1].full && st[v << 1 | 1].full;
        if (!st[v << 1].full)
            st[v].firstGap = st[v << 1].firstGap;
        else
            st[v].firstGap = st[v << 1 | 1].firstGap;
    }

    void update(int v, int l, int r, int ql, int qr, int delta) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            st[v].cover += delta;
            pull(v, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        update(v << 1, l, mid, ql, qr, delta);
        update(v << 1 | 1, mid + 1, r, ql, qr, delta);
        pull(v, l, r);
    }

    void range_add(int l, int r, int delta) {
        if (l > r) return;
        update(1, 0, n - 1, l, r, delta);
    }

    bool fully_covered() const { return st[1].full; }
    ll first_gap() const { return st[1].firstGap; }
};

// ------------------------------------------------------------
// Event type for plane sweep
// ------------------------------------------------------------
struct Event {
    ll u;        // sweep coordinate
    int delta;   // +1 add, -1 remove
    ll v1, v2;   // inclusive interval on v axis
    bool operator<(const Event &other) const {
        if (u != other.u) return u < other.u;
        return delta > other.delta; // add (+1) processed before remove (-1)
    }
};

// ------------------------------------------------------------
// survival test for a fixed T (with reusable vectors)
// ------------------------------------------------------------
static bool surviveT(const vector<Pt> &P, ll T, vector<Event> &ev, vector<ll> &vcoords) {
    if (T < 0) return true;

    const ll range = 2 * T;
    const ll minU = -range;
    const ll maxU = range;

    ev.clear();
    vcoords.clear();
    vcoords.push_back(-range);
    vcoords.push_back(range + 1); // sentinel for building at least one leaf

    ev.reserve(ev.capacity()); // capacity already set outside

    for (const auto &p : P) {
        ll u0 = p.x + p.y;
        ll v0 = p.x - p.y;
        ll u1 = max(minU, u0 - range);
        ll u2 = min(maxU, u0 + range);
        ll v1 = max(minU, v0 - range);
        ll v2 = min(maxU, v0 + range);
        if (u1 > u2 || v1 > v2) continue; // no overlap with player square

        ev.push_back({u1, +1, v1, v2});
        ev.push_back({u2 + 1, -1, v1, v2}); // +1 because intervals inclusive on u

        vcoords.push_back(v1);
        vcoords.push_back(v2 + 1);
    }

    sort(ev.begin(), ev.end());
    sort(vcoords.begin(), vcoords.end());
    vcoords.erase(unique(vcoords.begin(), vcoords.end()), vcoords.end());
    if ((int)vcoords.size() == 1) vcoords.push_back(vcoords.back() + 1);

    // Build segment tree on the compressed v axis
    SegTree seg(vcoords);

    auto add_interval = [&](ll L, ll R, int d) {
        int l = (int)(lower_bound(vcoords.begin(), vcoords.end(), L) - vcoords.begin());
        int r = (int)(lower_bound(vcoords.begin(), vcoords.end(), R + 1) - vcoords.begin()) - 1;
        seg.range_add(l, r, d);
    };

    const auto parity_ok = [](ll u, ll v) { return ((u ^ v) & 1LL) == 0; };

    auto gap_exists = [&](ll uL, ll uR) {
        if (seg.fully_covered()) return false;
        ll vgap = seg.first_gap();
        if (vgap < -range || vgap > range) return false;
        // need an integer u in [uL, uR] having same parity as vgap
        ll u = uL;
        if (!parity_ok(u, vgap)) ++u;
        return u <= uR;
    };

    size_t idx = 0;
    ll currentU = minU;

    while (idx < ev.size()) {
        ll nextU = ev[idx].u;
        if (currentU <= maxU && currentU < nextU) {
            if (gap_exists(currentU, min(maxU, nextU - 1))) return true; // safe point found
        }
        // process all events at u = nextU
        while (idx < ev.size() && ev[idx].u == nextU) {
            add_interval(ev[idx].v1, ev[idx].v2, ev[idx].delta);
            ++idx;
        }
        currentU = nextU;
    }
    // final stripe if any
    if (currentU <= maxU && gap_exists(currentU, maxU)) return true;

    return false; // no safe point: player captured within T steps
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int tc = 1;

    // reusable buffers for surviveT
    vector<Event> ev;
    vector<ll> vcoords;

    while (cin >> n && n != -1) {
        if (n == 0) {
            cout << "Case " << tc++ << ": never\n";
            continue;
        }

        vector<Pt> P(n);
        for (int i = 0; i < n; ++i) cin >> P[i].x >> P[i].y;

        // quick infinite survival check
        if (!origin_inside(convex_hull(P))) {
            cout << "Case " << tc++ << ": never\n";
            continue;
        }

        // prepare buffer capacities to avoid reallocations
        ev.reserve(2 * n + 10);
        vcoords.reserve(2 * n + 10);

        // Exponential search to find upper bound where survival becomes false
        ll lo = 0, hi = 1;
        while (surviveT(P, hi, ev, vcoords)) {
            lo = hi;
            if (hi > INF64 / 2) { hi = INF64; break; }
            hi <<= 1;
        }

        // Binary search for maximum survivable T
        while (lo < hi) {
            ll mid = (lo + hi + 1) >> 1;
            if (surviveT(P, mid, ev, vcoords)) lo = mid;
            else hi = mid - 1;
        }

        cout << "Case " << tc++ << ": " << lo << '\n';
    }

    return 0;
}