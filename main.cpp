#include <bits/stdc++.h>
using namespace std;

/*
 Asteroid Rangers – minimum number of relay network (MST) changes
 between t = 0 and +∞ while bases move with constant velocity.
 n ≤ 50 → complete graph has ≤ 1225 edges → O(n²) Prim is fine.
 An MST can change only when the relative order of two edge weights
 swaps, i.e. at roots of quadratic polynomials obtained by
 subtracting two squared-distance polynomials.
 The algorithm enumerates all such positive roots, sorts & dedups
 them, then evaluates the MST once inside every open interval
 defined by consecutive event instants plus once at t = 0 and once
 after the last event.  The number of different trees encountered is
 the answer.
*/

const long double EPS       = 1e-9L;   // generic epsilon
const long double TIME_EPS  = 1e-9L;   // epsilon for time comparison
const long double MST_EPS   = 1e-9L;   // epsilon when updating Prim keys
const long double INF_LD    = 1e300L;

struct Poly {              // coefficients of  a t² + b t + c  (squared distance)
    long double a, b, c;
};

// ------------ global input -----------------
int  N;                                   // number of bases ( ≤ 50 )
vector<array<long double,3>> P, V;        // position and velocity

// squared distance of bases i and j at time t
inline long double dist2 (int i, int j, long double t)
{
    long double dx = (P[i][0] + V[i][0]*t) - (P[j][0] + V[j][0]*t);
    long double dy = (P[i][1] + V[i][1]*t) - (P[j][1] + V[j][1]*t);
    long double dz = (P[i][2] + V[i][2]*t) - (P[j][2] + V[j][2]*t); // BUG fixed
    return dx*dx + dy*dy + dz*dz;
}

// Prim in O(N²)            – returns sorted list of encoded edges
vector<int> computeMST (long double t)
{
    vector<long double> key(N, INF_LD);
    vector<int>         parent(N, -1);
    vector<char>        used(N, 0);

    key[0] = 0.0;
    for (int rep = 0; rep < N; ++rep)
    {
        int u = -1; long double best = INF_LD;
        for (int i = 0; i < N; ++i)
            if (!used[i] && key[i] < best) { best = key[i]; u = i; }

        used[u] = 1;
        for (int v = 0; v < N; ++v)
            if (!used[v])
            {
                long double d = dist2(u, v, t);
                if (d + MST_EPS < key[v]) { key[v] = d; parent[v] = u; }
            }
    }

    vector<int> edges; edges.reserve(N-1);
    for (int v = 1; v < N; ++v)
    {
        int u = parent[v];
        int a = min(u,v), b = max(u,v);
        edges.push_back(a*60 + b);        // 60 > max N (=50)  ⇒ unique id
    }
    sort(edges.begin(), edges.end());
    return edges;
}

int main ()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, tc = 1;
    while (cin >> n)                     // until EOF
    {
        N = n;
        P.assign(N, {});
        V.assign(N, {});
        for (int i = 0; i < N; ++i)
        {
            long double x,y,z,vx,vy,vz;
            cin >> x >> y >> z >> vx >> vy >> vz;
            P[i] = {x,y,z};
            V[i] = {vx,vy,vz};
        }

        // ---- build all edge polynomials ----------------------------
        vector<Poly> poly; poly.reserve(N*(N-1)/2);
        for (int i = 0; i < N; ++i)
            for (int j = i+1; j < N; ++j)
            {
                long double dx  = P[i][0]-P[j][0];
                long double dy  = P[i][1]-P[j][1];
                long double dz  = P[i][2]-P[j][2];
                long double dvx = V[i][0]-V[j][0];
                long double dvy = V[i][1]-V[j][1];
                long double dvz = V[i][2]-V[j][2];

                long double a = dvx*dvx + dvy*dvy + dvz*dvz;
                long double b = 2*(dx*dvx + dy*dvy + dz*dvz);
                long double c = dx*dx + dy*dy + dz*dz;
                poly.push_back({a,b,c});
            }

        const int M = (int)poly.size();

        // ---- collect all event instants ---------------------------
        vector<long double> T; T.reserve(M*M/2 + 2);
        T.push_back(0.0L);

        for (int e = 0; e < M; ++e)
            for (int f = e+1; f < M; ++f)
            {
                long double A = poly[e].a - poly[f].a;
                long double B = poly[e].b - poly[f].b;
                long double C = poly[e].c - poly[f].c;

                if (fabsl(A) < EPS)                // linear
                {
                    if (fabsl(B) < EPS) continue; // identical polynomials
                    long double t = -C / B;
                    if (t > TIME_EPS) T.push_back(t);
                }
                else                               // quadratic
                {
                    long double D = B*B - 4*A*C;
                    if (D < -EPS) continue;        // no real roots
                    if (D < 0)  D = 0;             // clamp small negatives
                    long double s = sqrtl(D);
                    long double t1 = (-B - s) / (2*A);
                    long double t2 = (-B + s) / (2*A);
                    if (t1 > TIME_EPS) T.push_back(t1);
                    if (t2 > TIME_EPS) T.push_back(t2);
                }
            }

        sort(T.begin(), T.end());
        T.erase(unique(T.begin(), T.end(),
                      [](long double x,long double y)
                      { return fabsl(x-y) < TIME_EPS; }),
                 T.end());

        // ---- sweep over intervals ---------------------------------
        int modifications = 0;

        vector<int> last = computeMST(0.0L);   // tree at t = 0
        ++modifications;                       // initial installation

        for (size_t i = 1; i < T.size(); ++i)
        {
            long double left  = T[i-1];
            long double right = T[i];
            if (right - left < TIME_EPS) continue;   // zero-width interval
            long double mid = (left + right) * 0.5L; // inside (left, right)
            auto cur = computeMST(mid);
            if (cur != last) { ++modifications; last.swap(cur); }
        }

        // final half-open interval  [T.back(), ∞)
        auto finalTree = computeMST(T.back() + 1.0L);
        if (finalTree != last) ++modifications;

        cout << "Case " << tc++ << ": " << modifications << '\n';
    }
    return 0;
}