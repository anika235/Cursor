// complete solution for ICPC 'chips' problem
// Author: OpenAI GPT-4

#include <bits/stdc++.h>
using namespace std;

/* ---------------- Dinic max-flow ---------------- */
struct Dinic {
    struct Edge { int to, rev, cap; };
    int n, S, T;
    vector<vector<Edge>> g;
    vector<int> level, it;

    explicit Dinic(int N): n(N), g(N) {}

    void addEdge(int u,int v,int c){
        Edge a{v,(int)g[v].size(),c};
        Edge b{u,(int)g[u].size(),0};
        g[u].push_back(a);
        g[v].push_back(b);
    }
    bool bfs(){
        level.assign(n,-1);
        queue<int> q; q.push(S); level[S]=0;
        while(!q.empty()){
            int v=q.front(); q.pop();
            for(const auto &e: g[v]) if(e.cap && level[e.to]==-1){
                level[e.to]=level[v]+1;
                q.push(e.to);
            }
        }
        return level[T]!=-1;
    }
    int dfs(int v,int f){
        if(!f || v==T) return f;
        for(int &i=it[v]; i<(int)g[v].size(); ++i){
            Edge &e=g[v][i];
            if(e.cap && level[e.to]==level[v]+1){
                int ret=dfs(e.to, min(f, e.cap));
                if(ret){
                    e.cap-=ret;
                    g[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
    int maxflow(int s,int t){
        S=s; T=t; int flow=0, aug;
        while(bfs()){
            it.assign(n,0);
            while((aug = dfs(S, INT_MAX))) flow += aug;
        }
        return flow;
    }
};

/* ---------------- Problem specific ---------------- */
static const int INF = 1e9;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, A, B;
    int tc = 1;
    while (cin >> N >> A >> B){
        if(N==0 && A==0 && B==0) break;

        vector<string> board(N);
        for(int i=0;i<N;++i){
            string tmp; cin >> tmp;
            if((int)tmp.size()>N) tmp.resize(N);   // ignore trailing characters (robust input)
            board[i]=tmp;
        }

        vector<int> R0(N,0), C0(N,0), rowFree(N,0), colFree(N,0);
        int fixedTotal = 0, totalDots = 0;

        for(int i=0;i<N;++i){
            for(int j=0;j<N;++j){
                char ch = board[i][j];
                if(ch=='C'){
                    ++R0[i]; ++C0[j]; ++fixedTotal;
                }else if(ch=='.'){
                    ++rowFree[i]; ++colFree[j]; ++totalDots;
                }
            }
        }

        vector<int> diff(N);
        for(int i=0;i<N;++i) diff[i] = C0[i] - R0[i];

        int best = -1; // maximum number of widgets that can be added

        // iterate extra widgets from largest to 0
        for(int extra=totalDots; extra>=0 && best==-1; --extra){
            long long T = fixedTotal + extra;             // total components after placement
            long long P_ll = 1LL * A * T / B;             // per row/column limit
            int P = (int)P_ll;

            bool invalid=false;
            vector<int> Rcap(N), Ccap(N);
            for(int i=0;i<N && !invalid; ++i){
                Rcap[i] = min(rowFree[i], P - R0[i]);
                Ccap[i] = min(colFree[i], P - C0[i]);
                if(Rcap[i] < 0 || Ccap[i] < 0) invalid=true; // already exceed limit
            }
            if(invalid) continue;

            /* Build flow network with lower bounds */
            // Node layout:
            // 0 = original source, 1 = original sink
            // rows: 2 .. 2+N-1
            // cols: 2+N .. 2+2N-1
            int SRC = 0, SNK = 1;
            int rowBase = 2;
            int colBase = rowBase + N;
            int SS   = colBase + N;   // super source for lower-bound transformation
            int TT   = SS + 1;        // super sink

            Dinic dinic(TT+1);
            vector<int> balance(TT+1, 0);

            auto addLB = [&](int u,int v,int lo,int hi){
                dinic.addEdge(u,v,hi - lo);
                balance[u] -= lo;
                balance[v] += lo;
            };

            // source -> rows with capacity Rcap
            for(int i=0;i<N;++i) addLB(SRC, rowBase+i, 0, Rcap[i]);
            // columns -> sink with capacity Ccap
            for(int i=0;i<N;++i) addLB(colBase+i, SNK, 0, Ccap[i]);

            // edges for every open slot '.' (capacity 1)
            for(int i=0;i<N;++i){
                for(int j=0;j<N;++j){
                    if(board[i][j]=='.') addLB(rowBase+i, colBase+j, 0, 1);
                }
            }

            // edges to ensure row/column equality
            for(int i=0;i<N;++i){
                if(diff[i] > 0){
                    // need at least diff[i] flow from row i to column i
                    addLB(rowBase+i, colBase+i, diff[i], INF);
                }else if(diff[i] < 0){
                    // need at least -diff[i] flow from column i to row i
                    addLB(colBase+i, rowBase+i, -diff[i], INF);
                }else{
                    // diff == 0, still tie nodes together (lower=0)
                    addLB(rowBase+i, colBase+i, 0, INF);
                }
            }

            // high-capacity edge sink -> source (closing circulation)
            addLB(SNK, SRC, 0, INF);

            // Connect balances to super source/sink
            int demand = 0;
            for(int v=0; v<=TT; ++v){
                if(balance[v] > 0){
                    dinic.addEdge(SS, v, balance[v]);
                    demand += balance[v];
                }else if(balance[v] < 0){
                    dinic.addEdge(v, TT, -balance[v]);
                }
            }

            if(dinic.maxflow(SS, TT) == demand) best = extra;
        }

        cout << "Case " << tc++ << ": ";
        if(best < 0) cout << "impossible\n";
        else          cout << best << "\n";
    }
    return 0;
}