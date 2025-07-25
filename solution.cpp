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
        
        set<string> candidates;
        candidates.insert(l);
        candidates.insert(r);

        int n = l.length();
        int i = 0;
        while (i < n && l[i] == r[i]) i++;

        if (i < n) {
            string common = l.substr(0, i);
            
            // Middle digits between l[i] and r[i]
            for (char d = l[i] + 1; d < r[i]; d++) {
                string cand = common + d;
                for (int j = i+1; j < n; j++) {
                    for (char c = '0'; c <= '9'; c++) {
                        if (c != l[j] && c != r[j]) {
                            cand += c;
                            break;
                        }
                    }
                    if (cand.length() <= i + 1 + j - i - 1) {
                        // If we couldn't find a digit different from both l[j] and r[j]
                        cand += '0'; // fallback
                    }
                }
                if (cand.length() == n) {
                    candidates.insert(cand);
                }
            }

            // Candidate for d = l[i]
            string cand1 = common + l[i];
            bool tight = true;
            for (int j = i+1; j < n; j++) {
                bool found = false;
                char start = tight ? l[j] : '0';
                for (char c = start; c <= '9'; c++) {
                    if (c != l[j] && c != r[j]) {
                        cand1 += c;
                        if (c > l[j]) tight = false;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cand1 += l[j];
                }
            }
            candidates.insert(cand1);

            // Candidate for d = r[i]
            string cand2 = common + r[i];
            tight = true;
            for (int j = i+1; j < n; j++) {
                bool found = false;
                char end = tight ? r[j] : '9';
                for (char c = '0'; c <= end; c++) {
                    if (c != l[j] && c != r[j]) {
                        cand2 += c;
                        if (c < r[j]) tight = false;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cand2 += r[j];
                }
            }
            candidates.insert(cand2);
        }

        // Evaluate all candidates
        int best = 2*n;
        for (const auto& cand : candidates) {
            best = min(best, f(l, cand) + f(cand, r));
        }
        cout << best << "\n";
    }
    
    return 0;
}