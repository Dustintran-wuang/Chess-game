#ifndef GAME_H
#define GAME_H

#include "ChessBoard.h"
#include "AIEngine.h"
#include "DragHandler.h"
#include "BasePiece.h"
#include <SFML/Graphics.hpp>
#include "StockfishAI.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

enum class GameMode {
    PlayerVsPlayer,
    PlayerVsBot
};

enum class GameState {
    Playing,
    WhiteWins,
    BlackWins,
    Draw
};

class Game {
private:
    Board board;
    bool gameOver;
    GameState gameState;
    std::string gameResult; // Lưu kết quả game dạng string

    ChessBot m_chessBot;
    StockfishAI stockFish;
    Color m_aiColor = Color::Black; // Màu của AI, mặc định là đen

    DragHandler* dragHandler;

    bool isStockfish = false;

    std::string difficulty; // Biến lưu độ khó (Easy/Medium/Hard)
    GameMode currentMode;

    // Hàm kiểm tra các điều kiện kết thúc game
    void checkGameEndConditions();
    bool isStalemate(Color color) const;
    bool isInsufficientMaterial() const;
    bool hasValidMoves(Color color) const;

public:
    Game();                                         // Hàm khởi tạo
    ~Game();
    void startNewGame();                            // Bắt đầu ván mới
    void update();                                  // Cập nhật logic
    void render(sf::RenderWindow& window);          // Vẽ bàn cờ lên cửa sổ
    bool isGameOver() const;                        // Kiểm tra trạng thái kết thúc
    void handleInput(const sf::Event& event, sf::RenderWindow& window);       // Xử lý input người chơi

    void setDifficulty(const std::string& diff);
    std::string getDifficulty() const;

    void setGameMode(GameMode mode);
    GameMode getGameMode() const;

    void makeAIMove();          // Kích hoạt AI để thực hiện nước đi
    Color getAIColor() const;   // Lấy màu của AI (để kiểm tra lượt đi)

    // Hàm mới để lấy kết quả game
    std::string getGameResult() const;
    GameState getGameState() const;
};

#endif