#include "DragHandler.h"
#include "ChessBoard.h"

DragHandler::DragHandler(Board* gameBoard)
    : board(gameBoard), isDragging(false), hasSprite(false) {
    // Khởi tạo startSquare với giá trị mặc định
    startSquare = { -1, -1 }; // Vị trí không hợp lệ
}

DragHandler::~DragHandler() {
    // Clean up tự động với SFML
}

void DragHandler::start_Drag(Position square, sf::Vector2f mousePos) {
    // Kiểm tra có quân cờ tại vị trí không
    if (!hasPieceAt(square)) {
        return;
    }

    // Lấy sprite của quân cờ
    sf::Sprite* pieceSprite = getPieceSprite(square);
    if (!pieceSprite) {
        return;
    }

    // Bắt đầu kéo
    isDragging = true;
    startSquare = square;
    hasSprite = true;

    // Copy sprite để kéo
    draggedPieceSprite = *pieceSprite;

    // Đặt vị trí sprite theo chuột (căn giữa)
    sf::FloatRect bounds = draggedPieceSprite.getLocalBounds();
    draggedPieceSprite.setPosition(mousePos.x - bounds.width / 2,
        mousePos.y - bounds.height / 2);
}

void DragHandler::update_Drag(sf::Vector2f mousePos) {
    if (!isDragging || !hasSprite) {
        return;
    }

    // Cập nhật vị trí sprite theo chuột
    sf::FloatRect bounds = draggedPieceSprite.getLocalBounds();
    draggedPieceSprite.setPosition(mousePos.x - bounds.width / 2,
        mousePos.y - bounds.height / 2);
}

void DragHandler::end_Drag(Position targetSquare) {
    if (!isDragging) {
        return;
    }

    // Kiểm tra nước đi có hợp lệ không
    bool validMove = false;
    if (board && startSquare.x != -1 && startSquare.y != -1) {
        // Kiểm tra cơ bản: vị trí đích phải khác vị trí xuất phát
        if (startSquare.x != targetSquare.x || startSquare.y != targetSquare.y) {
            // Kiểm tra vị trí đích có hợp lệ không
            if (targetSquare.x >= 0 && targetSquare.x < 8 &&
                targetSquare.y >= 0 && targetSquare.y < 8) {

                // Thực hiện di chuyển sử dụng method movePiece từ Board
                board->movePiece(startSquare.y, startSquare.x,
                    targetSquare.y, targetSquare.x);
                validMove = true;
            }
        }
    }

    // Reset trạng thái kéo
    isDragging = false;
    hasSprite = false;
    startSquare = { -1, -1 };
}

bool DragHandler::is_Dragging_Piece() {
    return isDragging;
}

void DragHandler::draw_Dragged_Piece(sf::RenderWindow& window) {
    if (isDragging && hasSprite) {
        // Thêm hiệu ứng trong suốt khi kéo
        sf::Uint8 originalAlpha = draggedPieceSprite.getColor().a;
        sf::Color color = draggedPieceSprite.getColor();
        color.a = 200; // Làm hơi trong suốt một chút 
        draggedPieceSprite.setColor(color);

        window.draw(draggedPieceSprite);

        // Restore alpha gốc
        color.a = originalAlpha;
        draggedPieceSprite.setColor(color);
    }
}

// Các hàm hỗ trợ tương tác với ChessBoard
bool DragHandler::hasPieceAt(Position pos) {
    if (!board) {
        return false;
    }

    // Kiểm tra giới hạn bàn cờ
    if (pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8) {
        return false;
    }

    // Sử dụng getPieceName từ Board class
    std::string pieceName = board->getPieceName(pos.y, pos.x);
    return !pieceName.empty();
}

sf::Sprite* DragHandler::getPieceSprite(Position pos) {
    if (!board) {
        return nullptr;
    }

    // Kiểm tra giới hạn bàn cờ
    if (pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8) {
        return nullptr;
    }

    // Kiểm tra có quân cờ tại vị trí không
    if (!hasPieceAt(pos)) {
        return nullptr;
    }

    return board->getPieceSpriteAt(pos);
}

Position DragHandler::getStartSquare() const {
    return startSquare;
}
