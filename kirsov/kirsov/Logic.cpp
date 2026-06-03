#include "Logic.h"
#include <random>

bool CanCap(const State& s, const Piece& a, const Piece& b) {
    if (!a.alive || !b.alive) return false;
    if (&a == &b) return false;
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int adx = dx > 0 ? dx : -dx;
    int ady = dy > 0 ? dy : -dy;
    switch (a.type) {
    case KING: return adx <= 1 && ady <= 1 && (adx + ady > 0);
    case KNIGHT: return (adx == 1 && ady == 2) || (adx == 2 && ady == 1);
    case ROOK: return (dx == 0 || dy == 0) && PathClear(s, a.x, a.y, b.x, b.y);
    case BISHOP: return (adx == ady) && PathClear(s, a.x, a.y, b.x, b.y);
    case QUEEN: return (dx == 0 || dy == 0 || adx == ady) && PathClear(s, a.x, a.y, b.x, b.y);
    }
    return false;
}
bool PathClear(const State& s, int x1, int y1, int x2, int y2) {
    int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
    int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);
    int cx = x1 + dx;
    int cy = y1 + dy;
    while (cx != x2 || cy != y2) {
        for (int i = 0; i < s.count; i++)
            if (s.pcs[i].alive && s.pcs[i].x == cx && s.pcs[i].y == cy)
                return false;
        cx += dx;
        cy += dy;
    }
    return true;
}
void DoCap(State& s, int ai, int vi) {
    Piece& a = s.pcs[ai];
    Piece& v = s.pcs[vi];
    a.x = v.x;
    a.y = v.y;
    v.alive = false;
}
bool Solved(const State& s) {
    int alive = 0;
    for (int i = 0; i < s.count; i++)
        if (s.pcs[i].alive) alive++;
    return alive == 1;
}
static bool DFS(State& s, std::vector<Move>& path, int d, int md) {
    if (Solved(s)) return true;
    if (d >= md) return false;
    for (int i = 0; i < s.count; i++) {
        if (!s.pcs[i].alive) continue;
        for (int j = 0; j < s.count; j++) {
            if (i == j || !s.pcs[j].alive) continue;
            if (CanCap(s, s.pcs[i], s.pcs[j])) {
                State bak = s;
                Move m;
                m.ax = s.pcs[i].x;
                m.ay = s.pcs[i].y;
                m.vx = s.pcs[j].x;
                m.vy = s.pcs[j].y;
                DoCap(s, i, j);
                path.push_back(m);
                if (DFS(s, path, d + 1, md)) return true;
                path.pop_back();
                s = bak;
            }
        }
    }
    return false;
}
void Solve(const State& start, std::vector<Move>& out) {
    out.clear();
    State s = start;
    int md = 7;
    DFS(s, out, 0, md);
}
void GenPuzzle(State& s) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> cnt(3, 6);
    std::uniform_int_distribution<int> tp(0, 4);
    std::uniform_int_distribution<int> cd(0, 7);
    while (true) {
        s.count = cnt(rng);
        for (int i = 0; i < s.count; i++) {
            s.pcs[i].type = static_cast<Type>(tp(rng));
            s.pcs[i].alive = true;
            while (true) {
                int x = cd(rng);
                int y = cd(rng);
                bool occ = false;
                for (int j = 0; j < i; j++)
                    if (s.pcs[j].alive && s.pcs[j].x == x && s.pcs[j].y == y) {
                        occ = true;
                        break;
                    }
                if (!occ) {
                    s.pcs[i].x = x;
                    s.pcs[i].y = y;
                    break;
                }
            }
        }
        std::vector<Move> sol;
        Solve(s, sol);
        if (!sol.empty()) return;
    }
}
