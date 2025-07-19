#pragma once
#include "BasePiece.h"

class Rook : public BasePiece {
private:
	bool castling = true;

public:
	Rook(Color color, Position pos)
		: BasePiece(color, PieceType::Rook, pos) {}

	// -------- Getter --------
	bool can_castling() const{
		return castling;
	}

	// -------- Setter --------
	void set_castling(bool x) {
		castling = x;
	}

	// -------- Logic --------
	bool is_move_valid(const Board& board, Position dest) const override;
	std::vector<Position> get_all_moves(const Board& board) const override;
	std::unique_ptr<BasePiece> clone() const override;
};
