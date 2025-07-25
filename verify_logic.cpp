#include <bits/stdc++.h>
using namespace std;

void verify_case(int n, int k, string s, string expected) {
    cout << "n=" << n << ", k=" << k << ", s=" << s << ", expected=" << expected << endl;
    
    int ones = 0;
    for (char c : s) if (c == '1') ones++;
    cout << "ones=" << ones << endl;
    
    if (ones == 0) {
        cout << "Alice wins (no ones)" << endl;
        return;
    }
    
    if (ones <= k) {
        cout << "Alice wins (ones <= k): " << ones << " <= " << k << endl;
        return;
    }
    
    cout << "ones > k: " << ones << " > " << k << endl;
    
    if (k == 1) {
        cout << "k=1, ones >= 2, so Alice cannot win" << endl;
        cout << "My answer: Bob" << endl;
    } else {
        int hits = (n - 1) / k + 1;
        bool aliceWins = (hits < k);
        cout << "k>1, hits=" << hits << ", k=" << k << ", hits<k=" << aliceWins << endl;
        cout << "My answer: " << (aliceWins ? "Alice" : "Bob") << endl;
    }
    
    cout << "Expected: " << expected << endl;
    cout << "--------" << endl;
}

int main() {
    verify_case(5, 2, "11011", "Bob");
    verify_case(7, 4, "1011011", "Alice");
    verify_case(6, 1, "010000", "Alice");
    verify_case(4, 1, "1111", "Bob");
    verify_case(8, 3, "10110110", "Bob");
    verify_case(6, 4, "111111", "Alice");
    
    return 0;
}