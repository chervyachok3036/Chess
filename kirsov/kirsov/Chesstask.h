#pragma once
#include <windows.h>
#include "Logic.h"
#include "Pieces.h"
#include "Menu.h"

extern HWND game;
void OpenGame();
LRESULT CALLBACK GameProc(HWND h, UINT msg, WPARAM wp, LPARAM lp);
void ShowSolved();
void DrawBoard(HDC dc);

