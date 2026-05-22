#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Move.h"

class AI {
    private:
        char aiPlayer;
        char humanPlayer;
        int maxDepth;

        int minmax(Board board, int depth, char currentPlayer);
        char getOpponent(char player) const;

        int visitedNodes; //metoda wyswietlająca ilość przeanalizowyanych możliwosći

    public:
        AI(char aiPlayer, int maxDepth);

        Move findBestMove(Board board);
        int getVisitedNodes() const; 
};

#endif