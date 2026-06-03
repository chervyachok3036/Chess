#pragma once
#include <windows.h>

extern HWND rules;
extern int page;
void OpenRules();
LRESULT CALLBACK RulesProc(HWND h, UINT msg, WPARAM wp, LPARAM lp);
