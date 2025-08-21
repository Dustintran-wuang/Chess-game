#include "Game.h"
#include <iostream>

Game::Game() :  stockFish("C:/Users/ASUS/Downloads/stockfish-windows-x86-64-avx2/stockfish/stockfish.exe"), //them path o day
                board(), 
                gameOver(false), 
                gameState(GameState::Playing) {
    board.loadAssets();
    dragHandler = new DragHandler(&board, this);
    gameResult = "";
}

Game::~Game() {
    delete dragHandler;
}

void Game::update() {
    if (!gameOver) {
        checkGameEndConditions();
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
    if (gameOver) return; // Không xử lý input nếu game đã kết thúc

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
        dragHandler->start_Drag(adjustedPos, mousePos);
    }
    else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        dragHandler->update_Drag(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        int col = (mousePos.x - offsetX) / tileSize;
        int row = (mousePos.y - offsetY) / tileSize;

        Position adjustedPos = getRotatedPosition(col, row);
        dragHandler->end_Drag(adjustedPos);
    }
}

void Game::startNewGame() {
    // Xóa hết quân cờ cũ (logic)
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            board.setLogicPiece(y, x, nullptr);
        }
    }

    // Xóa hết sprite cũ (hiển thị)
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            board.setPiece(y, x, ""); // Xóa tên quân cờ
        }
    }

    // Quân đen
    board.setPiece(0, 0, "bRook");   board.setLogicPiece(0, 0, std::make_unique<Rook>(Color::Black, Position{ 0, 0 }));
    board.setPiece(0, 1, "bKnight"); board.setLogicPiece(0, 1, std::make_unique<Knight>(Color::Black, Position{ 1, 0 }));
    board.setPiece(0, 2, "bBishop"); board.setLogicPiece(0, 2, std::make_unique<Bishop>(Color::Black, Position{ 2, 0 }));
    board.setPiece(0, 3, "bQueen");  board.setLogicPiece(0, 3, std::make_unique<Queen>(Color::Black, Position{ 3, 0 }));
    board.setPiece(0, 4, "bKing");   board.setLogicPiece(0, 4, std::make_unique<King>(Color::Black, Position{ 4, 0 }));
    board.setPiece(0, 5, "bBishop"); board.setLogicPiece(0, 5, std::make_unique<Bishop>(Color::Black, Position{ 5, 0 }));
    board.setPiece(0, 6, "bKnight"); board.setLogicPiece(0, 6, std::make_unique<Knight>(Color::Black, Position{ 6, 0 }));
    board.setPiece(0, 7, "bRook");   board.setLogicPiece(0, 7, std::make_unique<Rook>(Color::Black, Position{ 7, 0 }));

    // Tốt đen
    for (int x = 0; x < 8; ++x) {
        board.setPiece(1, x, "bPawn");
        board.setLogicPiece(1, x, std::make_unique<Pawn>(Color::Black, Position{ x, 1 }));
    }

    // Tốt trắng
    for (int x = 0; x < 8; ++x) {
        board.setPiece(6, x, "wPawn");
        board.setLogicPiece(6, x, std::make_unique<Pawn>(Color::White, Position{ x, 6 }));
    }

    // Quân trắng 
    board.setPiece(7, 0, "wRook");   board.setLogicPiece(7, 0, std::make_unique<Rook>(Color::White, Position{ 0, 7 }));
    board.setPiece(7, 1, "wKnight"); board.setLogicPiece(7, 1, std::make_unique<Knight>(Color::White, Position{ 1, 7 }));
    board.setPiece(7, 2, "wBishop"); board.setLogicPiece(7, 2, std::make_unique<Bishop>(Color::White, Position{ 2, 7 }));
    board.setPiece(7, 3, "wQueen");  board.setLogicPiece(7, 3, std::make_unique<Queen>(Color::White, Position{ 3, 7 }));
    board.setPiece(7, 4, "wKing");   board.setLogicPiece(7, 4, std::make_unique<King>(Color::White, Position{ 4, 7 }));
    board.setPiece(7, 5, "wBishop"); board.setLogicPiece(7, 5, std::make_unique<Bishop>(Color::White, Position{ 5, 7 }));
    board.setPiece(7, 6, "wKnight"); board.setLogicPiece(7, 6, std::make_unique<Knight>(Color::White, Position{ 6, 7 }));
    board.setPiece(7, 7, "wRook");   board.setLogicPiece(7, 7, std::make_unique<Rook>(Color::White, Position{ 7, 7 }));

    gameOver = false;
    gameResult = "";

    board.startGame(); 

    board.setRotation(false);

    if (dragHandler) {
    }

    std::cout << "Start new game ! " << std::endl;
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

void Game::setDifficulty(const std::string& difficulty) {
    this->difficulty = difficulty;
    int searchDepth = 4; // Mặc định là Medium

    if (difficulty == "Easy") {
        searchDepth = 2; // AI tìm kiếm ít bước hơn
    }
    else if (difficulty == "Medium") {
        searchDepth = 3; // Cân bằng
    }
    else if (difficulty == "Hard") {
        isStockfish = true; // AI tìm kiếm sâu hơn, thông minh hơn
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
    Move bestMove;
    if (isStockfish) {bestMove= stockFish.findBestMove(board, m_aiColor);}
    else {bestMove= m_chessBot.findBestMove(board, m_aiColor);}

    // 3. Thực hiện nước đi trên bàn cờ
    board.makeMove(bestMove);
    std::cout << "AI has moved." << std::endl;
}

// Hàm này trả về màu của AI để bạn có thể sử dụng trong logic game
Color Game::getAIColor() const {
    return m_aiColor; // Trả về màu của AI mà bạn đã định nghĩa trong Game.h
}

// === CÁC HÀM MỚI ĐỂ XỬ LÝ KẾT THÚC GAME ===

void Game::checkGameEndConditions() {
    Color currentPlayer = board.getCurrentTurn();

    // Kiểm tra chiếu hết (checkmate)
    if (board.is_check(currentPlayer)) {
        if (!hasValidMoves(currentPlayer)) {
            // Checkmate - người chơi hiện tại thua
            gameOver = true;
            if (currentPlayer == Color::White) {
                gameState = GameState::BlackWins;
                gameResult = "Black Wins!";
            }
            else {
                gameState = GameState::WhiteWins;
                gameResult = "White Wins!";
            }
            board.playSound("game-end");
            std::cout << gameResult << " (Checkmate)" << std::endl;
            return;
        }
    }

    // Kiểm tra chiếu bí (stalemate)
    if (!board.is_check(currentPlayer) && !hasValidMoves(currentPlayer)) {
        gameOver = true;
        gameState = GameState::Draw;
        gameResult = "Draw - Stalemate!";
        board.playSound("game-end");
        std::cout << gameResult << std::endl;
        return;
    }

    // Kiểm tra không đủ quân để chiếu hết
    if (isInsufficientMaterial()) {
        gameOver = true;
        gameState = GameState::Draw;
        gameResult = "Draw - Insufficient Material!";
        board.playSound("game-end");
        std::cout << gameResult << std::endl;
        return;
    }
}

bool Game::hasValidMoves(Color color) const {
    // Duyệt qua tất cả quân cờ của màu đó
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board.get_piece_at({ x, y });
            if (piece && piece->get_color() == color) {
                // Lấy tất cả nước đi có thể của quân này
                std::vector<Position> moves = piece->get_all_moves(board);

                // Kiểm tra xem có nước đi nào không khiến vua bị chiếu
                for (const Position& move : moves) {
                    // Tạo bản sao board để test
                    Board testBoard(board);
                    auto capturedPiece = testBoard.move_piece_for_ai({ x, y }, move);

                    // Nếu sau nước đi này vua không bị chiếu, thì có nước đi hợp lệ
                    if (!testBoard.is_check(color)) {
                        return true;
                    }

                    // Hoàn tác nước đi test
                    testBoard.undo_move_for_ai({ x, y }, move, std::move(capturedPiece));
                }
            }
        }
    }
    return false;
}

bool Game::isInsufficientMaterial() const {
    std::vector<const BasePiece*> whitePieces;
    std::vector<const BasePiece*> blackPieces;

    // Thu thập tất cả quân cờ còn lại
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board.get_piece_at({ x, y });
            if (piece) {
                if (piece->get_color() == Color::White) {
                    whitePieces.push_back(piece);
                }
                else {
                    blackPieces.push_back(piece);
                }
            }
        }
    }

    // Kiểm tra các trường hợp không đủ quân

    // Chỉ có 2 vua
    if (whitePieces.size() == 1 && blackPieces.size() == 1) {
        return true;
    }

    // Một bên chỉ có vua, bên kia có vua + tượng hoặc vua + mã
    if (whitePieces.size() == 1 && blackPieces.size() == 2) {
        for (const BasePiece* piece : blackPieces) {
            if (piece->get_pieceType() == PieceType::Bishop ||
                piece->get_pieceType() == PieceType::Knight) {
                return true;
            }
        }
    }

    if (blackPieces.size() == 1 && whitePieces.size() == 2) {
        for (const BasePiece* piece : whitePieces) {
            if (piece->get_pieceType() == PieceType::Bishop ||
                piece->get_pieceType() == PieceType::Knight) {
                return true;
            }
        }
    }

    // Cả hai bên chỉ có vua + tượng cùng màu ô
    if (whitePieces.size() == 2 && blackPieces.size() == 2) {
        const BasePiece* whiteBishop = nullptr;
        const BasePiece* blackBishop = nullptr;

        for (const BasePiece* piece : whitePieces) {
            if (piece->get_pieceType() == PieceType::Bishop) {
                whiteBishop = piece;
                break;
            }
        }

        for (const BasePiece* piece : blackPieces) {
            if (piece->get_pieceType() == PieceType::Bishop) {
                blackBishop = piece;
                break;
            }
        }

        if (whiteBishop && blackBishop) {
            Position whitePos = whiteBishop->get_pos();
            Position blackPos = blackBishop->get_pos();

            // Kiểm tra xem hai tượng có cùng màu ô không
            bool whiteBishopOnLightSquare = (whitePos.x + whitePos.y) % 2 == 0;
            bool blackBishopOnLightSquare = (blackPos.x + blackPos.y) % 2 == 0;

            if (whiteBishopOnLightSquare == blackBishopOnLightSquare) {
                return true;
            }
        }
    }

    return false;
}

std::string Game::getGameResult() const {
    return gameResult;
}

GameState Game::getGameState() const {
    return gameState;
}