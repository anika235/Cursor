#include <bits/stdc++.h>
using namespace std;

/*  -----------------------------------------------------------
    Helper: maximum matches without column removal.
    Idea (classical CF solution):
       Work from right to left.  For each suffix we keep the set
       of values that appear on the black cells (colour 0) and
       on the white cells (colour 1) of a chess-board colouring
       (row + col) % 2.  For column i (1-based) colour of a_i is
       i % 2,   colour of b_i is (i+1) % 2.
       A column i can be made a match after a suitable sequence
       of allowed copy operations **iff** the two colour sets of
       the suffix i..n intersect (there exists at least one value
       that occurs in both colours).  The best we can do (without
       deletions) is therefore the number of suffix positions that
       satisfy this "good" condition.
       We maintain counters per value for each colour and an
       integer `commonCnt` = number of values that are present in
       **both** colours so far (the suffix currently processed).
       Complexity O(n).
----------------------------------------------------------------*/
static int noRemovalMatches(const vector<int>& a,const vector<int>& b)
{
    const int n = (int)a.size();
    if(n==0) return 0;

    // value range is 1..n, allocate n+1 for convenience
    vector<int> cnt[2];
    cnt[0].assign(n+2,0);
    cnt[1].assign(n+2,0);

    int commonCnt = 0;        // how many distinct values seen in *both* colours
    int ans       = 0;        // running answer

    auto updateColour = [&](int colour,int val){
        // before update, if val already present in both colours we will subtract
        if(cnt[0][val] > 0 && cnt[1][val] > 0) --commonCnt;
        ++cnt[colour][val];
        // after increment, maybe add back
        if(cnt[0][val] > 0 && cnt[1][val] > 0) ++commonCnt;
    };

    // iterate columns from right to left (1-based index i = pos+1)
    for(int pos=n-1; pos>=0; --pos){
        int i = pos+1;                 // convert to 1-based column index
        // add a_i
        int colourA = i & 1;           // row 0
        updateColour(colourA, a[pos]);
        // add b_i
        int colourB = (i + 1) & 1;     // row 1
        updateColour(colourB, b[pos]);

        if(commonCnt>0) ++ans;         // suffix starting at i is "good"
    }
    return ans;
}

/* -------------------------------------------------------------
   Wrapper requested by the exercise.  At the moment we implement
   only the no-removal logic.  Extension with a single deletion
   will come later.
-------------------------------------------------------------*/
static inline int maxMatches(const vector<int>& a,const vector<int>& b)
{
    return noRemovalMatches(a,b);
}

/* --------------------- driver code ------------------------- */
static inline void solveOne(){
    int n; if(!(cin>>n)) return; // safeguard
    vector<int> a(n), b(n);
    for(int &x: a) cin>>x;
    for(int &x: b) cin>>x;
    cout<< maxMatches(a,b) << '\n';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin>>T;
    while(T--) solveOne();
    return 0;
}