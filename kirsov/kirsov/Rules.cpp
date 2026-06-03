#include "Rules.h"
#include "Menu.h"
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;

HWND rules = nullptr;
int page = 0;
ULONG_PTR gdi;
const char* txt[] = {
    "Король: ходит на одну клетку в любом направлении.",
    "Ферзь: ходит по вертикали, горизонтали и диагонали.",
    "Ладья: ходит по вертикали и горизонтали.",
    "Слон: ходит по диагонали.",
    "Конь: ходит буквой Г."
};
Bitmap* img[5];
void LoadImg() {
    img[0] = new Bitmap(L"../../kirsov/res/Image/KING.jpg");
    img[1] = new Bitmap(L"../../kirsov/res/Image/QUEEN.jpg");
    img[2] = new Bitmap(L"../../kirsov/res/Image/ROOK.jpg");
    img[3] = new Bitmap(L"../../kirsov/res/Image/BISHOP.jpg");
    img[4] = new Bitmap(L"../../kirsov/res/Image/KNIGHT.jpg");
}
void DrawImg(HDC h) {
    Graphics g(h);
    Bitmap* b = img[page];
    if (!b) return;
    g.DrawImage(b, 40, 90, 300, 300);
}
void OpenRules() {
    page = 0;
    GdiplusStartupInput in;
    GdiplusStartup(&gdi, &in, nullptr);
    rules = CreateWindowA("RulesCls", "Правила", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(rules, SW_SHOW);
}
LRESULT CALLBACK RulesProc(HWND h, UINT m, WPARAM wp, LPARAM lp) {
    switch (m) {
    case WM_CREATE:
        LoadImg();
        CreateWindowA("BUTTON", "Назад", WS_CHILD | WS_VISIBLE,
            50, 400, 150, 40, h, (HMENU)1, nullptr, nullptr);
        CreateWindowA("BUTTON", "Далее", WS_CHILD | WS_VISIBLE,
            250, 400, 150, 40, h, (HMENU)2, nullptr, nullptr);
        CreateWindowA("BUTTON", "В меню", WS_CHILD | WS_VISIBLE,
            450, 10, 100, 30, h, (HMENU)3, nullptr, nullptr);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        RECT r{ 230,50,550,350 };
        DrawTextA(dc, txt[page], -1, &r, DT_LEFT | DT_TOP | DT_WORDBREAK);
        DrawImg(dc);
        EndPaint(h, &ps);
    } break;
    case WM_COMMAND:
        if (LOWORD(wp) == 1 && page > 0) page--;
        if (LOWORD(wp) == 2 && page < 4) page++;
        if (LOWORD(wp) == 3) {
            ShowWindow(h, SW_HIDE);
            ShowWindow(menu, SW_SHOW);
        }
        InvalidateRect(h, nullptr, TRUE);
        break;
    case WM_CLOSE:
        GdiplusShutdown(gdi);
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        GdiplusShutdown(gdi);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcA(h, m, wp, lp);
}
