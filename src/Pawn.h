#pragma once
#include "BasePiece.h"

class Pawn : public BasePiece {
public:
	bool first_move = true;
	bool just_moved_2step = false;

private:
	Pawn(Color color, Position pos)
		: BasePiece(color, PieceType::Pawn, pos) {}

	// -------- Getter --------
	bool is_first_move() const {
		return first_move;
	}
	bool did_just_move_2step() const {
		return just_moved_2step;
	}

	// -------- Setter --------
	void set_first_move(bool x) {
		first_move = x;
	}
	void set_just_moved_2step(bool x) {
		just_moved_2step = x;
	}

	// -------- Logic --------
	bool can_promote() const; // Tốt thăng thần <(")
	bool can_en_passant(const Board& board, Position dest) const; // Tốt thăng hoa <(")
	bool is_move_valid(const Board& board, Position dest) const override;
	std::vector<Position> get_all_moves(const Board& board) const override;
	std::unique_ptr<BasePiece> clone() const override;
};
