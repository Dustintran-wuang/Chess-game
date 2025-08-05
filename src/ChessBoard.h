#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>
#include "BasePiece.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"

class Board {
private:
    // --- Texture và Sprite ---
    sf::Texture boardTexture;                          // Texture bàn cờ
    sf::Sprite boardSprite;                            // Sprite bàn cờ

    std::map<std::string, sf::Texture> pieceTextures;  // Texture cho các quân cờ
    sf::Sprite pieces[8][8];                           // Sprite quân cờ trên bàn
    std::string pieceNames[8][8];                      // Tên quân cờ tại mỗi ô
    std::unique_ptr<BasePiece> board[8][8];

    // --- Âm thanh ---
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;

public:

    Board();  // Khai báo con trỏ mặc định

    Board(const Board& other);

    // Load toàn bộ tài nguyên (ảnh + âm thanh)
    bool loadAssets();

    // Vẽ bàn cờ, quân cờ, và các highlight
    void draw(sf::RenderWindow& window);

    // Đặt quân cờ (theo tên) tại một ô cụ thể
    void setPiece(int row, int col, const std::string& name);

    // Lấy tên quân cờ tại một ô cụ thể
    std::string getPieceName(int row, int col) const;

    // Phát âm thanh theo tên (Làm chung với logic, VD: castle) (bổ sung cho movePiece)
    void playSound(const std::string& name);

    // Phát âm thanh khi di chuyển (Làm chung với dragHandler)) 
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // --- Hỗ trợ set asset bằng getter từ BasePiece ---
    std::string getNameFromPiece(const BasePiece& piece) const {
        std::string name = (piece.get_color() == Color::White) ? "w" : "b";
        switch (piece.get_pieceType()) {
        case PieceType::Pawn:   name += "Pawn"; break;
        case PieceType::Rook:   name += "Rook"; break;
        case PieceType::Knight: name += "Knight"; break;
        case PieceType::Bishop: name += "Bishop"; break;
        case PieceType::Queen:  name += "Queen"; break;
        case PieceType::King:   name += "King"; break;
        }
        return name;
    }

    // Lấy vị trí quân cờ từ BasePiece
    Position getPiecePosition(const BasePiece& piece) const {
        return piece.get_pos();
    }

    // Phát âm thanh khi phong cấp quân cờ
    void promotePiece(int row, int col, const std::string& newPieceName);

    // Phát âm thanh khi bắt đầu game
    void startGame();

    // HÀM CẦN TRIỂN KHAI TRONG PHẦN LOGIC CỦA BOARD HIỆN TẠI:
    std::unique_ptr<BasePiece> move_piece_for_ai(Position from, Position to);
    void undo_move_for_ai(Position from, Position to, std::unique_ptr<BasePiece> capturedPiece);
    const BasePiece* get_piece_at(Position p) const; // Lấy quân cờ tại vị trí đó
    bool is_inside_board(Position p) const; // Kiểm tra vị trí đích đến của quân cờ có nằm trong bàn cờ hay không
    bool is_check(Color color) const; // Kiểm tra vua có bị chiếu hay không
    bool can_castle_rook(Position rookPos) const; // Vua kiểm tra xe đi hay chưa để nhập thành
    bool is_square_under_attacked(Position pos, Color byColor) const; // Kiểm tra xem ô đó có an toàn để vua nhập thành không
    bool isCheckmate(Color color) const;

    std::string toFen(Color nextTurn) const;

    // phần minh thêm:
    sf::Sprite* getPieceSpriteAt(Position pos); // bổ trợ cho xử lý kéo thả quân

};

#endif
