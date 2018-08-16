#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int32_t main() {
    const int N = 9;
    int a[N] = {1, 4, 2, 5, 6, 8, 4, 2, 6};
    int b[N];
    memcpy(b, a, N * sizeof(int));
    sort(b, b + N); // b = {1, 2, 2, 4, 4, 5, 6, 6, 8}
    int sz = unique(b, b + N) - b; // b = {1, 2, 4, 5, 6, 8, ?, ?, ?}, sz = 6
    for (int i = 0; i < N; ++i) {
        // find index of the highest b[x] that is less or equal than a[i]
        a[i] = lower_bound(b, b + sz, a[i]) - b;
    }
    // a = {0, 2, 1, 3, 4, 5, 2, 1, 4}
    return 0;
}
