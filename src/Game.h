#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>

// Enum các trạng thái của game
enum class GameState {
    MainMenu,    // Đang ở menu chính
    Playing,     // Đang chơi game
    GameOver     // Game kết thúc
};

class Game {
private:
    Board board;                // Bàn cờ
    GameState currentState;     // Trạng thái hiện tại
    Color currentTurn;          // Lượt đi hiện tại (Trắng/Đen)
    bool gameOver;              // Cờ kiểm tra game kết thúc

public:
    Game();
    
    /* ===== CÁC HÀM QUẢN LÝ GAME ===== */
    // Bắt đầu game mới
    void startNewGame();
    
    // Chơi lại từ đầu
    void restartGame();
    
    // Kết thúc game với thông báo
    void endGame(const std::string& result);
    
    // Xử lý nước đi từ vị trí 'from' đến 'to'
    void handleMove(const Position& from, const Position& to);
    
    /* ===== CÁC HÀM TRUY VẤN TRẠNG THÁI ===== */
    // Lấy trạng thái hiện tại
    GameState getCurrentState() const;
    
    // Lấy lượt đi hiện tại
    Color getCurrentTurn() const;
    
    // Kiểm tra game đã kết thúc chưa
    bool isGameOver() const;
    
    /* ===== TRUY CẬP BÀN CỜ ===== */
    Board& getBoard();
};

#endif // GAME_H
