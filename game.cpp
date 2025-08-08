#include "game.h"
#include <iostream>

Game::Game(): ai(4), human_side(WHITE) {}

void Game::loop(){
    std::cout << "Welcome to AI Chess (minimal). Moves: e2e4, type 'quit' to exit.\n";
    board.reset();
    board.show();
    while(true){
        if(board.side_to_move == human_side){
            std::string s; std::cout<<"Your move: "; std::cin>>s;
            if(!std::cin) break;
            if(s=="quit"||s=="exit") break;
            if(!board.move_from_uci(s)){
                std::cout<<"Illegal move or bad format. Use e2e4.\n";
                continue;
            }
            board.show();
        } else {
            std::cout<<"AI thinking...\n";
            Move m = ai.find_best(board, board.side_to_move);
            board.make_move(m);
            std::cout<<"AI plays: "<< sqname(m.from_r,m.from_c)<<sqname(m.to_r,m.to_c)<<"\n";
            board.show();
        }
    }
    std::cout<<"Goodbye!\n";
}
