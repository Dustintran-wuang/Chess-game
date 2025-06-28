#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>

class Board {
private:
    // --- Texture và Sprite ---
    sf::Texture boardTexture;                          // Texture bàn cờ
    sf::Sprite boardSprite;                            // Sprite bàn cờ

    std::map<std::string, sf::Texture> pieceTextures;  // Texture cho các quân cờ
    sf::Sprite pieces[8][8];                           // Sprite quân cờ trên bàn
    std::string pieceNames[8][8];                      // Tên quân cờ tại mỗi ô

    // --- Highlight ---
    std::vector<sf::RectangleShape> highlights;        // Các ô được highlight

    // --- Âm thanh ---
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;

public:
    // Load toàn bộ tài nguyên (ảnh + âm thanh)
    bool loadAssets();

    // Vẽ bàn cờ, quân cờ, và các highlight
    void draw(sf::RenderWindow& window);

    // Đặt quân cờ (theo tên) tại một ô cụ thể
    void setPiece(int row, int col, const std::string& name);

    // Lấy tên quân cờ tại một ô cụ thể
    std::string getPieceName(int row, int col) const;

    // Tô sáng các nước đi hợp lệ (Phần này làm chung với logic)
    void highlightValidMove(const std::vector<sf::Vector2i>& moves); 

    // Xóa toàn bộ highlight
    void clearHighlightMove();

	// Phát âm thanh theo tên (Làm chung với logic, VD: castle) (bổ sung cho movePiece)
    void playSound(const std::string& name);

	// Phát âm thanh khi di chuyển (Làm chung với dragHandler)) 
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
};

#endif
