#include <bits/stdc++.h>
using namespace std;

/*
   ICPC archive problem  “Keys”  (rings + keys, Adam vs Brenda)
   -----------------------------------------------------------
   input:  lines of two-character strings (lowercase = ring, uppercase = key),
           ended by a line 0  .  Several test cases.
   output:  Case i:  minKeyOps  minRingOps   or  impossible

   Strategy: brute force over every possible final distribution of keys and
   (free) rings.  For each candidate state compute #key detach/attach moves
   and the minimum number of ring-ring edits needed to ensure that Adam’s
   and Brenda’s rings are each internally connected and mutually separate.
*/

struct Graph {
    int n;                             // #rings (vertices)
    vector<vector<int>> adj;           // ring-ring edges (multi-edges irrelevant)
    Graph(int N=0): n(N), adj(N){}
    void addEdge(int u,int v){ adj[u].push_back(v); adj[v].push_back(u);}    
    /* connected components restricted to mask */
    int components(uint32_t mask) const{
        vector<char> vis(n,0);
        int cnt=0;
        for(int v=0;v<n;++v) if((mask>>v&1)&&!vis[v]){
            ++cnt;
            queue<int>q; q.push(v); vis[v]=1;
            while(!q.empty()){
                int x=q.front(); q.pop();
                for(int y:adj[x]) if((mask>>y&1)&&!vis[y]) vis[y]=1,q.push(y);
            }
        }
        return cnt;
    }
    /* number of edges with both ends in mask */
    int edgesInside(uint32_t mask) const{
        int e=0;
        for(int u=0;u<n;++u) if(mask>>u&1)
            for(int v:adj[u]) if(v<u && (mask>>v&1)) ++e;
        return e;
    }
};

static inline int ringCost(const Graph& G, uint32_t mask){
    if(!mask) return 0;          // no rings owned, no cost
    int r = __builtin_popcount(mask);
    int e = G.edgesInside(mask);
    int c = G.components(mask);
    /* keep (r-c) existing edges to build a spanning forest, delete the rest;
       then add (c-1) edges to connect the forest → connected component */
    return (e - (r - c))       /* deletions */
         + (c - 1);            /* insertions */
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; int tc=0;
    while(true){
        vector<string> lines;
        while(cin>>s && s!="0") lines.push_back(s);
        if(lines.empty()) break;
        ++tc;

        int ringId[26]; fill(begin(ringId), end(ringId), -1);
        auto getRing=[&](int idx){ if(ringId[idx]==-1) ringId[idx]=*max_element(begin(ringId),end(ringId))+1; return ringId[idx];};
        // But easier: maintain counter
        int R=0;
        auto mapRing=[&](int idx){ if(ringId[idx]==-1) ringId[idx]=R++; return ringId[idx]; };

        vector<int> keyRing(26,-1);
        vector<pair<int,int>> ringEdges;
        for(auto &t: lines){
            char a=t[0], b=t[1];
            if(islower(a)&&islower(b)){
                int u=mapRing(a-'a');
                int v=mapRing(b-'a');
                ringEdges.push_back({u,v});
            }else if(islower(a)&&isupper(b)){
                keyRing[b-'A']=mapRing(a-'a');
            }else if(isupper(a)&&islower(b)){
                keyRing[a-'A']=mapRing(b-'a');
            }
        }
        Graph G(R);
        for(auto &e:ringEdges) G.addEdge(e.first,e.second);

        vector<int> keys;
        for(int k=0;k<26;++k) if(keyRing[k]!=-1) keys.push_back(k);
        int K=keys.size();

        const int FULLK = 1<<K;
        // prepare counts per ring for fast update later
        vector<int> best={INT_MAX, INT_MAX}; // keyOps, ringOps

        // For each assignment of keys to Adam (bit set) / Brenda (unset)
        for(int mask=1; mask<FULLK-1; ++mask){
            vector<int> aCnt(R,0), bCnt(R,0);
            for(int i=0;i<K;++i){
                int r = keyRing[keys[i]];
                if(mask>>i&1) ++aCnt[r]; else ++bCnt[r];
            }
            int keyOpsBase=0;               // unavoidable moves so far
            uint32_t fixedA=0, fixedB=0;    // rings already decided
            vector<int> amb;                // ambiguous (equal counts >0)
            for(int r=0;r<R;++r){
                if(aCnt[r] && !bCnt[r]) fixedA |= 1u<<r;
                else if(bCnt[r] && !aCnt[r]) fixedB |= 1u<<r;
                else if(aCnt[r] && bCnt[r]){
                    if(aCnt[r]>bCnt[r]){
                        keyOpsBase += 2*bCnt[r];
                        fixedA |= 1u<<r;
                    }else if(bCnt[r]>aCnt[r]){
                        keyOpsBase += 2*aCnt[r];
                        fixedB |= 1u<<r;
                    }else{
                        keyOpsBase += aCnt[r]; // detach one side (min)
                        amb.push_back(r);
                    }
                }
            }
            if(keyOpsBase > best[0]) continue; // prune

            int ambN=amb.size();
            const int FULLA = 1<<ambN;
            for(int sub=0; sub<FULLA; ++sub){
                uint32_t RA=fixedA, RB=fixedB;
                int extraKeyOps=0;
                for(int i=0;i<ambN;++i){
                    int r=amb[i];
                    if(sub>>i&1) RA |= 1u<<r; else RB |= 1u<<r;
                }
                if(RA==0||RB==0) continue; // both need at least one ring

                int ringOps = ringCost(G, RA) + ringCost(G, RB);
                // edges crossing sets must be removed
                for(auto&e:ringEdges)
                    if( ((RA>>e.first&1)!=(RA>>e.second&1)) ) ++ringOps;

                int keyOps = keyOpsBase + extraKeyOps; // extraKeyOps is 0 here
                if(keyOps < best[0] || (keyOps==best[0] && ringOps<best[1]))
                    best = {keyOps, ringOps};
            }
        }
        cout << "Case " << tc << ": ";
        if(best[0]==INT_MAX) cout << "impossible\n";
        else                  cout << best[0] << ' ' << best[1] << "\n";
    }
}