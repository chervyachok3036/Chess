#include <windows.h>
#include "Menu.h"
#include "Rules.h"
#include "Chesstask.h"

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int) {
    WNDCLASSA m = {};
    m.lpfnWndProc = MenuProc;
    m.hInstance = h;
    m.lpszClassName = "MenuCls";
    m.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&m);

    WNDCLASSA r = {};
    r.lpfnWndProc = RulesProc;
    r.hInstance = h;
    r.lpszClassName = "RulesCls";
    r.hCursor = LoadCursor(nullptr, IDC_ARROW);
    r.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&r);

    WNDCLASSA g = {};
    g.lpfnWndProc = GameProc;
    g.hInstance = h;
    g.lpszClassName = "GameCls";
    g.hCursor = LoadCursor(nullptr, IDC_ARROW);
    g.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&g);

    OpenMenu();
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
