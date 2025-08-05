// Competitive Programming Solution
// Problem: To Add or to Multiply (ICPC World Finals 2005 Problem A)
// Author: ChatGPT assistant
//
// The processor applies only two operations on an integer: A (add constant a) and M (multiply by m).
// Any program consisting of these operations computes an affine function
//      f(x) = m^k * x + a * C,
// where k = #M in the program, and C is a non-negative integer depending on the positions of the A’s.
// The constant C can be written in base m using exactly (k+1) digits c_k … c_0 with 0 <= c_i < m :
//      C = Σ_{i=0..k} c_i * m^i .
// Digit c_i counts the A’s that are executed while exactly i multiplications are still to come.
// Therefore the (lexicographic) program corresponding to the digit vector is obtained by scanning from
// i = k down to 0 and outputting
//      c_i consecutive ‘A’, then (if i>0) one ‘M’.
// 
// The task is to find the shortest program such that for every input x in [p,q] the result y is in [r,s].
// This translates to choosing integers k>=0 and C>=0 that satisfy
//      r <= m^k * p + a*C                         (1)
//      m^k * q + a*C <= s                        (2)
//               0 <= C  (obvious)                (3)
// Together:
//      L <= C <= U ,   where
//           L = ceil((r - m^k * p)/a),   U = floor((s - m^k * q)/a).
// For a fixed k we must decide whether an integer C in [L,U] exists whose digit sum in base m
// (with k+1 digits) is minimal.  The total program length is
//          len = k + digitSum_m(C).
// We iterate over feasible k (m^k * q <= s) and, for each, search the interval [L,U] for the number C
// that minimises digitSum_m().  This is done by a standard digit-DP that works on the base-m digits of
// L and U (both padded to k+1 digits).  Because m can be as large as 10^9 we cannot iterate over all
// possible digits (0..m-1).  However, to minimise the *sum* of digits we only ever have to inspect up
// to four candidates at each position:  lo, lo+1, hi-1, hi, where
//      lo = lower bound for the current digit,  hi = upper bound.  Any other digit either produces the
// same flags (tight to low/high) as one of these four or is larger than all of them and therefore can
// never improve the total sum.
// The digit-DP returns the minimal digit sum; a second back-tracking pass constructs the *lexicographically
// smallest* digit vector with that minimal sum by always trying larger digits first (more leading ‘A’s
// -> lexicographically smaller).
// Finally, among all k we retain the program with minimal length, and among ties the one that is
// lexicographically smallest (comparing the run-length encoded strings without expanding huge blocks).
//
// Special cases:
// 1) If [p,q] is already inside [r,s] the empty program is optimal.
// 2) If m == 1, multiplications have no effect.  Hence k must be 0 and C is chosen by a simple interval
//    check (digit sum is just C in base-1).
//
// Complexity:  k is at most 60 (because m >= 2 and m^k * q <= 1e9),
//              digit-DP uses O((k+1)*4*4) states per k, totally negligible.
// Memory:      O(k) per test case.

#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
using i128  = __int128_t;
const int64 INF64 = (int64)4e18;

// ──────────────────────────────────────────────────────────────────────────────
// integer helpers (128-bit safe)
static inline int64 floor_div(i128 num, int64 den) {
    // floor division where den > 0
    if (den <= 0) throw;
    if (num >= 0) return (int64)(num / den);
    // for negative numerator ensure toward -inf
    i128 t = -num;
    int64 res = (int64)(-( (t + den - 1) / den ));
    return res;
}
static inline int64 ceil_div(i128 num, int64 den) {
    return -floor_div(-num, den);
}

// revert to MSB order:
static vector<int64> to_digits(i128 value, int len, int64 base) {
    // digits[0] is the most significant digit (coefficient of base^{len-1})
    vector<int64> d(len, 0);
    for (int i = len - 1; i >= 0; --i) {
        d[i] = (int64)(value % base);
        value /= base;
    }
    return d; // value assumed to fit into len digits
}

// Compare two programs given as run-length encoded groups (char, count).
// Returns true if a < b lexicographically.
static bool lex_less(const vector<pair<char,int64>>& A,
                     const vector<pair<char,int64>>& B) {
    size_t ia = 0, ib = 0;
    int64 remA = (ia < A.size() ? A[ia].second : 0);
    int64 remB = (ib < B.size() ? B[ib].second : 0);
    while (ia < A.size() && ib < B.size()) {
        char ca = A[ia].first;
        char cb = B[ib].first;
        if (ca != cb) return ca < cb; // 'A' < 'M'
        int64 take = min(remA, remB);
        remA -= take;
        remB -= take;
        if (remA == 0) {
            ++ia;
            if (ia < A.size()) remA = A[ia].second;
        }
        if (remB == 0) {
            ++ib;
            if (ib < B.size()) remB = B[ib].second;
        }
    }
    return ia == A.size() && ib != B.size();
}

// Build the run-length encoding from digit vector (c_k … c_0) with k Ms.
static vector<pair<char,int64>> build_groups(const vector<int64>& msbDigits) {
    // msbDigits[0] is c_k (most significant). We need access digits by exponent.
    int k = (int)msbDigits.size() - 1;
    vector<pair<char,int64>> grp;
    auto push = [&](char c, int64 cnt) {
        if (cnt == 0) return;
        if (!grp.empty() && grp.back().first == c) grp.back().second += cnt;
        else grp.push_back({c, cnt});
    };
    for (int exp = k; exp >= 0; --exp) {
        // Index in msbDigits
        int idx = k - exp; // c_k at index 0
        push('A', msbDigits[idx]);
        if (exp > 0) push('M', 1);
    }
    return grp;
}

// Digit DP per k to obtain minimal digit sum and lexicographically smallest digits.
struct DigitDP {
    int len;              // k+1
    int64 base;           // m
    vector<int64> Ld, Ud; // lower/upper bound digits (len)

    // memoisation arrays
    int64 dp[65][2][2];
    bool  vis[65][2][2];

    DigitDP(int len_, int64 base_, const vector<int64>& L, const vector<int64>& U)
        : len(len_), base(base_), Ld(L), Ud(U) {
        memset(vis, 0, sizeof(vis));
    }

    int64 solve(int pos, int tightL, int tightH) {
        if (pos == len) return 0;
        if (!tightL && !tightH) return 0; // free, choose all zeros
        int64 &memo = dp[pos][tightL][tightH];
        if (vis[pos][tightL][tightH]) return memo;
        vis[pos][tightL][tightH] = true;
        int64 lo = tightL ? Ld[pos] : 0;
        int64 hi = tightH ? Ud[pos] : base - 1;
        vector<int64> cand = {lo};
        if (lo + 1 <= hi) cand.push_back(lo + 1);
        if (hi != lo) cand.push_back(hi);
        if (hi > 0 && hi - 1 >= lo && hi - 1 != lo && hi - 1 != lo + 1)
            cand.push_back(hi - 1);
        sort(cand.begin(), cand.end());
        cand.erase(unique(cand.begin(), cand.end()), cand.end());

        int64 best = INF64;
        for (int64 d : cand) {
            int ntL = tightL && (d == lo);
            int ntH = tightH && (d == hi);
            // corrections: need comparing to Ld[pos] / Ud[pos]
            if (tightL) ntL = (d == Ld[pos]);
            if (tightH) ntH = (d == Ud[pos]);
            int64 nxt = solve(pos + 1, ntL, ntH);
            if (nxt == INF64) continue;
            best = min(best, d + nxt);
        }
        return memo = best;
    }

    bool construct(int pos, int tightL, int tightH, int64 rem, vector<int64>& out) {
        if (pos == len) return rem == 0;

        int64 lo = tightL ? Ld[pos] : 0;
        int64 hi = tightH ? Ud[pos] : base - 1;

        // prepare candidate digits in preference order (bigger first for lexicographic order)
        vector<int64> cand = {hi, lo};
        if (hi > 0 && hi - 1 >= lo) cand.push_back(hi - 1);
        if (lo + 1 <= hi) cand.push_back(lo + 1);
        sort(cand.begin(), cand.end(), greater<int64>());
        cand.erase(unique(cand.begin(), cand.end()), cand.end());

        for (int64 d : cand) {
            int ntL = tightL && (d == Ld[pos]);
            int ntH = tightH && (d == Ud[pos]);
            int64 nxt_min = solve(pos + 1, ntL, ntH);
            if (nxt_min == INF64) continue;
            if (d + nxt_min != rem) continue;
            out[pos] = d;
            if (construct(pos + 1, ntL, ntH, rem - d, out)) return true;
        }
        return false;
    }
};

// main per test case
struct Program {
    int64 length; // total operations
    vector<pair<char,int64>> groups; // run-length encoding, or empty when impossible
    bool exists;
    Program() : length(INF64), exists(false) {}
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 a,m,p,q,r,s; int caseNo=1;
    while(cin>>a>>m>>p>>q>>r>>s){
        if(a==0 && m==0 && p==0 && q==0 && r==0 && s==0) break;

        Program best;

        // Special: empty program?
        if(r <= p && q <= s){
            best.exists = true;
            best.length = 0;
            best.groups = {}; // empty
        }

        // Special: m == 1  => no sense in using M
        if(m==1){
            i128 numL = (i128)r - (i128)p;
            i128 numU = (i128)s - (i128)q;
            int64 low = ceil_div(numL, a);
            int64 up  = floor_div(numU, a);
            if(up >= low && up >= 0){
                int64 C = max<int64>(0, low); // minimal C
                int64 len = C; // k=0
                if(!best.exists || len < best.length){
                    best.exists = true;
                    best.length = len;
                    if(C==0){
                        best.groups.clear();
                    }else{
                        best.groups = {{'A', C}};
                    }
                }else if(len == best.length){
                    vector<pair<char,int64>> candidate;
                    if(C>0) candidate.push_back({'A',C});
                    if(lex_less(candidate,best.groups)) best.groups = candidate;
                }
            }
            cout << "Case " << caseNo++ << ": " << (best.exists ? (best.groups.empty()?"empty":"") : "impossible");
            if(best.exists && !best.groups.empty()){
                for(size_t i=0;i<best.groups.size();++i){
                    if(i) cout << ' ';
                    cout << best.groups[i].second << best.groups[i].first;
                }
            }
            cout << "\n";
            continue;
        }

        // Iterate over possible k (number of Ms)
        i128 M = 1; // m^k
        for(int k=0; ; ++k){
            // Check feasibility: m^k * q <= s (otherwise upper bound negative)
            if( (i128)q * M > (i128)s) break; // no larger k will work either

            // compute C interval
            i128 numL = (i128)r - M * (i128)p;
            i128 numU = (i128)s - M * (i128)q;
            int64 low = ceil_div(numL, a);
            int64 up  = floor_div(numU, a);

            if(up < low || up < 0){
                // advance power for next k
                M *= (i128)m;
                if(M > (i128)4e18) break;
                continue;
            }else{
                // adjust low to >=0
                if(low < 0) low = 0;
                int len = k + 1; // digits

                // Check representability: maximum value with len digits is base^len - 1
                auto canRepresent = [&](int64 val)->bool{
                    i128 cap = 1;
                    for(int i=0;i<len;i++){
                        cap *= (i128)m;
                        if(cap > (i128)val) return true;
                        if(cap > (i128)4e18) return true; // overflow guard, certainly > val
                    }
                    return false; // still <= val
                };
                if(!canRepresent(low)) {
                    M *= (i128)m;
                    if(M > (i128)4e18) break;
                    continue;
                }
                
                // clamp upper bound if necessary
                auto clampUpper = [&](int64 val)->int64{
                    i128 cap = 1;
                    for(int i=0;i<len;i++) cap *= (i128)m;
                    i128 maxVal = cap - 1;
                    if((i128)val > maxVal) return (int64)maxVal;
                    return val;
                };
                int64 upClamped = clampUpper(up);

                vector<int64> Ld = to_digits(low, len, m);
                vector<int64> Ud = to_digits(upClamped,  len, m);
                DigitDP dp(len, m, Ld, Ud);
                int64 minSum = dp.solve(0, 1, 1);
                
                if(minSum != INF64){
                    int64 totalLen = k + minSum;
                    vector<int64> digits(len,0);
                    // construct always succeeds as dp ensures existence
                    bool ok = dp.construct(0, 1, 1, minSum, digits);
                    vector<pair<char,int64>> groups = build_groups(digits);
                    if(!best.exists || totalLen < best.length){
                        best.exists = true;
                        best.length = totalLen;
                        best.groups = move(groups);
                    }else if(totalLen == best.length){
                        if(lex_less(groups, best.groups)) best.groups = move(groups);
                    }
                }
            }
            // next power of m (handled also in early-continue cases)
            M *= (i128)m;
            if(M > (i128)4e18) break; // prevent overflow although input bounds keep us safe
        }

        cout << "Case " << caseNo++ << ": ";
        if(!best.exists) cout << "impossible\n";
        else if(best.groups.empty()) cout << "empty\n";
        else {
            for(size_t i=0;i<best.groups.size();++i){
                if(i) cout << ' ';
                cout << best.groups[i].second << best.groups[i].first;
            }
            cout << "\n";
        }
    }
    return 0;
}