/*
 * Given a text and dictionary n words. 
 * Find all occurencies of these words in text in linear time
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

const int N = 1e5;
const int K = 26;

vector<int> ans[N]; // all occurencies of all strings

// node of a trie
struct node {
    int next[K]; // real edges 
    int go[K]; // go[x] is a node which we will visit if trying to go by letter x
               // (by real edges or not)
    int p = 0; // parent node
    int link = -1; // suffix link
    int up = -1; // compressed suffix link
    char pch = 0; // char at edge connecting this node with parent
    bool leaf = 0; // true if this node is a leaf
    vector<int> inds; // indices of words ending in this node
    node() {
        memset(next, -1, sizeof next);
        memset(go, -1, sizeof go);
    }
    node(int p, char pch) : p(p), pch(pch) {
        memset(next, -1, sizeof next);
        memset(go, -1, sizeof go);
    }
} t[N];

int sz = 1; // size of a trie

// add a string to a trie
void add_string(string s, int ind) {
    int v = 0;
    for (int i = 0; i < s.size(); ++i) {
        char c = s[i] - 'a';
        if (t[v].next[c] == -1) {
            t[v].next[c] = sz;
            t[sz++] = node(v, c);
        }
        v = t[v].next[c];
    }
    t[v].leaf = 1;
    t[v].inds.push_back(ind);
}

int go(int v, char c);

// get link to the end of the longest suffix of string ending in v
// that is present in trie
int get_link(int v) {
    if (t[v].link == -1) {
        if (v == 0 || t[v].p == 0)
            t[v].link = 0;
        else
            t[v].link = go(get_link(t[v].p), t[v].pch);
    }
    return t[v].link;
}

// go from node v by char c
// if there is no real edge for c, go from suffix link
int go(int v, char c) {
    if (t[v].go[c] == -1) {
        if (t[v].next[c] != -1)
            t[v].go[c] = t[v].next[c];
        else
            t[v].go[c] = v == 0 ? 0 : go(get_link(v), c);
    }
    return t[v].go[c];
}

// get first suffix link (or suffix link of suffix link, etc.)
// that is leaf
int get_up(int v) {
    if (t[v].up == -1) {
        int l = get_link(v);
        if (l == 0 || t[l].leaf)
            t[v].up = l;
        else
            t[v].up = get_up(l);
    }
    return t[v].up;
}

int main() {
    // read text and dictionary
    string text;
    cin >> text;
    int n;
    cin >> n;
    vector<string> dict(n);
    for (int i = 0; i < n; ++i) {
        cin >> dict[i];
        add_string(dict[i], i);
    }

    int v = 0; // current node
    for (int i = 0; i < text.size(); ++i) {
        char c = text[i] - 'a';
        v = go(v, c);
        int u = v;
        // go by compressed suffix links and update answer for
        // string terminating in them
        while (u > 0) {
            if (t[u].leaf) {
                for (int ind : t[u].inds) {
                    ans[ind].push_back(i + 2 - dict[ind].size());
                }
            }
            u = get_up(u);
        }
    }
    
    // print answer (1-numerated)
    for (int i = 0; i < n; ++i) {
        cout << dict[i] << ": ";
        for (int p : ans[i]) {
            cout << p << " ";
        }
        cout << endl;
    }
    return 0;
}
