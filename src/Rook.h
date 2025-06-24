#ifndef ROOK_H
#define ROOK_H

#include "BasePiece.h"

class Rook : public BasePiece {
private:
    bool has_moved = false;
public:
    Rook(const sf::Vector2i& position, const sf::Color& color);
    ~Rook() override;

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;

    bool canCastle() const;
    bool hasMoved() const;
    void setHasMoved(bool moved);
};

#endif // ROOK_H
