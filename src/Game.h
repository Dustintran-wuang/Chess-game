#ifndef GAME_H
#define GAME_H

//#include "ChessBoard.h" sử dụng logic kiểm tra để biết GameOver chưa
#include "ChessBoard.h"
#include "AIEngine.h"
#include "DragHandler.h"
#include "BasePiece.h"
#include <SFML/Graphics.hpp>
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

class Game {
private:
    Board board;
    bool gameOver;

    ChessBot m_chessBot;
	Color m_aiColor = Color::Black; // Màu của AI, mặc định là đen

    DragHandler* dragHandler;

    std::string difficulty; //Biến lưu độ khó (Easy/Medium/Hard)
    GameMode currentMode;

    
public:
    Game();                                         // Hàm khởi tạo
    ~Game();
    void startNewGame();                            // Bắt đầu ván mới
    void update();                                  // Cập nhật logic
    void render(sf::RenderWindow& window);          // Vẽ bàn cờ lên cửa sổ
    bool isGameOver() const;                        // Kiểm tra trạng thái kết thúc
    void Game::handleInput(const sf::Event& event, sf::RenderWindow& window);       // Xử lý input người chơi

    void setDifficulty(const std::string& diff);
    std::string getDifficulty() const;

    void setGameMode(GameMode mode);
    GameMode getGameMode() const;

	void makeAIMove();          // Kích hoạt AI để thực hiện nước đi
	Color getAIColor() const;   // Lấy màu của AI (để kiểm tra lượt đi)
};

#endif
