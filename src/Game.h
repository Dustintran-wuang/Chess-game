#ifndef GAME_H
#define GAME_H

//#include "ChessBoard.h" sử dụng logic kiểm tra để biết GameOver chưa
#include "ChessBoard.h"
#include "AIEngine.cpp"
#include "DragHandler.h"
#include "BasePiece.h"
#include <SFML/Graphics.hpp>
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

class Game {
private:
    Board board;
    bool gameOver;
    ChessBot bot;
    DragHandler dragHandler;
public:
    Game();                                         // Hàm khởi tạo
    void startNewGame();                            // Bắt đầu ván mới
    void update();                                  // Cập nhật logic
    void render(sf::RenderWindow& window);          // Vẽ bàn cờ lên cửa sổ
    bool isGameOver() const;                        // Kiểm tra trạng thái kết thúc
    void Game::handleInput(const sf::Event& event, sf::RenderWindow& window);       // Xử lý input người chơi
    void setDifficulty(int depth);                  // Thiết lập độ khó cho AI 
};

#endif
