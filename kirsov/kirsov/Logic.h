#pragma once
#include <vector>
#include "ChessState.h"

bool PathClear(const State& s, int x1, int y1, int x2, int y2);
bool CanCap(const State& s, const Piece& a, const Piece& b);
void DoCap(State& s, int ai, int vi);
bool Solved(const State& s);
void Solve(const State& start, std::vector<Move>& out);
void GenPuzzle(State& s);
