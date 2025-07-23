#pragma once
#include <SFML/Graphics.hpp>
#include "BasePiece.h"
#include <vector>

// Forward declaration
class Board;

class DragHandler {
private:
    Board* board;
    bool isDragging;
    bool hasSprite;
    Position startSquare;
    sf::Sprite draggedPieceSprite;
    Color currentTurn;
    std::vector<Position> validMoves;

    // Hàm hỗ trợ
    bool hasPieceAt(Position pos);
    sf::Sprite* getPieceSprite(Position pos);
    void calculateValidMoves();
    bool isValidMove(Position targetSquare) const;
    bool wouldExposeKing(Position from, Position to) const;
    void switchTurn();
    void checkGameState();
    void drawValidMoveHighlights(sf::RenderWindow& window);

public:
    DragHandler(Board* gameBoard);
    ~DragHandler();

    // Các hàm chính để xử lý kéo thả
    void start_Drag(Position square, sf::Vector2f mousePos);
    void update_Drag(sf::Vector2f mousePos);
    void end_Drag(Position targetSquare);

    // Hàm kiểm tra trạng thái
    bool is_Dragging_Piece();

    // Hàm vẽ
    void draw_Dragged_Piece(sf::RenderWindow& window);

    // Getter-Setter
    Position getStartSquare() const;
    void setCurrentTurn(Color turn);
    Color getCurrentTurn() const;
};