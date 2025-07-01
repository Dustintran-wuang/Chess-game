#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "Game.h"

class UIManager {
private:
    sf::RenderWindow window;    // Cửa sổ hiển thị
    Game game;                  // Đối tượng game quản lý logic
    
    // Cấu trúc nút UI
    struct Button {
        sf::RectangleShape shape;   // Hình dạng nút
        sf::Text text;              // Chữ trên nút
    };
    
    // Thành phần UI
    sf::Texture backgroundTexture;  // Texture nền
    sf::Sprite backgroundSprite;    // Sprite hiển thị nền
    sf::Font font;                  // Font chữ
    
    // Các nút chính
    Button startButton;     // Nút bắt đầu
    Button restartButton;   // Nút chơi lại
    Button quitButton;      // Nút thoát
    
    // Các text hiển thị
    sf::Text statusText;    // Hiển thị trạng thái
    sf::Text titleText;     // Tiêu đề game
    
    // Màu sắc UI
    sf::Color normalColor;  // Màu bình thường
    sf::Color hoverColor;   // Màu khi di chuột qua
    sf::Color pressedColor; // Màu khi nhấn

public:
    // Constructor
    UIManager();
    
    // Chạy vòng lặp game chính
    void run();
    
private:
    /* ===== CÁC HÀM KHỞI TẠO ===== */
    // Khởi tạo giao diện
    void initUI();
    
    // Tải tài nguyên (ảnh, font)
    bool loadAssets();
    
    /* ===== CÁC HÀM XỬ LÝ UI ===== */
    // Cập nhật trạng thái các nút
    void updateButtonStates();
    
    // Vẽ toàn bộ giao diện
    void drawUI();
    
    // Kiểm tra nút có được nhấn không
    bool isButtonClicked(const Button& button, const sf::Event& event);
    
    // Xử lý sự kiện input
    void handleEvents();
};

#endif // UIMANAGER_H
