#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "Board.h"

class UIManager {
private:
    // Cửa sổ và thành phần chính của game
    sf::RenderWindow window;  // Cửa sổ game chính
    Board board;              // Bàn cờ

    // Thành phần nền
    sf::Texture backgroundTexture;  // Chứa dữ liệu hình nền
    sf::Sprite backgroundSprite;    // Dùng để hiển thị nền

    // Các nút UI
    sf::RectangleShape startButton;   // Nút bắt đầu game mới
    sf::RectangleShape restartButton; // Nút chơi lại
    sf::RectangleShape quitButton;    // Nút thoát game

    // Thành phần chữ
    sf::Font font;            // Font chữ chính
    sf::Text startText;       // Chữ trên nút bắt đầu
    sf::Text restartText;     // Chữ trên nút chơi lại
    sf::Text quitText;        // Chữ trên nút thoát
    sf::Text statusText;      // Hiển thị trạng thái game
    sf::Text titleText;       // Tiêu đề game

    // Màu sắc
    sf::Color normalColor = sf::Color(70, 70, 70, 180);    // Màu nút bình thường (có độ trong suốt)
    sf::Color hoverColor = sf::Color(150, 150, 150, 200);  // Màu khi di chuột qua
    sf::Color pressedColor = sf::Color(20, 20, 20, 220);   // Màu khi nhấn nút
    sf::Color textColor = sf::Color::White;                // Màu chữ mặc định

    // Quản lý trạng thái game
    enum class GameState {
        MainMenu,  // Màn hình chính
        Playing,   // Đang chơi
        GameOver   // Kết thúc game
    };

    GameState currentState = GameState::MainMenu;  // Theo dõi trạng thái hiện tại
    Color currentTurn = Color::White;              // Lượt đi hiện tại
    bool gameOver = false;                         // Cờ kết thúc game

public:
    // Hàm khởi tạo và vòng lặp game
    UIManager();
    void runGameLoop();

private:
    // Khởi tạo và hiển thị UI
    void initUI();                      // Thiết lập các thành phần UI
    bool loadAssets();                  // Tải font và hình ảnh
    void updateButtonStates();          // Cập nhật trạng thái nút
    void drawUI();                      // Vẽ tất cả thành phần UI
    bool isButtonClicked(const sf::RectangleShape& button, const sf::Event& event); // Kiểm tra nhấn nút

    // Logic game
    void handleGameEvents();            // Xử lý sự kiện
    void updateGameState();             // Cập nhật trạng thái game
    void renderGame();                  // Hiển thị game
    void startNewGame();                // Bắt đầu game mới
    void restartCurrentGame();          // Chơi lại game
    void endCurrentGame(const std::string& result); // Kết thúc game với thông báo
};

#endif // UIMANAGER_H