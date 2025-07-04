#ifndef DRAGHANDLER_H
#define DRAGHANDLER_H

#include <SFML/Graphics.hpp>
#include "BasePiece.h"


class Board;

class DragHandler {
private:
    bool isDragging;                    // trạng thái đang kéo thả quân cờ hay không
    Position startSquare;               // vị trí ô bàn cờ lúc bắt đầu kéo quân (dùng Position từ BasePiece.h)
    sf::Sprite draggedPieceSprite;      // sprite của quân cờ đang được kéo
    bool hasSprite;                     // có sprite cần vẽ hay không
    Board* board;                       // con trỏ tới bàn cờ để lấy thông tin quân cờ và cập nhật nước đi

public:
    DragHandler(Board* gameBoard);

    ~DragHandler();

    void start_Drag(Position square, sf::Vector2f mousePos);
    void update_Drag(sf::Vector2f mousePos);
    void end_Drag(Position targetSquare);
    bool is_Dragging_Piece();
    void draw_Dragged_Piece(sf::RenderWindow& window);

    //  hàm hổ trợ để tương tác với Board
    bool hasPieceAt(Position pos);              // Kiểm tra có quân cờ tại vị trí
    sf::Sprite* getPieceSprite(Position pos);   // Lấy sprite của quân cờ
    Position getStartSquare() const;            // Lấy vị trí bắt đầu kéo
};

#endif