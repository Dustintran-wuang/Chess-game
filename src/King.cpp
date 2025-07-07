#include "King.h"
#include "ChessBoard.h"             
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool King::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;

    int dx = std::abs(dest.x - pos.x);
    int dy = std::abs(dest.y - pos.y);

    // Di chuyển 1 ô xung quanh
    if ((dx <= 1 && dy <= 1) && !(dx == 0 && dy == 0)) {
        const BasePiece* target = board.get_piece_at(dest);
        return (!target || target->get_color() != color);
    }

    // --- Castling ---
    if (!castling || pos.y != dest.y) return false;

    // Chỉ cho phép castling theo hàng, qua 2 ô
    if (dy == 0 && dx == 2) {
        int direction = (dest.x - pos.x > 0) ? 1 : -1;
        Position rookPos = (direction == 1) ? Position{7, pos.y} : Position{0, pos.y};
        const BasePiece* rook = board.get_piece_at(rookPos);

        // Quân xe phải tồn tại, cùng màu và chưa di chuyển
        if (!rook || rook->get_pieceType() != PieceType::Rook || rook->get_color() != color)
            return false;

        // Cả vua và xe chưa di chuyển
        if (!can_castling() || !board.can_castle_rook(rookPos))
            return false;

        // Không bị chiếu và không đi qua ô bị chiếu
        Position mid1{pos.x + direction, pos.y};
        Position mid2{pos.x + 2 * direction, pos.y};

        if (board.is_check(color)) return false;
        if (board.is_square_under_attacked(mid1, color)) return false;
        if (board.is_square_under_attacked(mid2, color)) return false;

        // Kiểm tra không có quân cờ nào cản đường
        if (board.get_piece_at(mid1) || board.get_piece_at(mid2)) return false;

        return true;
    }

    return false;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> King::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    // Di chuyển thông thường
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            Position next{ pos.x + dx, pos.y + dy };
            if (!board.is_inside_board(next)) continue;

            const BasePiece* target = board.get_piece_at(next);
            if (!target || target->get_color() != color) {
                moves.push_back(next);
            }
        }
    }

    // --- Castling ---
    if (!castling || board.is_check(color)) return moves;

    for (int dir : {-1, 1}) {
        Position rookPos = (dir == 1) ? Position{7, pos.y} : Position{0, pos.y};
        const BasePiece* rook = board.get_piece_at(rookPos);

        if (!rook || rook->get_pieceType() != PieceType::Rook || rook->get_color() != color)
            continue;

        if (!board.can_castle_rook(rookPos))
            continue;

        Position mid1{pos.x + dir, pos.y};
        Position mid2{pos.x + 2 * dir, pos.y};

        if (board.get_piece_at(mid1) || board.get_piece_at(mid2)) continue;
        if (board.is_square_under_attacked(mid1, color) || board.is_square_under_attacked(mid2, color)) continue;

        moves.push_back(mid2);
    }

    return moves;
}

// --------- CLONE ----------
std::unique_ptr<BasePiece> King::clone() const {
    return std::make_unique<King>(*this);
}
