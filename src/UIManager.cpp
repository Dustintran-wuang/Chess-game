#include "UIManager.h"
#include <iostream>
using namespace std;

UIManager::UIManager() : window(sf::VideoMode(800, 600), "Chess Game") {
    // Khởi tạo màu sắc
    normalColor = sf::Color(70, 70, 70);    	//Màu nút khi không bị tác động
	hoverColor = sf::Color(100, 100, 100);  // Màu nút khi rê chuột qua
	pressedColor = sf::Color(50, 50, 50);   // Màu nút khi nhấn chuột

    // Khởi tạo UI
    if (!loadAssets()) {
        cout << "Failed to load assets!" << endl;
        window.close();
    }
    initUI();
}

void UIManager::run() {
    while (window.isOpen()) {
        handleEvents();
        updateButtonStates();
        drawUI();
    }
}

bool UIManager::loadAssets() {
    // Tải font chữ
    if (!font.loadFromFile("assets/Fonts/arial.ttf")) {
        cout << "Failed to load font!" << endl;
        return false;
    }

    // Tải background
    if (!backgroundTexture.loadFromFile("assets/images/menu_bg.jpg")) {
        cout << "Failed to load background!" << endl;
        return false;
    }
    backgroundSprite.setTexture(backgroundTexture);

    return true;
}

void UIManager::initUI() {
    // Khởi tạo tiêu đề
    titleText.setString("CHESS GAME");
    titleText.setFont(font);
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(250, 50);

    // Khởi tạo nút Start
    startButton.shape.setSize(sf::Vector2f(200, 50));
    startButton.shape.setPosition(300, 200);
    startButton.shape.setFillColor(normalColor);

    startButton.text.setString("Start Game");
    startButton.text.setFont(font);
    startButton.text.setCharacterSize(24);
    startButton.text.setFillColor(sf::Color::White);
    startButton.text.setPosition(330, 210);

    // Khởi tạo nút Restart
    restartButton.shape.setSize(sf::Vector2f(200, 50));
    restartButton.shape.setPosition(300, 300);
    restartButton.shape.setFillColor(normalColor);

    restartButton.text.setString("Restart");
    restartButton.text.setFont(font);
    restartButton.text.setCharacterSize(24);
    restartButton.text.setFillColor(sf::Color::White);
    restartButton.text.setPosition(350, 310);

    // Khởi tạo nút Quit
    quitButton.shape.setSize(sf::Vector2f(200, 50));
    quitButton.shape.setPosition(300, 400);
    quitButton.shape.setFillColor(normalColor);

    quitButton.text.setString("Quit");
    quitButton.text.setFont(font);
    quitButton.text.setCharacterSize(24);
    quitButton.text.setFillColor(sf::Color::White);
    quitButton.text.setPosition(370, 410);

    // Khởi tạo text trạng thái
    statusText.setString("");
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50, 500);
}

void UIManager::updateButtonStates() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Cập nhật trạng thái nút Start
    if (startButton.shape.getGlobalBounds().contains(mousePos)) {
        startButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
    }
    else {
        startButton.shape.setFillColor(normalColor);
    }

    // Cập nhật trạng thái nút Restart
    if (restartButton.shape.getGlobalBounds().contains(mousePos)) {
        restartButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
    }
    else {
        restartButton.shape.setFillColor(normalColor);
    }

    // Cập nhật trạng thái nút Quit
    if (quitButton.shape.getGlobalBounds().contains(mousePos)) {
        quitButton.shape.setFillColor(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? pressedColor : hoverColor);
    }
    else {
        quitButton.shape.setFillColor(normalColor);
    }
}

void UIManager::drawUI() {
    window.clear();

    // Vẽ background
    window.draw(backgroundSprite);

    // Vẽ tiêu đề
    window.draw(titleText);

    // Vẽ các nút
    window.draw(startButton.shape);
    window.draw(startButton.text);

    window.draw(restartButton.shape);
    window.draw(restartButton.text);

    window.draw(quitButton.shape);
    window.draw(quitButton.text);

    // Vẽ trạng thái
    window.draw(statusText);

    window.display();
}

// Kiểm tra xem nút có được nhấn hay không
bool UIManager::isButtonClicked(const Button& button, const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        return button.shape.getGlobalBounds().contains(
            window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))
        );
    }
    return false;
}

//Xử lý các nút chức năng
void UIManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        /*=============Xử lý click nút=============*/

        //Start
        if (isButtonClicked(startButton, event)) {
            game.startNewGame();
            statusText.setString("Game started!");
        }
	
	// Quit
        else if (isButtonClicked(quitButton, event)) {
            window.close();
        }
    }
}


