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
        // compute initial product using counts of elements >= v
        vector<int> freqA(M+1,0), freqB(M+1,0);
        for(long long v: a) ++freqA[getId(v)];
        for(long long v: b) ++freqB[getId(v)];

        long long prod = 1;
        long long cumA = 0, cumB = 0, prevMin = 0;
        for(int id=M; id>=1; --id){ // iterate values in decreasing order
            cumA += freqA[id];
            cumB += freqB[id];
            long long curMin = min(cumA, cumB);
            long long exp = curMin - prevMin;
            if(exp){
                prod = prod * mod_pow(valMod[id], exp) % MOD;
                prevMin = curMin;
            }
        }

        // helpers to obtain counts of elements >= value with compressed id
        auto count_ge_A = [&](int id){
            return n - fenwA.sumPrefix(id-1);
        };
        auto count_ge_B = [&](int id){
            return n - fenwB.sumPrefix(id-1);
        };
        
        // Output answers
        vector<long long> answers;
        answers.reserve(q+1);
        answers.push_back(prod);

        vector<long long> curA=a, curB=b; // again for operations processing
        for(int qi=0; qi<q; ++qi){
            int o = op[qi], x = idx[qi];
            if(o == 1){
                long long oldVal = curA[x];
                long long newVal = oldVal + 1;
                int idOld = getId(oldVal);
                int idNew = getId(newVal);

                int geA_before = count_ge_A(idNew); // before modification
                int geB = count_ge_B(idNew);

                if(geA_before < geB){
                    prod = prod * valMod[idNew] % MOD * invMod[idOld] % MOD;
                }

                // update fenwick
                fenwA.add(idOld, -1);
                fenwA.add(idNew, 1);
                curA[x] = newVal;
            }else{
                long long oldVal = curB[x];
                long long newVal = oldVal + 1;
                int idOld = getId(oldVal);
                int idNew = getId(newVal);

                int geB_before = count_ge_B(idNew); // before modification
                int geA = count_ge_A(idNew);

                if(geB_before < geA){
                    prod = prod * valMod[idNew] % MOD * invMod[idOld] % MOD;
                }

                // update fenwick
                fenwB.add(idOld, -1);
                fenwB.add(idNew, 1);
                curB[x] = newVal;
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