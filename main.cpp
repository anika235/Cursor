#include <bits/stdc++.h>
using namespace std;

static const int MOD = 998244353;

long long mod_pow(long long a,long long e){
    long long res=1%MOD;
    a%=MOD;
    while(e){
        if(e&1) res=res*a%MOD;
        a=a*a%MOD;
        e>>=1;
    }
    return res;
}

struct Fenwick{
    int n; vector<int> bit;
    Fenwick(int n=0):n(n),bit(n+1,0){}
    void reset(int N){n=N;bit.assign(n+1,0);}    
    void add(int idx,int delta){
        for(int i=idx;i<=n;i+=i&-i) bit[i]+=delta;
    }
    int sumPrefix(int idx){
        int s=0;
        for(int i=idx;i>0;i-=i&-i) s+=bit[i];
        return s;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; if(!(cin>>T)) return 0;
    while(T--){
        int n,q;cin>>n>>q;
        vector<long long> a(n),b(n);
        for(int i=0;i<n;++i) cin>>a[i];
        for(int i=0;i<n;++i) cin>>b[i];

        // read queries first, collect values
        vector<int> op(q), idx(q);
        vector<long long> vals;
        vals.reserve(n*2+q+5);
        for(long long v: a) vals.push_back(v);
        for(long long v: b) vals.push_back(v);

        vector<long long> currA=a, currB=b;
        for(int i=0;i<q;++i){
            cin>>op[i]>>idx[i];
            --idx[i];
            if(op[i]==1){
                currA[idx[i]]++;
                vals.push_back(currA[idx[i]]);
            }else{
                currB[idx[i]]++;
                vals.push_back(currB[idx[i]]);
            }
        }
        // coordinate compression
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int M=vals.size();
        auto getId=[&](long long v){return (int)(lower_bound(vals.begin(), vals.end(), v)-vals.begin())+1;};

        vector<long long> valMod(M+1);
        vector<long long> invMod(M+1);
        for(int i=1;i<=M;++i){
            valMod[i]=vals[i-1]%MOD;
            invMod[i]=mod_pow(valMod[i], MOD-2);
        }

        Fenwick fenwA(M), fenwB(M);
        // counts
        for(long long v:a) fenwA.add(getId(v),1);
        for(long long v:b) fenwB.add(getId(v),1);

        // build exponent map
        vector<int> exps(M+1,0);
        vector<long long> SA=a, SB=b;
        sort(SA.begin(), SA.end(), greater<>());
        sort(SB.begin(), SB.end(), greater<>());
        long long prod=1;
        for(int i=0;i<n;++i){
            long long mv=min(SA[i], SB[i]);
            int id=getId(mv);
            exps[id]++;
            prod=prod*valMod[id]%MOD;
        }
        std::set<int> S;
        for(int i=1;i<=M;++i) if(exps[i]) S.insert(i);

        auto addExp=[&](int id){
            if(exps[id]==0) S.insert(id);
            exps[id]++;
        };
        auto removeExp=[&](int id){
            exps[id]--;
            if(exps[id]==0) S.erase(id);
        };

        auto predecessor=[&](int id){
            auto it=S.lower_bound(id);
            if(it==S.begin()) return -1; // should not happen
            --it; return *it;
        };

        // helper to compute diff at value idx (with current fenwick)
        auto diffAt=[&](int id){
            int prefB=fenwB.sumPrefix(id-1);
            int prefA=fenwA.sumPrefix(id-1);
            return prefB - prefA; // diff = prefB_less - prefA_less
        };

        // Output answers
        vector<long long> answers;
        answers.reserve(q+1);
        answers.push_back(prod);

        vector<long long> curA=a, curB=b; // again for operations processing
        for(int qi=0;qi<q;++qi){
            int o=op[qi], x=idx[qi];
            if(o==1){
                long long old=curA[x];
                long long newv=old+1;
                int idNew=getId(newv);
                int diff_old=diffAt(idNew);
                // update fenwick counts
                fenwA.add(getId(old), -1);
                fenwA.add(idNew, +1);
                curA[x]=newv;
                if(diff_old==-1){
                    // crossing -1 to 0
                    addExp(idNew);
                    int w=predecessor(idNew);
                    removeExp(w);
                    prod=prod*valMod[idNew]%MOD*invMod[w]%MOD;
                }
            }else{
                long long old=curB[x];
                long long newv=old+1;
                int idNew=getId(newv);
                int diff_old=diffAt(idNew);
                // update fenwick counts
                fenwB.add(getId(old), -1);
                fenwB.add(idNew, +1);
                curB[x]=newv;
                if(diff_old==0){
                    // crossing 0 to -1
                    removeExp(idNew);
                    int w=predecessor(idNew);
                    addExp(w);
                    prod=prod*invMod[idNew]%MOD*valMod[w]%MOD;
                }
            }
            answers.push_back(prod);
        }
        // print answers
        for(size_t i=0;i<answers.size();++i){
            if(i) cout<<' ';
            cout<<answers[i]%MOD;
        }
        cout<<"\n";
    }
    return 0;
}