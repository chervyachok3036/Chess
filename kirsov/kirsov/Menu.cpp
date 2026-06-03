#include "Menu.h"
#include "Rules.h"
#include "Chesstask.h"

HWND menu = nullptr;
void OpenMenu() {
    menu = CreateWindowA("MenuCls", "Меню", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(menu, SW_SHOW);
}
LRESULT CALLBACK MenuProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        CreateWindowA("STATIC",
            "Добро пожаловать!\nВ этих задачах вы должны съесть все фигуры,\nчтобы осталась только одна.",
            WS_CHILD | WS_VISIBLE, 50, 30, 500, 80,
            h, nullptr, nullptr, nullptr);
        CreateWindowA("BUTTON", "Начать",
            WS_CHILD | WS_VISIBLE, 200, 150, 200, 40,
            h, (HMENU)1, nullptr, nullptr);
        CreateWindowA("BUTTON", "Правила",
            WS_CHILD | WS_VISIBLE, 200, 200, 200, 40,
            h, (HMENU)2, nullptr, nullptr);
        break;
    case WM_COMMAND:
        if (LOWORD(wp) == 1) {
            ShowWindow(h, SW_HIDE);
            OpenGame();
        }
        else if (LOWORD(wp) == 2) {
            ShowWindow(h, SW_HIDE);
            OpenRules();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcA(h, msg, wp, lp);
}
