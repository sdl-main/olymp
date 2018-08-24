#include <iostream>

using namespace std;

const int N = 100001;

int a[N];
int n; // size of a[]
int bit[N]; // binary indexed tree over a[]

// get sum of a[1...x] in O(log(N))
int sum(int x) {
    int ans = 0;
    while (x > 0) {
        ans += bit[x];
        x -= x & -x;
    }
    return ans;
}

// get a[x] in O(log(N))
int get(int x) {
    int ans = bit[x];
    if (x > 0) {
        int z = x - (x & -x);
        --x;
        while (x != z) {
            ans -= bit[x];
            x -= x & -x;
        }
    }
    return ans;
}

// get sum a[l..r] in O(2*logN)
int get(int l, int r) {
    return sum(r) - sum(l-1);
}

// add v to a[x] and update tree in O(logN)
void add(int x, int v) {
    while (x <= n) {
        bit[x] += v;
        x += x & -x;
    }
}

// set a[x] to v in O(logN)
void modify(int x, int v) {
    int delta = v - get(x);
    add(x, delta);
}

// build tree in O(NlogN)
void build() {
    for (int i = 1; i <= n; ++i) {
        modify(i, a[i]);
    }
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    build();

    // queries: 1 L R - get sum a[L..R]
    //          2 X V - set a[X] to V
    //          3 X   - get a[X]
    
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int t;
        cin >> t;
        if (t == 1) {
            int l, r;
            cin >> l >> r;
            cout << get(l, r) << '\n';
        }
        else if (t == 2) {
            int x, v;
            cin >> x >> v;
            modify(x, v);
        }
        else {
            int x;
            cin >> x;
            cout << get(x) << '\n';
        }
    }
    return 0;
}
