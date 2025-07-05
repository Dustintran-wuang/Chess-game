#include "Rook.h"
#include "ChessBoard.h"
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool Rook::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;
    if (dest == pos) return false;

    int dx = dest.x - pos.x;
    int dy = dest.y - pos.y;

    // Chỉ cho phép đi thẳng hàng hoặc cột
    if (dx != 0 && dy != 0) return false;

    // Kiểm tra đường đi có bị cản bởi quân cờ khác không
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    Position current = pos;
    current.x += stepX;
    current.y += stepY;

    while (current != dest) {
        if (board.get_piece_at(current) != nullptr) {
            return false;
        }
        current.x += stepX;
        current.y += stepY;
    }

    // Ô đến phải trống hoặc ô có quân địch
    const BasePiece* target = board.get_piece_at(dest);
    if (target && target->get_color() == color) {
        return false;
    }

    return true;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> Rook::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (auto& dir : directions) {
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
std::unique_ptr<BasePiece> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}
