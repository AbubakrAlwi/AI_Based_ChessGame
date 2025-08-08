#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "move.h"
#include <vector>
#include <array>

struct PieceOnSquare {
    Piece p;
    Color c;
    PieceOnSquare(): p(EMPTY), c(NO_COLOR) {}
    PieceOnSquare(Piece pp, Color cc): p(pp), c(cc) {}
};

class Board {
public:
    Board();
    void reset();
    void show() const;
    bool move_from_uci(const std::string &m);
    std::vector<Move> generate_moves(Color side) const;
    bool make_move(const Move &m);
    void unmake_move();
    int evaluate(Color side) const; // positive is good for side
    Color side_to_move;
    bool in_bounds(int r,int c) const { return r>=0 && r<8 && c>=0 && c<8; }
private:
    std::array<std::array<PieceOnSquare,8>,8> b;
    struct Undo { Move m; PieceOnSquare captured; };
    std::vector<Undo> history;

    void place_piece(int r,int c, Piece p, Color col){ b[r][c]=PieceOnSquare(p,col); }
    PieceOnSquare at(int r,int c) const { return b[r][c]; }

    void gen_pawn_moves(int r,int c, Color col, std::vector<Move>&out) const;
    void gen_knight_moves(int r,int c, Color col, std::vector<Move>&out) const;
    void gen_sliding_moves(int r,int c, Color col, const std::vector<std::pair<int,int>>&dirs, std::vector<Move>&out) const;
    void gen_king_moves(int r,int c, Color col, std::vector<Move>&out) const;
};

#endif // BOARD_H
