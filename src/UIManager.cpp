#include "UIManager.h"
#include <iostream>
using namespace std;

// ===== HÀM KHỞI TẠO UI =====
UIManager::UIManager()
    : window(sf::VideoMode(800, 600), "Chess Game", sf::Style::Titlebar | sf::Style::Close), currentState(UIState::MainMenu) {

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
        handleEvents();

        window.clear();

        if (currentState == UIState::MainMenu || currentState == UIState::DifficultySelect) {
            updateButtonStates();
            drawUI();
        }
        else if (currentState == UIState::Playing) {
            game.update();
            game.render(window);  // Game chỉ vẽ, không clear/display
        }

        window.display();
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

	// Nút chọn độ khó (8 nút)
    for (int i = 0; i < 8; ++i) {
        Button btn;
        btn.shape.setSize({ 150, 40 });
        btn.shape.setFillColor(normalColor);
        btn.shape.setPosition(100 + (i % 4) * 170, 400 + (i / 4) * 60);

        btn.text.setFont(font);
        btn.text.setCharacterSize(20);
        btn.text.setFillColor(sf::Color::White);
        btn.text.setString("Level " + std::to_string(i + 1));
        btn.text.setPosition(btn.shape.getPosition().x + 30, btn.shape.getPosition().y + 5);

        difficultyButtons.push_back(btn);
    }

    // Text trạng thái (ví dụ: "Game Over!")
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50, 500);
}

// ===== CẬP NHẬT MÀU CỦA NÚT =====
void UIManager::updateButtonStates() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (currentState == UIState::MainMenu) {
        if (startButton.shape.getGlobalBounds().contains(mousePos))
            startButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            startButton.shape.setFillColor(normalColor);

        if (quitButton.shape.getGlobalBounds().contains(mousePos))
            quitButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            quitButton.shape.setFillColor(normalColor);
    }
    else if (currentState == UIState::DifficultySelect) {
        for (auto& btn : difficultyButtons) {
            if (btn.shape.getGlobalBounds().contains(mousePos))
                btn.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
            else
                btn.shape.setFillColor(normalColor);
        }
    }
}


// ===== VẼ TOÀN BỘ GIAO DIỆN MENU =====
void UIManager::drawUI() {
    window.draw(backgroundSprite);

    if (currentState == UIState::MainMenu) {
        window.draw(titleText);
        window.draw(startButton.shape);
        window.draw(startButton.text);
        window.draw(quitButton.shape);
        window.draw(quitButton.text);
    }
    else if (currentState == UIState::DifficultySelect) {
        for (const auto& btn : difficultyButtons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }
    }

    window.draw(statusText);
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
        if (event.type == sf::Event::Closed)
            window.close();             // Người dùng bấm nút X

        if (currentState == UIState::MainMenu) {
            if (isButtonClicked(startButton, event)) {
                currentState = UIState::DifficultySelect;           // Chuyển sang chọn độ khó
                statusText.setString("Select Difficulty");
            }
            else if (isButtonClicked(quitButton, event)) {
                window.close();
            }
        }
        // ======= CHỌN ĐỘ KHÓ =======
        else if (currentState == UIState::DifficultySelect) {           
            for (int i = 0; i < difficultyButtons.size(); ++i) {
                if (isButtonClicked(difficultyButtons[i], event)) {
                    int level = i + 1;
                    game.setDifficulty(level);                      // Gán độ khó cho AI
                    game.startNewGame();                            // Bắt đầu ván mới
                    currentState = UIState::Playing;
                    statusText.setString("Difficulty: " + std::to_string(level));
                    break;
                }
            }
        }

        // ======= ĐANG CHƠI GAME =======
        else if (currentState == UIState::Playing) {
            game.handleInput(event, window);  // Truyền sự kiện cho bàn cờ
        }
    }
}

