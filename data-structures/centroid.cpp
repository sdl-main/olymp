#include <bits/stdc++.h>

using namespace std;

const int N = 100001;

vector<int> g[N];
bool used[N];
int sz[N], ans[N], oya[N];
int num = 0; // size of current tree

// dfs to count subtree sizes
void dfsz(int v, int p = -1) {
    sz[v] = 1;
    for (int to : g[v]) {
        if (!used[to] && to != p) {
            dfsz(to, v);
            sz[v] += sz[to];
        }
    }
    if (p == -1) {
        num = sz[v];
    }
}

// dfs to find centroid
int get(int v, int p = -1) {
    for (int to : g[v]) {
        if (!used[to] && to != p && sz[to] * 2 > num) {
            return get(to, v);
        }
    }
    return v;
}

// get centroid in a tree rooted in v
int get_centroid(int v) {
    // count sz[] for the current tree
    dfsz(v);
    
    // get centroid
    int cent = get(v);
    used[cent] = 1;
    
    // find centroids of children's trees
    for (int to : g[cent]) {
        if (!used[to]) {
            int ch = get_centroid(to);
            // connect them to our centroid
            oya[ch] = cent;
        }
    }
    return cent;
}

int main() {
    
    // read graph
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n-1; ++i) {
        int a, b;
        cin >> a >> b;
        --a, --b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    
    get_centroid(0);
    return 0;
}
