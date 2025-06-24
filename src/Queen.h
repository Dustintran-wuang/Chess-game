#ifndef QUEEN_H
#define QUEEN_H

#include "BasePiece.h"

class Queen : public BasePiece {
public:
    Queen(const sf::Vector2i& position, const sf::Color& color);
    ~Queen() override;

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const override;
};

#endif // QUEEN_H

