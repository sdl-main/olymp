#include <bits/stdc++.h>

#define ll long long
#define ld long double
#define INF 2000000000
#define pb push_back
#define pii pair<int,int>

using namespace std;

const int N = 10;

char d[N][N];

struct st {
    char x1, y1, x2, y2;
    bool hod;
};

vector<st> g[N][N][N][N][2];
bool win[N][N][N][N][2];
bool lose[N][N][N][N][2];
char deg[N][N][N][N][2];
bool used[N][N][N][N][2];

bool check(char x1, char y1, char x2, char y2) {
    return x1 >= 1 && x1 <= 8 && x2 >= 1 && x2 <= 9 && y1 >= 1 && y1 <= 8 && y2 >= 1 && y2 <= 8 &&
        d[x1][y1] != 1 && d[x2][y2] != 1;
}

void dfs(char x1, char y1, char x2, char y2, char hod) {
    used[x1][y1][x2][y2][hod] = 1;
    for (auto &s : g[x1][y1][x2][y2][hod]) {
        char xx1 = s.x1, xx2 = s.x2, yy1 = s.y1, yy2 = s.y2, hodd = s.hod;
        if (used[xx1][yy1][xx2][yy2][hodd]) continue;
        if (lose[x1][y1][x2][y2][hod]) {
            win[xx1][yy1][xx2][yy2][hodd] = 1;
        }
        else if (--deg[xx1][yy1][xx2][yy2][hodd] == 0) {
            lose[xx1][yy1][xx2][yy2][hodd] = 1;
        }
        else continue;
        dfs(xx1, yy1, xx2, yy2, hodd);
    }
}

bool shoot(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 > y2) swap(y1, y2);
        for (int y = y1; y <= y2; ++y) {
            if (d[x1][y] == 1) return 0;
        }
        return 1;
    }
    if (y1 == y2) {
        if (x1 > x2) swap(x1, x2);
        for (int x = x1; x <= x2; ++x) {
            if (d[x][y1] == 1) return 0;
        }
        return 1;
    }
    if (abs(x2-x1) == abs(y2-y1)) {
        if (x1 > x2) swap(x1, x2), swap(y1, y2);
        if (y2 > y1) {
            for (int x = x1, y = y1; x <= x2; ++x, ++y) {
                if (d[x][y] == 1) return 0;
            }
            return 1;
        }
        else {
            for (int x = x1, y = y1; x <= x2; ++x, --y) {
                if (d[x][y] == 1) return 0;
            }
            return 1;
        }
    }
    return 0;
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    char t_x, t_y, b_x, b_y;
    for (int i = 1; i <= 8; ++i) {
        string s;
        cin >> s;
        for (int j = 1; j <= 8; ++j) {
            d[i][j] = s[j-1] - '0';
            if (d[i][j] == 2) {
                b_x = i, b_y = j;
            }
            if (d[i][j] == 3) {
                t_x = i, t_y = j;
            }
        }
    }
    for (char x1 = 1; x1 <= 8; ++x1) {
        for (char y1 = 1; y1 <= 8; ++y1) {
            for (char x2 = 1; x2 <= 9; ++x2) {
                for (char y2 = 1; y2 <= 8; ++y2) {
                    for (char hod = 0; hod <= 1; ++hod) {
                        if (!check(x1, y1, x2, y2)) continue;
                        bool wi = 0, lo = 0;
                        if (hod == 0) {
                            lo = x2 == 9;
                            wi = !lo && shoot(x1,y1,x2,y2);
                        }
                        else {
                            wi = x2 == 9;
                            lo = !wi && shoot(x1,y1,x2,y2);
                        }
                        win[x1][y1][x2][y2][hod] = wi;
                        lose[x1][y1][x2][y2][hod] = lo;
                        if (wi || lo) {
                            continue;
                        }
                        st s = {x1, y1, x2, y2, hod};
                        vector<char> dx = {-1, -1, -1, 0, 0, 1, 1, 1};
                        vector<char> dy = {-1,  0,  1, -1,1,-1, 0, 1};
                        for (int i = 0; i < 8; ++i) {
                            int xx1 = x1, xx2 = x2, yy1 = y1, yy2 = y2;
                            if (hod == 0) {
                                xx1 += dx[i];
                                yy1 += dy[i];
                            }
                            else {
                                xx2 += dx[i];
                                yy2 += dy[i];
                            }
                            if (check(xx1, yy1, xx2, yy2)) {
                                g[xx1][yy1][xx2][yy2][!hod].pb(s);
                                ++deg[x1][y1][x2][y2][hod];
                            }
                        }
                    }
                }
            }
        }
    }
    for (char x1 = 1; x1 <= 8; ++x1) {
        for (char y1 = 1; y1 <= 8; ++y1) {
            for (char x2 = 1; x2 <= 9; ++x2) {
                for (char y2 = 1; y2 <= 8; ++y2) {
                    for (char hod = 0; hod <= 1; ++hod) {
                        if (!used[x1][y1][x2][y2][hod] && (win[x1][y1][x2][y2][hod] ||
                                                          lose[x1][y1][x2][y2][hod])) {
                            dfs(x1, y1, x2, y2, hod);
                        }
                    }
                }
            }
        }
    }
    if (win[t_x][t_y][b_x][b_y][1]) {
        cout << 1;
    }
    else {
        cout << -1;
    }
    return 0;
}
