#include "Game.h"

Game::Game() : gameOver(false) {
    board.loadAssets();
}

void Game::update() {
    // Logic kiểm tra thắng thua ở đây
    // Ví dụ đơn giản:
    if (/* ví dụ */ false) {
        gameOver = true;
    }
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    board.draw(window);
    window.display();
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::handleInput(const sf::Event& event) {
    // Xử lý nhập chuột, kéo thả quân, v.v.
}

void Game::startNewGame() {
    board.startGame();     // Phát âm thanh bắt đầu
    gameOver = false;

    // Trắng
    for (int i = 0; i < 8; ++i)
        board.setPiece(6, i, "wPawn");
    board.setPiece(7, 0, "wRook");
    board.setPiece(7, 1, "wKnight");
    board.setPiece(7, 2, "wBishop");
    board.setPiece(7, 3, "wQueen");
    board.setPiece(7, 4, "wKing");
    board.setPiece(7, 5, "wBishop");
    board.setPiece(7, 6, "wKnight");
    board.setPiece(7, 7, "wRook");

    // Đen
    for (int i = 0; i < 8; ++i)
        board.setPiece(1, i, "bPawn");
    board.setPiece(0, 0, "bRook");
    board.setPiece(0, 1, "bKnight");
    board.setPiece(0, 2, "bBishop");
    board.setPiece(0, 3, "bQueen");
    board.setPiece(0, 4, "bKing");
    board.setPiece(0, 5, "bBishop");
    board.setPiece(0, 6, "bKnight");
    board.setPiece(0, 7, "bRook");
}

void Game::setDifficulty(int depth) {
    bot.setDifficulty(depth);  // Gọi xuống lớp ChessBot
}
