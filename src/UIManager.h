#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>    // Thư viện đồ họa SFML
#include "Game.h"               // Bao gồm lớp Game để xử lý logic trò chơi

// Lớp quản lý toàn bộ giao diện người dùng của game
class UIManager {
private:
    sf::RenderWindow window;    // Cửa sổ hiển thị chính
    Game game;                  // Đối tượng điều khiển game

    // ===== TRẠNG THÁI UI =====
    enum class UIState {
        MainMenu,           // Trạng thái ở menu chính
        DifficultySelect,   // Chọn độ khó
        Playing,            // Trạng thái đang chơi
        GameOver            // Trạng thái kết thúc game
    };

    UIState currentState;       // Trạng thái hiện tại của giao diện

    // ===== CẤU TRÚC NÚT BẤM =====
    struct Button {
        sf::RectangleShape shape;   // Hình dạng hình chữ nhật của nút
        sf::Text text;              // Văn bản hiển thị trên nút
    };

    // ===== THÀNH PHẦN GIAO DIỆN =====
    sf::Texture backgroundTexture;  // Hình nền menu chính
    sf::Sprite backgroundSprite;    // Sprite để vẽ hình nền
    sf::Font font;                  // Font chữ dùng cho văn bản giao diện

    // ===== NÚT GIAO DIỆN =====
    Button pvpButton; // Nút Player vs Player
    Button pvbButton; // Nút Player vs Bot

    Button startButton;     // Nút bắt đầu game
    Button quitButton;      // Nút thoát game
    std::vector<Button> difficultyButtons; // Nút chọn độ khó

    // ===== NÚT GAME OVER =====
    Button playAgainButton;     // Nút chơi lại
    Button mainMenuButton;      // Nút về menu chính

    // ===== VĂN BẢN HIỂN THỊ =====
    sf::Text statusText;    // Hiển thị thông báo trạng thái (VD: Game Over)
    sf::Text titleText;     // Hiển thị tiêu đề game (VD: CHESS GAME)
    sf::Text gameOverText;  // Hiển thị kết quả trận đấu

    // ===== MÀU SẮC NÚT =====
    sf::Color normalColor;  // Màu bình thường
    sf::Color hoverColor;   // Màu khi di chuột qua
    sf::Color pressedColor; // Màu khi nhấn chuột

    // ===== BIẾN TRẠNG THÁI GAME =====
    std::string gameResult; // Lưu kết quả game ("White Wins", "Black Wins", "Draw")

public:
    // ===== HÀM KHỞI TẠO =====
    UIManager();            // Constructor

    // ===== VÒNG LẶP CHÍNH =====
    void run();             // Hàm chạy chính của giao diện

    // ===== HÀM XỬ LÝ KẾT THÚC GAME =====
    void showGameOver(const std::string& result);

private:

    void initUI();          // Thiết lập giao diện và vị trí nút

    bool loadAssets();      // Tải hình ảnh, font chữ,...

    void updateButtonStates();   // Cập nhật màu nút theo vị trí chuột

    void drawUI();               // Vẽ toàn bộ giao diện lên cửa sổ

    bool isButtonClicked(const Button& button, const sf::Event& event); // Kiểm tra nút có bị nhấn

    void handleEvents();                                                // Xử lý sự kiện 

    void initGameOverUI();  // Khởi tạo giao diện game over
};

#endif