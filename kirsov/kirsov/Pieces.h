#pragma once
#include <windows.h>
#include "ChessState.h"

wchar_t ToSym(Type t);
void DrawPiece(HDC h, const Piece& p, int size, int offX, int offY);
void DrawAll(HDC h, const State& s, int size);
