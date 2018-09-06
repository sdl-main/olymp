/*
 * Hamming code is used to find and fix single errors in word
 * (in the following code word is 16-bit)
 */

#include <iostream>
#include <string>

using namespace std;

// invert bit (0 -> 1, 1 -> 0)
void invert(char &c) {
    if (c == '1')
        c = '0';
    else
        c = '1';
}

// get data from 21-bit code
string get_num(string& s) {
    string ans = "";
    for (int i = 2; i < s.size(); i *= 2) {
        for (int j = i; j < i * 2 - 1 && j < s.size(); ++j) {
            ans += s[j];
        }
    }
    return ans;
}

// add 5-bit code to 16-bit data
void add_code(string &s) {
    string ss(21, '0');
    int ind = 0;
    for (int i = 2; i < ss.size(); i *= 2) {
        for (int j = i; j < i * 2 - 1 && j < ss.size(); ++j) {
            ss[j] = s[ind++];
            int k = j+1;
            if (ss[j] == '1') {
                for (int q = 0; q < 5; ++q) {
                    if ((k >> q) & 1) {
                        invert(ss[(1 << q) - 1]);
                    }
                }
            }
        }
    }
    s = ss;
}

// fix one error
void repair(string &s) {
    string ss = get_num(s);
    add_code(ss);
    int ind = 0;
    for (int i = 1; i < s.size(); i *= 2) {
        if (s[i-1] != ss[i-1]) {
            ind += i;
        }
    }
    if (ind != 0)
        invert(s[ind-1]);
}


int main() {
    string s = "1111000010101110";
    add_code(s);  // "001011100000101101110"
    invert(s[4]); // "001001100000101101110"
    repair(s);
    cout << get_num(s); // "1111000010101110"
    return 0;
}
