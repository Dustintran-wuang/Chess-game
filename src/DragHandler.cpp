#include "DragHandler.h"
#include "ChessBoard.h"
#include "Game.h"

DragHandler::DragHandler(Board* gameBoard, Game* g)
    : board(gameBoard), game(g), isDragging(false), hasSprite(false), currentTurn(Color::White) {
    startSquare = { -1, -1 };
}

DragHandler::~DragHandler() {
}

void DragHandler::start_Drag(Position square, sf::Vector2f mousePos) {
    // Kiểm tra có quân cờ tại vị trí không
    if (!hasPieceAt(square)) {
        return;
    }

    // Kiểm tra có phải lượt của quân cờ này không
    const BasePiece* piece = board->get_piece_at(square);
    if (!piece || piece->get_color() != currentTurn) {
        return; // Không phải lượt của quân này
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

    // Tính toán và lưu các nước đi hợp lệ
    calculateValidMoves();
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

    bool validMove = false;

    if (board && startSquare.x != -1 && startSquare.y != -1) {
        // Kiểm tra vị trí đích có hợp lệ không
        if (board->is_inside_board(targetSquare)) {
            // Kiểm tra nước đi có trong danh sách nước đi hợp lệ không
            if (isValidMove(targetSquare)) {
                // Lấy quân cờ để kiểm tra
                const BasePiece* piece = board->get_piece_at(startSquare);
                if (piece && piece->is_move_valid(*board, targetSquare)) {

                    // Kiểm tra xem nước đi có khiến vua bị chiếu không
                    if (!wouldExposeKing(startSquare, targetSquare)) {
                        // Thực hiện di chuyển
                        board->movePiece(startSquare.y, startSquare.x,
                            targetSquare.y, targetSquare.x);

                        validMove = true;

                        // Đổi lượt
                        switchTurn();

                        // Nếu chơi PvP, xoay bàn cờ
                        if (game && game->getGameMode() == GameMode::PlayerVsPlayer) {
                            bool currentRotation = board->getRotation();
                            board->setRotation(!currentRotation);
                        }

                        // Kiểm tra trạng thái game (chiếu, chiếu hết)
                        checkGameState();
                    }
                }
            }
        }
    }

    // Reset trạng thái kéo
    isDragging = false;
    hasSprite = false;
    startSquare = { -1, -1 };
    validMoves.clear();
}

bool DragHandler::is_Dragging_Piece() {
    return isDragging;
}

void DragHandler::draw_Dragged_Piece(sf::RenderWindow& window) {
    if (isDragging && hasSprite) {
        // Vẽ highlight cho các nước đi hợp lệ
        drawValidMoveHighlights(window);

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

    if (!board->is_inside_board(pos)) {
        return false;
    }

    return board->get_piece_at(pos) != nullptr;
}

sf::Sprite* DragHandler::getPieceSprite(Position pos) {
    if (!board || !board->is_inside_board(pos)) {
        return nullptr;
    }

    if (!hasPieceAt(pos)) {
        return nullptr;
    }

    return board->getPieceSpriteAt(pos);
}

Position DragHandler::getStartSquare() const {
    return startSquare;
}

// Các hàm mới để xử lý logic game
void DragHandler::calculateValidMoves() {
    validMoves.clear();

    if (!board || startSquare.x == -1 || startSquare.y == -1) {
        return;
    }

    const BasePiece* piece = board->get_piece_at(startSquare);
    if (!piece) {
        return;
    }

    // Lấy tất cả nước đi có thể của quân cờ
    std::vector<Position> possibleMoves = piece->get_all_moves(*board);

    // Lọc ra những nước đi không khiến vua bị chiếu
    for (const Position& move : possibleMoves) {
        if (!wouldExposeKing(startSquare, move)) {
            validMoves.push_back(move);
        }
    }
}

bool DragHandler::isValidMove(Position targetSquare) const {
    for (const Position& move : validMoves) {
        if (move.x == targetSquare.x && move.y == targetSquare.y) {
            return true;
        }
    }
    return false;
}

bool DragHandler::wouldExposeKing(Position from, Position to) const {
    if (!board) return true;

    // Tạo bản sao board để test
    Board testBoard(*board);

    // Thực hiện nước đi thử
    auto capturedPiece = testBoard.move_piece_for_ai(from, to);

    // Kiểm tra vua có bị chiếu không
    bool kingInCheck = testBoard.is_check(currentTurn);

    // Hoàn tác nước đi
    testBoard.undo_move_for_ai(from, to, std::move(capturedPiece));

    return kingInCheck;
}

void DragHandler::switchTurn() {
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
}

void DragHandler::checkGameState() {
    if (!board) return;

    Color opponent = (currentTurn == Color::White) ? Color::Black : Color::White;

    if (board->is_check(opponent)) {
        if (board->isCheckmate(opponent)) {
            // Chiếu hết - game over
            std::cout << "Checkmate! " <<
                (currentTurn == Color::White ? "White" : "Black") << " wins!" << std::endl;
            board->playSound("game-end");
        }
        else {
            // Chỉ chiếu
            std::cout << "Check!" << std::endl;
            board->playSound("check");
        }
    }
}

void DragHandler::drawValidMoveHighlights(sf::RenderWindow& window) {
    if (!isDragging || validMoves.empty()) {
        return;
    }

    sf::Vector2u windowSize = window.getSize();
    int tileSize = std::min(windowSize.x, windowSize.y) / 8;
    int boardSize = tileSize * 8;
    int offsetX = (windowSize.x - boardSize) / 2;
    int offsetY = (windowSize.y - boardSize) / 2;

    bool isRotated = board->getRotation();

    for (const Position& move : validMoves) {
        int drawX = isRotated ? 7 - move.x : move.x;
        int drawY = isRotated ? 7 - move.y : move.y;

        sf::CircleShape highlight(tileSize * 0.15f);
        highlight.setFillColor(sf::Color(0, 255, 0, 100)); // Màu xanh lá trong suốt

        highlight.setPosition(
            offsetX + drawX * tileSize + tileSize / 2 - highlight.getRadius(),
            offsetY + drawY * tileSize + tileSize / 2 - highlight.getRadius()
        );

        window.draw(highlight);
    }
}

void DragHandler::setCurrentTurn(Color turn) {
    currentTurn = turn;
}

Color DragHandler::getCurrentTurn() const {
    return currentTurn;
}
