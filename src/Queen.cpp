#include "Queen.h"
#include "ChessBoard.h"
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool Queen::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest) || dest == pos)
        return false;

    int dx = dest.x - pos.x;
    int dy = dest.y - pos.y;

    // Kiểm tra xem có đi thẳng hoặc chéo không
    if (dx != 0 && dy != 0 && std::abs(dx) != std::abs(dy))
        return false;

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    Position current = pos;
    current.x += stepX;
    current.y += stepY;

    // Kiểm tra đường bị chặn
    while (current != dest) {
        if (board.get_piece_at(current) != nullptr)
            return false;
        current.x += stepX;
        current.y += stepY;
    }

    // Đích đến phải trống hoặc ăn quân địch
    const BasePiece* target = board.get_piece_at(dest);
    if (target && target->get_color() == color)
        return false;

    return true;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> Queen::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},    // Gộp 2 thằng
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}   // Bishop với Rook
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
std::unique_ptr<BasePiece> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}
