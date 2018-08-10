#include <iostream>
#include <vector>

using namespace std;

struct node {
    int x;   // size of subtree
    int y;   // priority
    int v;   // value
    int sum; // sum of tree
    int add; // addition on the tree
    node *l; // left child
    node *r; // right child
    node(){}
    node(int v) : v(v), x(1), y(rand()), l(NULL),
                  r(NULL), sum(v), add(0) {}
};

node *root = NULL;

int get_x(node *a) {
    return a ? a->x : 0;
}

int get_sum(node *a) {
    return a ? a->sum + a->x * a->add : 0;
}

// add value to a subtree
void padd(node *a, int add) {
    if (!a)
        return;
    a->add += add;
}

// push addition to children
void push(node *a) {
    if (!a)
        return;
    a->v += a->add;
    padd(a->l, a->add);
    padd(a->r, a->add);
    a->add = 0;
}

// update size and sum of a node
void update(node *a) {
    if (!a)
        return;
    a->x = 1 + get_x(a->l) + get_x(a->r);
    a->sum = a->v + get_sum(a->l) + get_sum(a->r);
}

// merges two treaps and returns pointer to new root
node *merge(node *a, node *b) {
    if (!a || !b)
        return a ? a : b;
    else if (a->y <= b->y) {
        push(b);
        b->l = merge(a, b->l);
        update(b);
        return b;
    }
    else {
        push(a);
        a->r = merge(a->r, b);
        update(a);
        return a;
    }
}

// splits treap T to treaps A and B so that A has K elements
void split(node *t, int k, node *&a, node *&b) {
    if (!t)
        a = b = NULL;
    else {
        push(t);
        int l = get_x(t->l);
        if (l < k) {
            split(t->r, k-l-1, t->r, b);
            a = t;
        }
        else {
            split(t->l, k, a, t->l);
            b = t;
        }
        update(a);
        update(b);
    }
}

// insert new value
void insert(int pos, int val) {
    node *t1, *t2;
    split(root, pos, t1, t2);
    root = merge(t1, merge(new node(val), t2));
}

// erase value at the position
void erase(int pos) {
    node *t1, *t2, *t3;
    split(root, pos, t1, t2);
    split(t2, 1, t2, t3);
    root = merge(t1, t3);
}

// get sum of a segment
int get_sum(int l, int r) {
    node *t1, *t2, *t3;
    split(root, l, t1, t2);
    split(t2, r-l+1, t2, t3);
    int ans = get_sum(t2);
    root = merge(t1, merge(t2, t3));
    return ans;
}

// add value to a segment
void add(int l, int r, int add) {
    node *t1, *t2, *t3;
    split(root, l, t1, t2);
    split(t2, r-l+1, t2, t3);
    padd(t2, add);
    push(t2);
    root = merge(t1, merge(t2, t3));
}

// build a treap based on array
void build(vector<int> &v) {
    for (int i = 0; i < v.size(); ++i) {
        insert(i, v[i]);
    }
}

// print the treap
void dfs(node *a) {
    if (!a)
        return;
    push(a);
    dfs(a->l);
    cout << a->v << " ";
    dfs(a->r);
}

int main() {
    vector<int> v = {1, 3, 2, 5, 6, 4};
    build(v);
    erase(1);
    add(0, 3, 2);
    dfs(root);
    // output: 3 4 7 8 4
    return 0;
}
