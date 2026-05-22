#include "AI.h"

#include <vector>

AI::AI(char aiPlayer, int maxDepth) {
    this->aiPlayer = aiPlayer;
    this->maxDepth = maxDepth;

    if (aiPlayer == 'w') {
        humanPlayer = 'b';
    } else {
        humanPlayer = 'w';
    }
}

char AI::getOpponent(char player) const {
    if (player == 'w') {
        return 'b';
    }

    return 'w';
}

Move AI::findBestMove(Board board) {
    std::vector<Move> moves = board.generateMoves(aiPlayer);

    if (moves.empty()) {
        return Move();
    }

    Move bestMove = moves[0];
    int bestScore = -1000000;

    for (int i =0; i< static_cast<int>(moves.size()); i++) {
        Board copy = board;
        copy.applyMove(moves[i]);

        int score = minmax(copy, maxDepth -1, humanPlayer);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}

int AI::minmax(Board board, int depth, char currentPlayer) {
    if (depth == 0 || !board.hasPieces('w') || !board.hasPieces('b')) {
        return board.evaluate(aiPlayer);
    }

    std::vector<Move> moves = board.generateMoves(currentPlayer);

    if (moves.empty()) {
        return board.evaluate(aiPlayer);
    }

    if (currentPlayer == aiPlayer) {
        int bestScore = -1000000;

        for (int i=0; i<static_cast<int>(moves.size()); i++) {
            Board copy = board;
            copy.applyMove(moves[i]);

            int score = minmax(copy, depth -1, getOpponent(currentPlayer));

            if (score > bestScore) {
                bestScore = score;
            }
        }

        return bestScore;
    } else {
        int bestScore = 1000000;

        for (int i = 0; i < static_cast<int>(moves.size()); i++) {
            Board copy = board;
            copy.applyMove(moves[i]);

            int score = minmax(copy, depth -1, getOpponent(currentPlayer));

            if(score < bestScore) {
                bestScore = score;
            }
        }

        return bestScore;
    }
}