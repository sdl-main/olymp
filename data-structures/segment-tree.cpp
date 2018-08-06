#include <iostream>

using namespace std;

const int N = 100000;
const int NEUTRAL = 0; //neutral element

int t[4*N]; // our tree
int a[N]; // our array

// return a (*) b, where (*) is an operation
int merge(int a, int b) {
    return a + b;
}

// build a segment tree with root v on a[tl]...a[tr] segment
void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl];
    }
    else {
        int tm = (tl + tr) >> 1;
        // build left son
        build(v*2, tl, tm);
        // build right son
        build(v*2+1, tm+1, tr);
        // merge them
        t[v] = merge(t[v*2], t[v*2+1]);
    }
}

// get result of the operation on a[L]...a[R] segment
int get(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return NEUTRAL;
    }
    if (tl == l && tr == r) {
        return t[v];
    }
    int tm = (tl + tr) >> 1;
    return merge(get(v*2, tl, tm, l, min(tm, r)), get(v*2+1, tm+1, tr, max(l, tm+1), r));
}

// modify a[pos] to val and update tree
void modify(int v, int tl, int tr, int pos, int val) {
    if (tl == tr) {
        t[v] = val;
        a[pos] = val;
    }
    else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm) {
            modify(v*2, tl, tm, pos, val);
        }
        else {
            modify(v*2+1, tm+1, tr, pos, val);
        }
        t[v] = merge(t[v*2], t[v*2+1]);
    }
}

int main() {
    // read an array
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // build a segment tree
    build(1, 0, n-1);
    
    // queries
    // 1 L R - get sum of [L...R]
    // 2 X V - modify a[X] = V
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            cin >> l >> r;
            cout << get(1, 0, n-1, l-1, r-1) << '\n';
        }
        else {
            int x, v;
            cin >> x >> v;
            modify(1, 0, n-1, x-1, v);
        }
    }
    return 0;
}
