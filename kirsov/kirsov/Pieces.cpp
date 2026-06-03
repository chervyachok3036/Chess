#include "Pieces.h"
#include <windows.h>

wchar_t ToSym(Type t) {
    switch (t) {
    case KING:   return L'♔';
    case QUEEN:  return L'♕';
    case ROOK:   return L'♖';
    case BISHOP: return L'♗';
    case KNIGHT: return L'♘';
    }
    return L'?';
}
void DrawPiece(HDC h, const Piece& p, int size, int offX, int offY) {
    if (!p.alive) return;
    wchar_t s = ToSym(p.type);
    int px = offX + p.x * size;
    int py = offY + p.y * size;
    RECT r{ px, py, px + size, py + size };
    SetBkMode(h, TRANSPARENT);
    SetTextColor(h, RGB(0, 0, 0));
    int f = size - 10;
    HFONT font = CreateFontW(f, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Symbol");
    HFONT old = (HFONT)SelectObject(h, font);
    DrawTextW(h, &s, 1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(h, old);
    DeleteObject(font);
}
void DrawAll(HDC h, const State& s, int size) {
    for (int i = 0; i < s.count; i++) {
        const Piece& p = s.pcs[i];
        if (p.alive) DrawPiece(h, p, size, 0, 0);
    }
}

