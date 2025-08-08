#ifndef AI_H
#define AI_H

#include "board.h"
#include <optional>

class AI {
public:
    AI(int maxd=4): max_depth(maxd) {}
    Move find_best(Board &board, Color for_side);
    int nodes_searched = 0;
private:
    int max_depth;
    int alphabeta(Board &board, int depth, int alpha, int beta, Color side);
};

#endif // AI_H


/* ========== FILE: ai.cpp ========== */

#include "ai.h"
#include <limits>

Move AI::find_best(Board &board, Color for_side){
    nodes_searched = 0;
    int best = std::numeric_limits<int>::min();
    Move best_move;
    auto moves = board.generate_moves(for_side);
    for(auto &m : moves){
        board.make_move(m);
        int val = -alphabeta(board, max_depth-1, std::numeric_limits<int>::min()+1, std::numeric_limits<int>::max()-1, (Color)(-for_side));
        board.unmake_move();
        if(val>best){ best = val; best_move = m; }
    }
    return best_move;
}

int AI::alphabeta(Board &board, int depth, int alpha, int beta, Color side){
    nodes_searched++;
    if(depth==0){
        return board.evaluate(side);
    }
    auto moves = board.generate_moves(side);
    if(moves.empty()){
        // checkmate or stalemate approximation
        return board.evaluate(side);
    }
    for(auto &m: moves){
        board.make_move(m);
        int val = -alphabeta(board, depth-1, -beta, -alpha, (Color)(-side));
        board.unmake_move();
        if(val>=beta) return val;
        if(val>alpha) alpha=val;
    }
    return alpha;
}
