#include "Game.h"

Game::Game() : board(), gameOver(false) {
    board.loadAssets();
    dragHandler = new DragHandler(&board, this);
}

Game::~Game() {
    delete dragHandler;
}



void Game::update() {
    // Logic kiểm tra thắng thua ở đây
    // Ví dụ đơn giản:
    if (/* ví dụ */ false) {
        gameOver = true;
    }
}

void Game::render(sf::RenderWindow& window) {
    board.draw(window);

    if (dragHandler->is_Dragging_Piece()) {
        dragHandler->draw_Dragged_Piece(window);
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2u winSize = window.getSize();
    int tileSize = std::min(winSize.x, winSize.y) / 8;
    int boardSize = tileSize * 8;
    int offsetX = (winSize.x - boardSize) / 2;
    int offsetY = (winSize.y - boardSize) / 2;

    auto getRotatedPosition = [&](int col, int row) -> Position {
        if (currentMode == GameMode::PlayerVsPlayer && board.getRotation()) {
            return Position{ 7 - col, 7 - row };
        }
        else {
            return Position{ col, row };
        }
        };

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;

        if (board.isShowingPromotion()) {
            board.handlePromotionClick(mouseX, mouseY, window);
            return;
        }

        sf::Vector2f mousePos(mouseX, mouseY);
        int col = (mousePos.x - offsetX) / tileSize;
        int row = (mousePos.y - offsetY) / tileSize;

        Position adjustedPos = getRotatedPosition(col, row);
        dragHandler -> start_Drag(adjustedPos, mousePos);
    }
    else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        dragHandler -> update_Drag(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        int col = (mousePos.x - offsetX) / tileSize;
        int row = (mousePos.y - offsetY) / tileSize;

        Position adjustedPos = getRotatedPosition(col, row);
        dragHandler -> end_Drag(adjustedPos);
    }
}

void Game::startNewGame() {
    board.startGame();     // Phát âm thanh bắt đầu
    gameOver = false;

    // Quân trắng
    for (int i = 0; i < 8; ++i) {
        board.setPiece(6, i, "wPawn");
        board.setLogicPiece(6, i, std::make_unique<Pawn>(Color::White, Position{ i,6 }));
    }

    board.setPiece(7, 0, "wRook");
    board.setLogicPiece(7, 0, std::make_unique<Rook>(Color::White, Position{ 0,7 }));

    board.setPiece(7, 1, "wKnight");
    board.setLogicPiece(7, 1, std::make_unique<Knight>(Color::White, Position{ 1,7 }));

    board.setPiece(7, 2, "wBishop");
    board.setLogicPiece(7, 2, std::make_unique<Bishop>(Color::White, Position{ 2,7 }));

    board.setPiece(7, 3, "wQueen");
    board.setLogicPiece(7, 3, std::make_unique<Queen>(Color::White, Position{ 3,7 }));

    board.setPiece(7, 4, "wKing");
    board.setLogicPiece(7, 4, std::make_unique<King>(Color::White, Position{ 4,7 }));

    board.setPiece(7, 5, "wBishop");
    board.setLogicPiece(7, 5, std::make_unique<Bishop>(Color::White, Position{ 5,7 }));

    board.setPiece(7, 6, "wKnight");
    board.setLogicPiece(7, 6, std::make_unique<Knight>(Color::White, Position{ 6,7 }));

    board.setPiece(7, 7, "wRook");
    board.setLogicPiece(7, 7, std::make_unique<Rook>(Color::White, Position{ 7,7 }));

    // Quân đen
    for (int i = 0; i < 8; ++i) {
        board.setPiece(1, i, "bPawn");
        board.setLogicPiece(1, i, std::make_unique<Pawn>(Color::Black, Position{ i,1 }));
    }

    board.setPiece(0, 0, "bRook");
    board.setLogicPiece(0, 0, std::make_unique<Rook>(Color::Black, Position{ 0,0 }));

    board.setPiece(0, 1, "bKnight");
    board.setLogicPiece(0, 1, std::make_unique<Knight>(Color::Black, Position{ 1,0 }));

    board.setPiece(0, 2, "bBishop");
    board.setLogicPiece(0, 2, std::make_unique<Bishop>(Color::Black, Position{ 2,0 }));

    board.setPiece(0, 3, "bQueen");
    board.setLogicPiece(0, 3, std::make_unique<Queen>(Color::Black, Position{ 3,0 }));

    board.setPiece(0, 4, "bKing");
    board.setLogicPiece(0, 4, std::make_unique<King>(Color::Black, Position{ 4,0 }));

    board.setPiece(0, 5, "bBishop");
    board.setLogicPiece(0, 5, std::make_unique<Bishop>(Color::Black, Position{ 5,0 }));

    board.setPiece(0, 6, "bKnight");
    board.setLogicPiece(0, 6, std::make_unique<Knight>(Color::Black, Position{ 6,0 }));

    board.setPiece(0, 7, "bRook");
    board.setLogicPiece(0, 7, std::make_unique<Rook>(Color::Black, Position{ 7,0 }));
}

std::string Game::getDifficulty() const {
    return difficulty;
}

void Game::setGameMode(GameMode mode) {
    currentMode = mode;
}

GameMode Game::getGameMode() const {
    return currentMode;
}

// Hàm này sẽ được gọi khi người dùng chọn độ khó từ menu
void Game::setDifficulty(const std::string& difficulty) {
    int searchDepth = 4; // Mặc định là Medium

    if (difficulty == "Easy") {
        searchDepth = 2; // AI tìm kiếm ít bước hơn
    }
    else if (difficulty == "Medium") {
        searchDepth = 4; // Cân bằng
    }
    else if (difficulty == "Hard") {
        searchDepth = 6; // AI tìm kiếm sâu hơn, thông minh hơn
    }

    // Gọi hàm setDifficulty có sẵn trong ChessBot (AIEngine.cpp)
    // để cập nhật độ sâu tìm kiếm cho AI.
    m_chessBot.setDifficulty(searchDepth);
}

// === PHẦN CẦN KÍCH HOẠT ===
void Game::makeAIMove() {
    // 1. Đảm bảo đến lượt AI và game chưa kết thúc
    if (board.getCurrentTurn() != m_aiColor || isGameOver()) {
        return;
    }

    // 2. Gọi AI để tìm nước đi tốt nhất
    //    Hàm findBestMove sẽ sử dụng giá trị "depth" bạn đã set trong setDifficulty
    std::cout << "AI is thinking..." << std::endl;
    Move bestMove = m_chessBot.findBestMove(board, m_aiColor);

    // 3. Thực hiện nước đi trên bàn cờ
    board.makeMove(bestMove);
    std::cout << "AI has moved." << std::endl;
}

// Hàm này trả về màu của AI để bạn có thể sử dụng trong logic game
Color Game::getAIColor() const {
    return m_aiColor; // Trả về màu của AI mà bạn đã định nghĩa trong Game.h
}
