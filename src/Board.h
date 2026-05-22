#ifndef BOARD_H
#define BOARD_H

#include "Move.h"

#include <string>
#include <vector>

class Board {
private:
    char fields[8][8];

    bool parsePosition(const std::string& position, int& row, int& col) const;

    bool isPlayerPiece(char piece, char player) const;
    bool isOpponentPiece(char piece, char player) const;
    bool isKing(char piece) const;

    void promoteIfNeeded(int row, int col);

    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;

    // Bicie damka z dystansu
    bool findCapturePieceOnPath(int fromRow, int fromCol,
                                int toRow, int toCol,
                                char player,
                                int& capturedRow,
                                int& capturedCol) const;

    bool canManCaptureFrom(int row, int col, char player) const;
    bool canKingCaptureFrom(int row, int col, char player) const;

    void generateManMoves(int row, int col, char player,
                          bool onlyCaptures,
                          std::vector<Move>& moves) const;

    void generateKingMoves(int row, int col, char player,
                           bool onlyCaptures,
                           std::vector<Move>& moves) const;


    void generateManCaptureSequences(Board board, int row, int col, char player, Move currentMove, std::vector<Move>& moves) const;
    void generateKingCaptureSequences(Board board, int row, int col, char player, Move currentMove, std::vector<Move>& moves) const;

public:
    Board();

    void print() const;

    bool movePiece(const std::string& from, const std::string& to, char player);

    // Monitoring bic
    bool hasAnyCapture(char player) const;
    bool isCaptureMove(const std::string& from, const std::string& to, char player) const;
    bool canCaptureFromPosition(const std::string& position, char player) const;

    // Monitoring konca gry
    bool hasPieces(char player) const;

    // Metoda pod automatyzacje ruchow i min-max
    std::vector<Move> generateMoves(char player) const;
    void applyMove(const Move& move);

    //ocena planszy
    int evaluate(char player) const;
};

#endif