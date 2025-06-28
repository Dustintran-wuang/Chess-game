#pragma once
#include "BasePiece.h"

class Bishop : public BasePiece {
public:
    Bishop(Color color, Position pos)
        : BasePiece(color, PieceType::Bishop, pos) {}

    // -------- Logic --------
    bool is_move_valid(const Board& board, Position dest) const override;
    std::vector<Position> get_all_moves(const Board& board) const override;
    std::unique_ptr<BasePiece> clone() const override;
};
