#include "King.h"
#include "ChessBoard.h"             
#include <cmath>

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool King::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;
    if (dest == pos) return false;

    int dx = dest.x - pos.x;
    int dy = dest.y - pos.y;

    // ----- Di chuyển bình thường -----
    if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
        const BasePiece* target = board.get_piece_at(dest);
        if (target && target->get_color() == color)
            return false;

        // Không được đi vào ô bị chiếu
        if (board.is_square_under_attacked(dest, color == Color::White ? Color::Black : Color::White))
            return false;

        return true;
    }

    // ----- Nhập thành -----
    if (dy == 0 && std::abs(dx) == 2) {
        // 1. Vua chưa di chuyển
        if (!can_castling()) return false;

        Color opponent = (color == Color::White) ? Color::Black : Color::White;

        // 2. Không đang bị chiếu
        if (board.is_square_under_attacked(pos, opponent))
            return false;

        // 3. Xác định hướng (kingSide = true nếu dx > 0)
        bool kingSide = (dx > 0);
        Position rookPos = kingSide
            ? Position{ pos.x + 3, pos.y } // rook bên phải
        : Position{ pos.x - 4, pos.y }; // rook bên trái

        const BasePiece* rook = board.get_piece_at(rookPos);
        if (!rook || rook->get_pieceType() != PieceType::Rook) return false;

        const Rook* r = dynamic_cast<const Rook*>(rook);
        if (!r || !r->can_castling()) return false;

        // 4. Các ô giữa vua và rook phải trống
        int step = kingSide ? 1 : -1;
        Position checkPos = pos;
        for (int i = 1; i < (kingSide ? 3 : 4); i++) {
            checkPos.x += step;
            if (board.get_piece_at(checkPos) != nullptr)
                return false;
        }

        // 5. Các ô vua đi qua không bị chiếu
        checkPos = pos;
        for (int i = 1; i <= 2; i++) {
            checkPos.x += step;
            if (board.is_square_under_attacked(checkPos, opponent))
                return false;
        }
        return true;
    }
    return false;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> King::get_all_moves(const Board& board) const {
    std::vector<Position> moves;

    // --- Di chuyển thông thường ---
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            Position next{ pos.x + dx, pos.y + dy };

            if (is_move_valid(board, next)) {
                moves.push_back(next);
            }
        }
    }

    // --- Nhập thành ---
    Position kingSideDest{ pos.x + 2, pos.y };
    if (is_move_valid(board, kingSideDest)) {
        moves.push_back(kingSideDest);
    }

    Position queenSideDest{ pos.x - 2, pos.y };
    if (is_move_valid(board, queenSideDest)) {
        moves.push_back(queenSideDest);
    }

    return moves;
}

// --------- CLONE ----------
std::unique_ptr<BasePiece> King::clone() const {
    return std::make_unique<King>(*this);
}
