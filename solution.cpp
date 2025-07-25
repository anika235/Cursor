#include <iostream>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

// Function to count matching digits between two numbers
int f(const string& a, const string& b) {
    int count = 0;
    for (int i = 0; i < a.length(); i++) {
        if (a[i] == b[i]) {
            count++;
        }
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        string l, r;
        cin >> l >> r;
        
        int n = l.length();
        int best = n * 2;
        
        // Use a set to avoid duplicate candidates
        set<string> candidates;
        
        // Always try the boundaries
        candidates.insert(l);
        candidates.insert(r);
        
        // Generate candidates by modifying each position
        for (int pos = 0; pos < n; pos++) {
            for (char d = '0'; d <= '9'; d++) {
                string temp = l;
                temp[pos] = d;
                if (temp >= l && temp <= r) {
                    candidates.insert(temp);
                }
            }
        }
        
        // Build x greedily: for each position, choose digit that minimizes contribution
        string x = l;
        for (int pos = 0; pos < n; pos++) {
            int best_contrib = 2;
            char best_digit = x[pos];
            
            for (char d = '0'; d <= '9'; d++) {
                string temp = x;
                temp[pos] = d;
                
                if (temp >= l && temp <= r) {
                    int contrib = 0;
                    if (d == l[pos]) contrib++;
                    if (d == r[pos]) contrib++;
                    
                    if (contrib < best_contrib) {
                        best_contrib = contrib;
                        best_digit = d;
                    }
                }
            }
            x[pos] = best_digit;
        }
        candidates.insert(x);
        
        // Test all candidates
        for (const string& candidate : candidates) {
            best = min(best, f(l, candidate) + f(candidate, r));
        }
        
        cout << best << "\n";
    }
    
    return 0;
}