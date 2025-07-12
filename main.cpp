#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    if(!(cin>>S)) return 0;
    int numA=0,numY=0,numN=0,numG=0,numOther=0;
    for(char ch: S){
        if(ch=='A'||ch=='E'||ch=='I'||ch=='O'||ch=='U') numA++;
        else if(ch=='Y') numY++;
        else if(ch=='N') numN++;
        else if(ch=='G') numG++;
        else numOther++;
    }
    int n=S.size();
    int best=0;
    int maxK = n/3; // maximum syllables possible by raw length
    for(int k=1;k<=maxK;++k){
        int yV = max(0, k - numA); // Y letters used as vowel
        if(yV>numY) continue; // not enough vowels
        int yConsAvail = numY - yV; // remaining Y that can be consonant
        // Total single consonant letters available before using NG digraphs
        int singleBase = numOther + numN + numG + yConsAvail;
        // upper bound for NG digraphs
        int limitByLetters = min(numN, numG);
        int limitByTokens = 2*k; // can't exceed total consonant tokens needed
        int limitBySingles = singleBase - 2*k; // derived from availability condition
        int upperY = min({limitByTokens, limitByLetters, limitBySingles});
        if(upperY < 0) continue; // singles not enough, impossible
        int length = 3*k + upperY; // length of word
        if(length>best) best=length;
    }
    cout<<best<<"\n";
    return 0;
}
