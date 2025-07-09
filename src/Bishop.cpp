
#include "Bishop.h"
#include "ChessBoard.h"
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool Bishop::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;
    if (dest == pos) return false;

    int dx = dest.x - pos.x;
    int dy = dest.y - pos.y;

    if (std::abs(dx) != std::abs(dy)) return false;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    Position current = pos;
    current.x += stepX;
    current.y += stepY;

    while (current != dest) {
        if (board.get_piece_at(current) != nullptr)
            return false;
        current.x += stepX;
        current.y += stepY;
    }

    const BasePiece* target = board.get_piece_at(dest);
    if (target && target->get_color() == color)
        return false;

    return true;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> Bishop::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const Position& dir : directions) {
        Position current = pos;
        while (true) {
            current.x += dir.x;
            current.y += dir.y;

            if (!board.is_inside_board(current))
                break;

            const BasePiece* target = board.get_piece_at(current);
            if (!target) {
                moves.push_back(current);
            }
            else {
                if (target->get_color() != color)
                    moves.push_back(current);
                break;
            }
        }
    }
    return moves;
}

// --------- CLONE ----------
std::unique_ptr<BasePiece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}
