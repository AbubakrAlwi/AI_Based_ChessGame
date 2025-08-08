#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

enum Color : int8_t { WHITE = 1, BLACK = -1, NO_COLOR = 0 };

enum Piece : int8_t {
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
};

struct Square {
    int r; // 0..7 (rank 8..1 mapped appropriately)
    int c; // 0..7 (file a..h)
    Square(): r(-1), c(-1) {}
    Square(int rr, int cc): r(rr), c(cc) {}
};

inline bool inside(int r, int c) { return r>=0 && r<8 && c>=0 && c<8; }

#endif // TYPES_H


/* ========== FILE: move.h ========== */

#ifndef MOVE_H
#define MOVE_H

#include "types.h"
#include <string>

struct Move {
    int from_r, from_c;
    int to_r, to_c;
    Piece promotion; // QUEEN if promotion, else EMPTY
    Move(): from_r(-1),from_c(-1),to_r(-1),to_c(-1),promotion(EMPTY) {}
    Move(int fr,int fc,int tr,int tc,Piece p=EMPTY): from_r(fr),from_c(fc),to_r(tr),to_c(tc),promotion(p) {}
};

inline std::string sqname(int r,int c){
    char f = 'a' + c;
    char rank = '1' + (7 - r); // r=7 -> rank 1
    std::string s;
    s.push_back(f);
    s.push_back(rank);
    return s;
}

#endif // MOVE_H
