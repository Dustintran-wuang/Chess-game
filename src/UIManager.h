#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class UIManager {
private:
    sf::Font font;                                                      // Font chữ 
    sf::Text timerText;                                                 // Hiển thị đồng hồ thời gian
    sf::Clock clock;                                                    // Đo thời gian đã trôi qua
    sf::Time elapsedTime;                                               // Thời gian lưu được từ clock

    // Cấu trúc nút giao diện: gồm hình chữ nhật và chữ
    struct Button {
        sf::RectangleShape shape;                                       // Hình dạng của nút
        sf::Text label;                                                 // Nhãn nút (vd: "Start")
    };

    std::vector<Button> buttons;                                        // Danh sách các nút UI: Start, Restart, Quit, Difficulty

public:
    UIManager();

    void loadUI();                                                      // Load font, tạo nút, setup text
    void updateTimer();                                                 // Cập nhật thời gian vào `timerText`
    void draw(sf::RenderWindow& window);                                // Vẽ tất cả UI ra màn hình
    void setStatus(const std::string& text);                            // Đổi dòng trạng thái đồng hồ
    int isClicked(sf::Vector2f mousePos);                               // Trả về index nút được click (hoặc -1)
};

#endif 
