#include "Knight.h"
#include "ChessBoard.h"
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool Knight::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;

    int dx = std::abs(dest.x - pos.x);
    int dy = std::abs(dest.y - pos.y);

    // Mã đi hình chữ L: (2,1) hoặc (1,2)
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2)))
        return false;

    const BasePiece* target = board.get_piece_at(dest);
    return (!target || target->get_color() != color);
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> Knight::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {pos.x + 2, pos.y + 1}, {pos.x + 2, pos.y - 1},
        {pos.x - 2, pos.y + 1}, {pos.x - 2, pos.y - 1},
        {pos.x + 1, pos.y + 2}, {pos.x + 1, pos.y - 2},
        {pos.x - 1, pos.y + 2}, {pos.x - 1, pos.y - 2}
    };

    for (const Position& p : directions) {
        if (!board.is_inside_board(p)) continue;

        const BasePiece* target = board.get_piece_at(p);
        if (!target || target->get_color() != color) {
            moves.push_back(p);
        }
    }
    return moves;
}

// --------- CLONE ----------
std::unique_ptr<BasePiece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}
