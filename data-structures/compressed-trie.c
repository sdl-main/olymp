#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    struct node *link;
    struct node *next;
    char *s;
    int n;
} node;

node *make_node(char *x, int n) {
    node *a = (node*) malloc(sizeof(node));
    a->link = a->next = NULL;
    a->s = x;
    a->n = n;
    return a;
}

void split(node *a, int k) {
    node *p = make_node(a->s + k, a->n - k);
    p->link = a->link;
    a->link = p;
    char *buf = (char*) malloc(k);
    strncpy(buf, a->s, k);
    //free(t->s);
    a->s = buf;
    a->n = k;
}

int lcp(char *a, int n, char *b, int m) {
    for (int i = 0; i < n; ++i) {
        if (i == m || a[i] != b[i])
            return i;
    }
    return n;
}

node *insert(node *t, char *x, int n) {
    if (!t)
        return make_node(x, n);
    int k = lcp(x, n, t->s, t->n);
    if (!k)
        t->next = insert(t->next, x, n);
    else {
        if (k < t->n)
            split(t, k);
        t->link = insert(t->link, x + k, n - k);
    }
    return t;
}

node *find(node *t, char *x, int n) {
    if (!t)
        return NULL;
    int k = lcp(x, n, t->s, t->n);
    if (!k) return find(t->next, x, n);
    if (k == n) return t;
    if (k == t->n) return find(t->link, x + k, n - k);
    return NULL;
}

void print(node *t) {
    if (!t) return;
    //printf("%d %s %d %d %d %lld\n", (int)t, t->s, t->n, (int)(t->next), (int)(t->link), t->ip);
    print(t->next);
    print(t->link);
}

void free_tree(node *t) {
    if (!t) return;
    free_tree(t->next);
    free_tree(t->link);
    free(t);
}

int get(node *t) {
    if (!t) return 0;
    int ans = 0;
    ans += t->n;
    ans += get(t->next);
    ans += get(t->link);
    return ans;
}

int main(void) {
    char buf[2002];
    scanf("%s", buf);
    int n = strlen(buf);
    node *root = NULL;
    for (int i = 0; i < n; ++i) {
        root = insert(root, buf + i, n - i);
    }
    printf("%d", get(root) + 1);
    return 0;
}
