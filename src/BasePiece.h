#ifndef BASEPIECE_H
#define BASEPIECE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Abstract base class for all chess pieces
class BasePiece {
protected:
    std::string name;               // Name of the piece ("Pawn", "Knight", etc.)
    sf::Vector2i position;          // Board position (0..7, 0..7)
    sf::Color color;                // Color (white or black)
public:
    BasePiece(const std::string& name, const sf::Vector2i& position, const sf::Color& color);
    virtual ~BasePiece();

    // Pure virtual functions to be overridden
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, BasePiece* board[8][8]) const = 0;

    // Getters
    const std::string& getName() const;
    const sf::Vector2i& getPosition() const;
    const sf::Color& getColor() const;

    // Setter
    void setPosition(const sf::Vector2i& newPos);
};

#endif
