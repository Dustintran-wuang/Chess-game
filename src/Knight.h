#ifndef KNIGHT_H
#define KNIGHT_H

#include "BasePiece.h"

class Knight : public BasePiece {
public:
    Knight(const sf::Vector2i& position, const sf::Color& color);
    ~Knight() override;

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;
};

#endif // KNIGHT_H
