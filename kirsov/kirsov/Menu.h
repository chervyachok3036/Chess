#pragma once
#include <windows.h>

extern HWND menu;
void OpenMenu();
LRESULT CALLBACK MenuProc(HWND h, UINT msg, WPARAM wp, LPARAM lp);
