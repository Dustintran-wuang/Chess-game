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
    initGameOverUI();
}

// ===== VÒNG LẶP CHÍNH CỦA GIAO DIỆN =====
void UIManager::run() {
    while (window.isOpen()) {
        // Kiểm tra trạng thái game để chuyển sang màn hình game over
        if (currentState == UIState::Playing && game.isGameOver()) {
            gameResult = game.getGameResult();
            showGameOver(gameResult);
        }

        handleEvents();

        window.clear();

        if (currentState == UIState::MainMenu || currentState == UIState::DifficultySelect || currentState == UIState::GameOver) {
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

    // Nút PvP
    pvpButton.shape.setSize({ 200, 50 });
    pvpButton.shape.setPosition(300, 180);
    pvpButton.shape.setFillColor(normalColor);
    pvpButton.text.setFont(font);
    pvpButton.text.setCharacterSize(24);
    pvpButton.text.setFillColor(sf::Color::White);
    pvpButton.text.setString("Player vs Player");
    pvpButton.text.setPosition(310, 190);

    // Nút PvBot
    pvbButton.shape.setSize({ 200, 50 });
    pvbButton.shape.setPosition(300, 260);
    pvbButton.shape.setFillColor(normalColor);
    pvbButton.text.setFont(font);
    pvbButton.text.setCharacterSize(24);
    pvbButton.text.setFillColor(sf::Color::White);
    pvbButton.text.setString("Player vs Bot");
    pvbButton.text.setPosition(320, 270);

    // Nút Quit lùi xuống một chút
    quitButton.shape.setPosition(300, 340);
    quitButton.text.setPosition(370, 350);

    // Nút chọn độ khó (3 nút)
    vector<string> labels = { "Easy", "Medium", "Hard" };

    for (int i = 0; i < 3; ++i) {
        Button btn;
        btn.shape.setSize({ 200, 50 });
        btn.shape.setFillColor(normalColor);
        btn.shape.setPosition(300, 200 + i * 80);

        btn.text.setFont(font);
        btn.text.setCharacterSize(24);
        btn.text.setFillColor(sf::Color::White);
        btn.text.setString(labels[i]);
        btn.text.setPosition(
            btn.shape.getPosition().x + 50,
            btn.shape.getPosition().y + 10
        );

        difficultyButtons.push_back(btn);
    }

    // Text trạng thái (ví dụ: "Game Over!")
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50, 500);
}

// ===== KHỞI TẠO GIAO DIỆN GAME OVER =====
void UIManager::initGameOverUI() {
    // Text hiển thị kết quả
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setPosition(200, 150);

    // Nút Play Again
    playAgainButton.shape.setSize({ 200, 50 });
    playAgainButton.shape.setPosition(200, 300);
    playAgainButton.shape.setFillColor(normalColor);
    playAgainButton.text.setString("Play Again");
    playAgainButton.text.setFont(font);
    playAgainButton.text.setCharacterSize(24);
    playAgainButton.text.setFillColor(sf::Color::White);
    playAgainButton.text.setPosition(230, 310);

    // Nút Main Menu
    mainMenuButton.shape.setSize({ 200, 50 });
    mainMenuButton.shape.setPosition(450, 300);
    mainMenuButton.shape.setFillColor(normalColor);
    mainMenuButton.text.setString("Main Menu");
    mainMenuButton.text.setFont(font);
    mainMenuButton.text.setCharacterSize(24);
    mainMenuButton.text.setFillColor(sf::Color::White);
    mainMenuButton.text.setPosition(485, 310);
}

// ===== HIỂN THỊ MÀN HÌNH GAME OVER =====
void UIManager::showGameOver(const std::string& result) {
    gameResult = result;
    gameOverText.setString(result);
    
    // Căn giữa text dựa trên độ dài
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    gameOverText.setPosition(400, 200);
    
    currentState = UIState::GameOver;
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

        if (pvpButton.shape.getGlobalBounds().contains(mousePos))
            pvpButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            pvpButton.shape.setFillColor(normalColor);

        if (pvbButton.shape.getGlobalBounds().contains(mousePos))
            pvbButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            pvbButton.shape.setFillColor(normalColor);
    }
    else if (currentState == UIState::DifficultySelect) {
        for (auto& btn : difficultyButtons) {
            if (btn.shape.getGlobalBounds().contains(mousePos))
                btn.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
            else
                btn.shape.setFillColor(normalColor);
        }
    }
    else if (currentState == UIState::GameOver) {
        if (playAgainButton.shape.getGlobalBounds().contains(mousePos))
            playAgainButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            playAgainButton.shape.setFillColor(normalColor);

        if (mainMenuButton.shape.getGlobalBounds().contains(mousePos))
            mainMenuButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
        else
            mainMenuButton.shape.setFillColor(normalColor);
    }
}

// ===== VẼ TOÀN BỘ GIAO DIỆN MENU =====
void UIManager::drawUI() {
    window.draw(backgroundSprite);

    if (currentState == UIState::MainMenu) {
        window.draw(titleText);
        window.draw(pvpButton.shape);
        window.draw(pvpButton.text);
        window.draw(pvbButton.shape);
        window.draw(pvbButton.text);
        window.draw(quitButton.shape);
        window.draw(quitButton.text);
    }
    else if (currentState == UIState::DifficultySelect) {
        for (const auto& btn : difficultyButtons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }
    }
    else if (currentState == UIState::GameOver) {
        window.draw(gameOverText);
        window.draw(playAgainButton.shape);
        window.draw(playAgainButton.text);
        window.draw(mainMenuButton.shape);
        window.draw(mainMenuButton.text);
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
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Xử lý khi ở Menu chính
        if (currentState == UIState::MainMenu) {
            if (isButtonClicked(pvpButton, event)) {
                game.setGameMode(GameMode::PlayerVsPlayer);
                game.startNewGame();
                currentState = UIState::Playing;
            }
            else if (isButtonClicked(pvbButton, event)) {
                currentState = UIState::DifficultySelect;
            }
            else if (isButtonClicked(quitButton, event)) {
                window.close();
            }
        }
        // Chọn độ khó
        else if (currentState == UIState::DifficultySelect) {
            for (int i = 0; i < difficultyButtons.size(); ++i) {
                if (isButtonClicked(difficultyButtons[i], event)) {
                    string difficulty = difficultyButtons[i].text.getString();

                    game.setDifficulty(difficulty);
                    game.startNewGame();
                    game.setGameMode(GameMode::PlayerVsBot);

                    currentState = UIState::Playing;
                    statusText.setString("Difficulty: " + difficulty);
                    break;
                }
            }
        }
        // Xử lý khi đang chơi game
        else if (currentState == UIState::Playing) {
            game.handleInput(event, window);
        }
        // Xử lý màn hình game over
        else if (currentState == UIState::GameOver) {
            if (isButtonClicked(playAgainButton, event)) {
                // Chơi lại với cùng cài đặt
                game.startNewGame();
                currentState = UIState::Playing;
            }
            else if (isButtonClicked(mainMenuButton, event)) {
                // Về menu chính
                currentState = UIState::MainMenu;
            }
        }
    }
}