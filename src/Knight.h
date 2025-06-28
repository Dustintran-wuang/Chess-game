#pragma once
#include "BasePiece.h"

using namespace std;

class Knight : public BasePiece {
public:
    Knight(Color color, Position pos)
        : BasePiece(color, PieceType::Knight, pos) {}

    // -------- Logic --------
    bool is_move_valid(const Board& board, Position dest) const override;
    vector<Position> get_all_moves(const Board& board) const override;
    unique_ptr<BasePiece> clone() const override;
};
