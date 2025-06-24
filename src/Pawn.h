#ifndef PAWN_H
#define PAWN_H

#include "BasePiece.h"

class Pawn : public BasePiece {
private:
    bool has_moved = false;         // Kiểm tra đã di chuyển lần đầu hay chưa
    bool just_moved_two = false;    // Dùng cho en passant
public:
    Pawn(const sf::Vector2i& position, const sf::Color& color);
    ~Pawn() override;

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;

    bool isPromotionMove(int toX) const;
    bool canPromote() const;
    bool hasMoved() const;
    void setHasMoved(bool moved);
    bool isEnPassantMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const;
    bool canEnPassant() const;
    void setJustMovedTwo(bool movedTwo);
    bool didJustMoveTwo() const;
};

#endif // PAWN_H
