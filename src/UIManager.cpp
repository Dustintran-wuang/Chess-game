#include "UIManager.h"
#include <iostream>
using namespace std;

// ===== HÀM KHỞI TẠO UI =====
UIManager::UIManager()
    : window(sf::VideoMode(800, 600), "Chess Game"), currentState(UIState::MainMenu) {

    // Gán màu cho các trạng thái nút
    normalColor = sf::Color(70, 70, 70);
    hoverColor = sf::Color(100, 100, 100);
    pressedColor = sf::Color(50, 50, 50);

    // Nếu tải tài nguyên thất bại → in lỗi và đóng cửa sổ
    if (!loadAssets()) {
        cout << "Failed to load assets!" << endl;
        window.close();
    }

    // Thiết lập các thành phần giao diện
    initUI();
}

// ===== VÒNG LẶP CHÍNH CỦA GIAO DIỆN =====
void UIManager::run() {
    while (window.isOpen()) {
        handleEvents();  // Xử lý sự kiện (chuột, bàn phím,...)

        // Nếu đang ở menu chính
        if (currentState == UIState::MainMenu) {
            updateButtonStates();  // Cập nhật màu nút theo chuột
            drawUI();              // Vẽ giao diện menu
        }
        // Nếu đang chơi game
        else if (currentState == UIState::Playing) {
            game.update();              // Cập nhật logic game
            game.render(window);        // Vẽ bàn cờ

            // Nếu game kết thúc → quay lại menu
            if (game.isGameOver()) {
                currentState = UIState::MainMenu;
                statusText.setString("Game Over! Back to menu.");
            }
        }
    }
}

// ===== TẢI FONT VÀ HÌNH NỀN =====
bool UIManager::loadAssets() {
    if (!font.loadFromFile("assets/Fonts/arial.ttf")) return false;
    if (!backgroundTexture.loadFromFile("assets/images/menu_bg.jpg")) return false;
    backgroundSprite.setTexture(backgroundTexture);
    return true;
}

// ===== KHỞI TẠO GIAO DIỆN (MENU) =====
void UIManager::initUI() {
    // Tiêu đề
    titleText.setString("CHESS GAME");
    titleText.setFont(font);
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(250, 50);

    // Nút Start
    startButton.shape.setSize({ 200, 50 });
    startButton.shape.setPosition(300, 200);
    startButton.shape.setFillColor(normalColor);
    startButton.text.setString("Start Game");
    startButton.text.setFont(font);
    startButton.text.setCharacterSize(24);
    startButton.text.setFillColor(sf::Color::White);
    startButton.text.setPosition(330, 210);

    // Nút Quit
    quitButton.shape.setSize({ 200, 50 });
    quitButton.shape.setPosition(300, 300);
    quitButton.shape.setFillColor(normalColor);
    quitButton.text.setString("Quit");
    quitButton.text.setFont(font);
    quitButton.text.setCharacterSize(24);
    quitButton.text.setFillColor(sf::Color::White);
    quitButton.text.setPosition(370, 310);

    // Text trạng thái (ví dụ: "Game Over!")
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50, 500);
}

// ===== CẬP NHẬT MÀU CỦA NÚT =====
void UIManager::updateButtonStates() {
    // Lấy vị trí chuột trong cửa sổ
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Kiểm tra nút Start
    if (startButton.shape.getGlobalBounds().contains(mousePos))
        startButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
    else
        startButton.shape.setFillColor(normalColor);

    // Kiểm tra nút Quit
    if (quitButton.shape.getGlobalBounds().contains(mousePos))
        quitButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
    else
        quitButton.shape.setFillColor(normalColor);
}

// ===== VẼ TOÀN BỘ GIAO DIỆN MENU =====
void UIManager::drawUI() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(startButton.shape);
    window.draw(startButton.text);
    window.draw(quitButton.shape);
    window.draw(quitButton.text);
    window.draw(statusText);
    window.display();
}

// ===== KIỂM TRA NÚT CÓ BỊ CLICK KHÔNG =====
bool UIManager::isButtonClicked(const Button& button, const sf::Event& event) {
    return event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left &&
        button.shape.getGlobalBounds().contains(
            window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }));
}

// ===== XỬ LÝ SỰ KIỆN (PHÍM, CHUỘT) =====
void UIManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();  // Người dùng bấm nút X
        }

        // Nếu đang ở menu
        if (currentState == UIState::MainMenu) {
            if (isButtonClicked(startButton, event)) {
                game.startNewGame();      // Bắt đầu ván mới
                currentState = UIState::Playing;
                statusText.setString(""); // Xóa thông báo cũ
            }
            else if (isButtonClicked(quitButton, event)) {
                window.close();          // Thoát game
            }
        }
        // Nếu đang chơi game → chuyển sự kiện cho game xử lý
        else if (currentState == UIState::Playing) {
            game.handleInput(event);     // Gửi sự kiện vào bàn cờ
        }
    }
}
