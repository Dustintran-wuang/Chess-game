#ifndef BISHOP_H
#define BISHOP_H

#include "BasePiece.h"

class Bishop : public BasePiece {
public:
    Bishop(const sf::Vector2i& position, const sf::Color& color);
    ~Bishop() override;

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;
};

#endif // BISHOP_H
