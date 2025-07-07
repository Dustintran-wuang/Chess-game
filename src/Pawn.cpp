#include "Pawn.h"
#include "ChessBoard.h"
#include <memory>

// --------- PHONG TỐT THÀNH HẬU, XE,... ----------
bool Pawn::can_promote() const {
    // Tốt trắng lên hàng 0, tốt đen lên hàng 7
    return (color == Color::White && pos.y == 0) ||
        (color == Color::Black && pos.y == 7);
}

// --------- EN PASSANT ----------
bool Pawn::can_en_passant(const Board& board, Position dest) const {
    int dir = (color == Color::White) ? -1 : 1; // Cờ trắng thì -1, đen thì 1
    if (dest.y != pos.y + dir) return false;          // FALSE nếu không tiến 1 hàng
    if (std::abs(dest.x - pos.x) != 1) return false;  // FALSE nếu không chéo 1 cột
    // Ô đích phải trống
    if (board.get_piece_at(dest) != nullptr) return false;

    // Kiểm tra xem quân địch nằm kế bên hay không
    Position sidePawn{ dest.x, pos.y };
    const BasePiece* side = board.get_piece_at(sidePawn);
    if (!side || side->get_pieceType() != PieceType::Pawn ||
        side->get_color() == color)
        return false;

    // Kiểm tra xem quân tốt của địch có vừa đi 2 bước không
    const Pawn* sidePawnPtr = dynamic_cast<const Pawn*>(side);
    return sidePawnPtr && sidePawnPtr->did_just_move_2step();
}

// --------- NƯỚC ĐI HỢP LỆ HAY KHÔNG ----------
bool Pawn::is_move_valid(const Board& board, Position dest) const {
    if (!board.is_inside_board(dest)) return false;
    int dir = (color == Color::White) ? -1 : 1;
    int dy = dest.y - pos.y;
    int dx = dest.x - pos.x;

    // 1) Đi thẳng
    if (dx == 0) {
        // Một bước
        if (dy == dir && board.get_piece_at(dest) == nullptr) return true;

        // Hai bước
        if (dy == 2 * dir && first_move) {
            Position mid{ pos.x, pos.y + dir };
            if (board.get_piece_at(mid) == nullptr &&
                board.get_piece_at(dest) == nullptr)
                return true;
        }
    }

    // 2) Ăn chéo
    if (std::abs(dx) == 1 && dy == dir) {
        const BasePiece* target = board.get_piece_at(dest);
        if (target && target->get_color() != color) return true;

        // 3) En passant
        if (can_en_passant(board, dest)) return true;
    }
    
    return false;
}

// --------- THÊM CÁC NƯỚC ĐI HỢP LỆ VÀO VECTOR ----------
std::vector<Position> Pawn::get_all_moves(const Board& board) const {
    std::vector<Position> moves;
    int dir = (color == Color::White) ? -1 : 1;

    // Đi 1 bước
    Position one{pos.x, pos.y + dir};
    if (board.is_inside_board(one) && board.get_piece_at(one) == nullptr)
        moves.push_back(one);

    // Đi 2 bước
    Position two{pos.x, pos.y + 2 * dir};
    if (first_move &&
        board.is_inside_board(two) && board.get_piece_at(one) == nullptr && board.get_piece_at(two) == nullptr)

        moves.push_back(two);

    // Ăn chéo
    for (int dx : {-1, 1}) {
        Position diag{pos.x + dx, pos.y + dir};

        if (!board.is_inside_board(diag)) continue;
        const BasePiece* target = board.get_piece_at(diag);

        if (target && target->get_color() != color)
            moves.push_back(diag);

        else if (can_en_passant(board, diag))
            moves.push_back(diag);
    }
    
    return moves;
}

// --------- CLONE ----------
std::unique_ptr<BasePiece> Pawn::clone() const {
    auto p = std::make_unique<Pawn>(*this);
    return p;
}

