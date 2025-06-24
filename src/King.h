#ifndef KING_H
#define KING_H

#include "BasePiece.h"

class King : public BasePiece {
private:
    bool has_moved; // true nếu vua đã di chuyển (ảnh hưởng đến nhập thành)
public:
    King(const sf::Vector2i& position, const sf::Color& color);
    ~King() override;

    void draw(sf::RenderWindow& window) override;

    // Kiểm tra nước đi có hợp lệ không (bao gồm cả castling)
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;

    // Các hàm kiểm tra riêng cho castling
    bool isCastlingMove(int fromX, int fromY, int toX, int toY) const;
    bool canCastle() const; // Vua chưa từng di chuyển
    bool hasMovedBefore() const;

    // Cập nhật trạng thái đã di chuyển
    void setHasMoved(bool moved);
};

#endif // KING_H
