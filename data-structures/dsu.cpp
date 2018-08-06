#include <iostream>

using namespace std;

const int N = 100000;

int parent[N]; // array of parent pointers
int sz[N]; // sizes of sets

// finds leader of set that contains v
int find_leader(int v) {
    return parent[v] == v ? v : parent[v] = find_leader(parent[v]);
}

// combines two sets which contain a and b
void union_sets(int a, int b) {
    a = find_leader(a);
    b = find_leader(b);

    if (a != b) {
        // size heuristic
        if (sz[a] > sz[b])
            swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];

        // random heuristic
        // if (rand() & 1)
        //     swap(a, b);
        // parent[a] = b;
    }
}

// adds v to a new set
void make_set(int v) {
    parent[v] = v;
    sz[v] = 1;
}

