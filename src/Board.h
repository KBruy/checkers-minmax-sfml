#ifndef BOARD_H
#define BOARD_H

class Board {
    private:
        char fields[8][8];

    public:
        Board();

        void print() const;
};

#endif