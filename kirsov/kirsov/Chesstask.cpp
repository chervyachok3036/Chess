#include "Chesstask.h"
#include "Logic.h"
#include "Pieces.h"
#include "Menu.h"
#include <vector>

HWND game = nullptr;
const int CSZ = 60;
const int BSZ = 8;
const int OX = 20;
const int OY = 20;
State init;
State cur;
std::vector<Move> sol;
int solI = 0;
std::vector<State> hist;
std::vector<int> histI;
bool drag = false;
int dragP = -1;
int offX = 0;
int offY = 0;
int mx = 0;
int my = 0;
HWND btnNext = nullptr;
HWND btnUndo = nullptr;
HWND btnReset = nullptr;
HWND btnNew = nullptr;
void Push() {
    hist.push_back(cur);
    histI.push_back(solI);
}
void Undo(HWND h) {
    if (hist.empty()) return;
    cur = hist.back();
    hist.pop_back();
    solI = histI.back();
    histI.pop_back();
    RECT r{ OX,OY,OX + CSZ * 8,OY + CSZ * 8 };
    InvalidateRect(h, &r, FALSE);
}
void DoMove(int a, int b) {
    Piece& p = cur.pcs[a];
    Piece& v = cur.pcs[b];
    p.x = v.x;
    p.y = v.y;
    v.alive = false;
}
bool One() {
    int c = 0;
    for (int i = 0; i < cur.count; i++)
        if (cur.pcs[i].alive) c++;
    return c == 1;
}
void Redraw(HWND h) {
    RECT r{ OX,OY,OX + CSZ * 8,OY + CSZ * 8 };
    InvalidateRect(h, &r, FALSE);
}
void DoAndCheck(HWND h, int a, int b) {
    Push();
    DoMove(a, b);
    if (One()) ShowSolved();
    Redraw(h);
}
int Find(const State& s, int x, int y) {
    for (int i = 0; i < s.count; i++)
        if (s.pcs[i].alive && s.pcs[i].x == x && s.pcs[i].y == y)
            return i;
    return -1;
}
void ApplyPref(int n) {
    cur = init;
    for (int i = 0; i < n && i < (int)sol.size(); i++) {
        const Move& m = sol[i];
        int a = Find(cur, m.ax, m.ay);
        int b = Find(cur, m.vx, m.vy);
        if (a == -1 || b == -1) continue;
        DoMove(a, b);
    }
}
HWND Btn(HWND p, int id, const char* t, int x, int y) {
    return CreateWindowA("BUTTON", t, WS_CHILD | WS_VISIBLE,
        x, y, 260, 35, p, (HMENU)id, nullptr, nullptr);
}
void MakeBtns(HWND h) {
    btnNext = Btn(h, 6, "Показать следующий ход", 520, 40);
    btnUndo = Btn(h, 4, "Отмена хода", 520, 85);
    btnReset = Btn(h, 5, "Начальная позиция", 520, 130);
    btnNew = Btn(h, 3, "Новая задача", 520, 175);
    Btn(h, 7, "В меню", 520, 220);
}
void PaintDB(HWND h, HDC dc) {
    HDC mem = CreateCompatibleDC(dc);
    HBITMAP bm = CreateCompatibleBitmap(dc, 900, 700);
    HBITMAP old = (HBITMAP)SelectObject(mem, bm);
    HBRUSH bg = CreateSolidBrush(RGB(255, 255, 255));
    RECT full{ 0,0,900,700 };
    FillRect(mem, &full, bg);
    DeleteObject(bg);
    DrawBoard(mem);
    BitBlt(dc, 0, 0, 900, 700, mem, 0, 0, SRCCOPY);
    SelectObject(mem, old);
    DeleteObject(bm);
    DeleteDC(mem);
}
void Reset() {
    drag = false;
    dragP = -1;
    solI = 0;
    hist.clear();
    histI.clear();
}
void ResetInit(HWND h) {
    Reset();
    cur = init;
    Redraw(h);
}
void New(HWND h) {
    Reset();
    GenPuzzle(init);
    cur = init;
    Solve(init, sol);
    Redraw(h);
}
LRESULT CALLBACK SolvedProc(HWND h, UINT m, WPARAM wp, LPARAM lp) {
    switch (m) {
    case WM_CREATE:
        Btn(h, 1, "Следующая задача", 40, 80);
        Btn(h, 2, "Остаться здесь", 40, 125);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(30, 30, 30));
        RECT r{ 0,10,340,70 };
        DrawTextA(dc, "Вы решили задачу!", -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(h, &ps);
    } break;
    case WM_COMMAND:
        if (LOWORD(wp) == 1) {
            New(game);
            DestroyWindow(h);
        }
        else if (LOWORD(wp) == 2) {
            DestroyWindow(h);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(h);
        break;
    }
    return DefWindowProcA(h, m, wp, lp);
}
void ShowSolved() {
    WNDCLASSA c = {};
    c.lpfnWndProc = SolvedProc;
    c.hInstance = GetModuleHandle(nullptr);
    c.lpszClassName = "SolvedCls";
    RegisterClassA(&c);
    HWND w = CreateWindowA("SolvedCls", "Готово!",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        300, 200, 350, 250, nullptr, nullptr, nullptr, nullptr);
    ShowWindow(w, SW_SHOW);
}
void DrawBoard(HDC dc) {
    for (int y = 0; y < BSZ; y++)
        for (int x = 0; x < BSZ; x++) {
            int cx = OX + x * CSZ;
            int cy = OY + y * CSZ;
            RECT r{ cx,cy,cx + CSZ,cy + CSZ };
            bool d = (x + y) % 2;
            COLORREF l = RGB(245, 235, 220);
            COLORREF dk = RGB(210, 180, 140);
            HBRUSH b = CreateSolidBrush(d ? dk : l);
            FillRect(dc, &r, b);
            DeleteObject(b);
        }
    for (int i = 0; i < cur.count; i++) {
        if (!cur.pcs[i].alive) continue;
        if (drag && i == dragP) continue;
        DrawPiece(dc, cur.pcs[i], CSZ, OX, OY);
    }
    if (drag && dragP != -1) {
        Piece p = cur.pcs[dragP];
        RECT r;
        r.left = mx - offX;
        r.top = my - offY;
        r.right = r.left + CSZ;
        r.bottom = r.top + CSZ;
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(0, 0, 0));
        HFONT f = CreateFontW(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Symbol");
        HFONT old = (HFONT)SelectObject(dc, f);
        wchar_t s[2] = { ToSym(p.type),0 };
        DrawTextW(dc, s, 1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, old);
        DeleteObject(f);
    }
}
int Hit(int x, int y) {
    x -= OX;
    y -= OY;
    if (x < 0 || y < 0) return -1;
    int bx = x / CSZ;
    int by = y / CSZ;
    if (bx < 0 || bx >= 8 || by < 0 || by >= 8) return -1;
    for (int i = 0; i < cur.count; i++)
        if (cur.pcs[i].alive && cur.pcs[i].x == bx && cur.pcs[i].y == by)
            return i;
    return -1;
}
LRESULT CALLBACK GameProc(HWND h, UINT m, WPARAM wp, LPARAM lp) {
    switch (m) {
    case WM_CREATE:
        MakeBtns(h);
        break;
    case WM_LBUTTONDOWN: {
        int x = LOWORD(lp);
        int y = HIWORD(lp);
        int i = Hit(x, y);
        if (i != -1) {
            drag = true;
            dragP = i;
            offX = x - (OX + cur.pcs[i].x * CSZ);
            offY = y - (OY + cur.pcs[i].y * CSZ);
            mx = x;
            my = y;
            SetCapture(h);
        }
    } break;
    case WM_MOUSEMOVE:
        if (drag) {
            mx = LOWORD(lp);
            my = HIWORD(lp);
            Redraw(h);
        }
        break;
    case WM_LBUTTONUP: {
        if (!drag) break;
        ReleaseCapture();
        int x = LOWORD(lp);
        int y = HIWORD(lp);
        int t = Hit(x, y);
        if (t != -1 && t != dragP) {
            Piece a = cur.pcs[dragP];
            Piece b = cur.pcs[t];
            if (CanCap(cur, a, b)) DoAndCheck(h, dragP, t);
        }
        drag = false;
        dragP = -1;
        Redraw(h);
    } break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        PaintDB(h, dc);
        EndPaint(h, &ps);
    } break;
    case WM_COMMAND: {
        int id = LOWORD(wp);
        if (id == 6) {
            if (solI < (int)sol.size()) {
                Push();
                solI++;
                ApplyPref(solI);
                if (One()) ShowSolved();
                Redraw(h);
            }
        }
        else if (id == 4) Undo(h);
        else if (id == 5) ResetInit(h);
        else if (id == 3) New(h);
        else if (id == 7) {
            ShowWindow(h, SW_HIDE);
            ShowWindow(menu, SW_SHOW);
        }
    } break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(h, m, wp, lp);
    }
    return 0;
}
void OpenGame() {
    game = CreateWindowA("GameCls", "Шахматная задача",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 700, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(game, SW_SHOW);
    New(game);
}
