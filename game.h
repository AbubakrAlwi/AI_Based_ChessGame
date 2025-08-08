#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "ai.h"
#include <string>

class Game {
public:
    Game();
    void loop();
private:
    Board board;
    AI ai;
    Color human_side;
};

#endif // GAME_H
