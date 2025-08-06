#include <bits/stdc++.h>
using namespace std;

/*  Optimised kinetic MST for "Asteroid Rangers" (n ≤ 50)
    -----------------------------------------------------
    Let w_e(t) be the squared distance of edge e at time t (quadratic).
    MST can change only when weights of two edges become equal *and*
    exactly one of them is in the current MST.  We therefore process
    all O(m²) edge-edge intersection events (m ≤ 1225) once, but we
    recompute the MST only for event groups that satisfy the condition
    above.  Because the MST of n vertices has n-1 ≤ 49 edges, every
    MST change replaces exactly one edge, hence the total number of
    changes – and therefore the number of expensive MST computations –
    is O(m) in the worst case ( ≤ 1225 ), easily fast.
*/

// -------- generic constants -------------------------------------------------
const long double EPS = 1e-8L;     // uniform epsilon for everything
const long double INF = 1e300L;

// -------- input data --------------------------------------------------------
int  N;                                        // #bases  ( ≤ 50 )
vector<array<long double,3>> P, V;             // positions / velocities

inline long double dist2(int i,int j,long double t)
{
    long double dx = (P[i][0] + V[i][0]*t) - (P[j][0] + V[j][0]*t);
    long double dy = (P[i][1] + V[i][1]*t) - (P[j][1] + V[j][1]*t);
    long double dz = (P[i][2] + V[i][2]*t) - (P[j][2] + V[j][2]*t);
    return dx*dx + dy*dy + dz*dz;
}

inline int edgeId(int u,int v){      // unique id in   [0 , N*N)
    if(u>v) swap(u,v);
    return u*N + v;
}

// ---------- Prim ( O(N²) ) --------------------------------------------------
vector<int> prim(long double t)
{
    vector<long double> key(N, INF);
    vector<int> parent(N,-1);
    vector<char> used(N,0);
    key[0]=0;
    for(int rep=0; rep<N; ++rep){
        int u=-1; long double best=INF;
        for(int i=0;i<N;++i) if(!used[i] && key[i] < best){ best=key[i]; u=i; }
        used[u]=1;
        for(int v=0; v<N; ++v) if(!used[v]){
            long double d = dist2(u,v,t);
            if(d + EPS < key[v]){ key[v]=d; parent[v]=u; }
        }
    }
    vector<int> edges; edges.reserve(N-1);
    for(int v=1; v<N; ++v) edges.push_back(edgeId(v,parent[v]));
    sort(edges.begin(), edges.end());
    return edges;
}

// ---------- main ------------------------------------------------------------
struct Poly{ long double a,b,c; };
struct Event{ long double t; int e,f; };        // edges e,f cross at time t

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, tc=1;
    while(cin>>n){
        N=n;
        P.assign(N,{}); V.assign(N,{});
        for(int i=0;i<N;++i){
            long double x,y,z,vx,vy,vz; cin>>x>>y>>z>>vx>>vy>>vz;
            P[i]={x,y,z}; V[i]={vx,vy,vz};
        }

        // ---- build edge list + polynomials --------------------------------
        struct EdgeInfo{ int u,v; Poly p; };
        vector<EdgeInfo> edges; edges.reserve(N*(N-1)/2);
        for(int i=0;i<N;++i) for(int j=i+1;j<N;++j){
            long double dx=P[i][0]-P[j][0];
            long double dy=P[i][1]-P[j][1];
            long double dz=P[i][2]-P[j][2];
            long double dvx=V[i][0]-V[j][0];
            long double dvy=V[i][1]-V[j][1];
            long double dvz=V[i][2]-V[j][2];
            long double a=dvx*dvx+dvy*dvy+dvz*dvz;
            long double b=2*(dx*dvx + dy*dvy + dz*dvz);
            long double c=dx*dx + dy*dy + dz*dz;
            edges.push_back({i,j,{a,b,c}});
        }
        const int M = edges.size();

        // ---- collect all intersection events -----------------------------
        vector<Event> ev; ev.reserve((size_t)M*M/2);
        for(int e=0;e<M;++e){
            auto [ue,ve,pe]=edges[e];
            for(int f=e+1;f<M;++f){
                auto [uf,vf,pf]=edges[f];
                long double A = pe.a - pf.a;
                long double B = pe.b - pf.b;
                long double C = pe.c - pf.c;
                if(fabsl(A) < EPS){
                    if(fabsl(B) < EPS) continue;           // identical curves
                    long double t = -C/B;
                    if(t > EPS) ev.push_back({t,e,f});
                }else{
                    long double D = B*B - 4*A*C;
                    if(D < -EPS) continue;
                    if(D < 0) D=0;
                    long double s = sqrtl(D);
                    long double t1 = (-B - s)/(2*A);
                    long double t2 = (-B + s)/(2*A);
                    if(t1 > EPS) ev.push_back({t1,e,f});
                    if(t2 > EPS) ev.push_back({t2,e,f});
                }
            }
        }
        sort(ev.begin(),ev.end(),[](const Event&a,const Event&b){return a.t<b.t;});

        // ---- initial MST --------------------------------------------------
        vector<int> curTree = prim(0.0L);
        vector<char> inMST(N*N,0);
        for(int id:curTree) inMST[id]=1;
        int changes = 1;                        // initial installation

        // ---- scan events --------------------------------------------------
        size_t i=0; const size_t E = ev.size();
        while(i<E){
            long double t = ev[i].t;
            size_t j=i;                    // group events with same time
            while(j<E && fabsl(ev[j].t - t) < EPS) ++j;

            bool mayChange=false;
            for(size_t k=i;k<j && !mayChange;++k){
                int id1=edgeId(edges[ev[k].e].u, edges[ev[k].e].v);
                int id2=edgeId(edges[ev[k].f].u, edges[ev[k].f].v);
                mayChange = (inMST[id1] ^ inMST[id2]);
            }
            if(mayChange){
                long double sample;
                if(j<E && ev[j].t - t > EPS)
                    sample = (t + ev[j].t)*0.5L;
                else
                    sample = t + 1e-4L;           // tiny step forward

                auto newTree = prim(sample);
                if(newTree != curTree){
                    ++changes;
                    fill(inMST.begin(), inMST.end(), 0);
                    for(int id:newTree) inMST[id]=1;
                    curTree.swap(newTree);
                }
            }
            i=j;
        }

        // ---- after last event --------------------------------------------
        long double lastT = (E? ev.back().t : 0.0L);
        auto finalTree = prim(lastT + 1.0L);
        if(finalTree != curTree) ++changes;

        cout << "Case "<<tc++<<": "<<changes<<"\n";
    }
    return 0;
}