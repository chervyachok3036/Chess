#pragma once

enum Type
{
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT
};
struct Piece
{
    Type type;
    int x, y;
    bool alive;
};
struct Move
{
    int ax, ay;
    int vx, vy;
};
struct State
{
    Piece pcs[16];
    int count;
};
