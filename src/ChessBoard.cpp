#include "ChessBoard.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;  // Đặt bí danh cho namespace filesystem để thao tác với hệ thống file

bool ChessBoard::loadAssets() {
    // Load bàn cờ
    if (!boardTexture.loadFromFile("assets/Board and pieces/brown.png")) {
        std::cerr << "Không thể tải texture bàn cờ!" << std::endl;
        return false;
    }
    boardSprite.setTexture(boardTexture);  // Gán texture đã tải cho sprite bàn cờ

    // Load texture các quân cờ
    std::string pieces[] = { "wPawn", "wRook", "wKnight", "wBishop", "wQueen", "wKing",
                           "bPawn", "bRook", "bKnight", "bBishop", "bQueen", "bKing" };

    // Duyệt qua tất cả các loại quân cờ và tải texture tương ứng
    for (const auto& piece : pieces) {
        if (!pieceTextures[piece].loadFromFile("assets/Board and pieces/" + piece + ".png")) {
            std::cerr << "Không thể tải texture " << piece << "!" << std::endl;
            return false;
        }
    }

    // Load âm thanh
    std::string sounds[] = { "move", "capture", "castle", "check", "promote", "game-start", "game-end" };
    for (const auto& sound : sounds) {
        if (!soundBuffers[sound].loadFromFile("assets/Sounds-chess/" + sound + ".mp3")) {
            std::cerr << "Không thể tải âm thanh " << sound << "!" << std::endl;
            continue;  // Tiếp tục tải các âm thanh khác nếu có lỗi
        }
        sounds[sound].setBuffer(soundBuffers[sound]);  // Gán buffer âm thanh cho đối tượng sound
    }

    return true;  // Trả về true nếu tải tất cả assets thành công
}

void ChessBoard::draw(sf::RenderWindow& window) {
    window.draw(boardSprite);  // Vẽ bàn cờ lên cửa sổ

    // Vẽ các quân cờ
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!pieceNames[row][col].empty()) {  // Nếu ô có quân cờ
                window.draw(pieces[row][col]);  // Vẽ quân cờ tại vị trí [row][col]
            }
        }
    }
}

void ChessBoard::setPiece(int row, int col, const std::string& name) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;  // Kiểm tra giới hạn bàn cờ

    pieceNames[row][col] = name;  // Gán tên quân cờ
    if (!name.empty()) {  // Nếu tên không rỗng
        pieces[row][col].setTexture(pieceTextures[name]);  // Gán texture tương ứng
        pieces[row][col].setPosition(col * 64, row * 64);  // Đặt vị trí (giả sử mỗi ô 64x64 pixel)
    }
}

std::string ChessBoard::getPieceName(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return "";  // Kiểm tra giới hạn
    return pieceNames[row][col];  // Trả về tên quân cờ tại vị trí [row][col]
}

void ChessBoard::playSound(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {  // Nếu âm thanh tồn tại
        sounds[name].play();  // Phát âm thanh
    }
    else {
        std::cerr << "Không tìm thấy âm thanh " << name << "!" << std::endl;
    }
}

void ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    // Kiểm tra giới hạn bàn cờ
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) return;

    // Kiểm tra nếu có quân cờ ở vị trí đích (ăn quân)
    if (!pieceNames[toRow][toCol].empty()) {
        playSound("capture");  // Phát âm thanh ăn quân
    }
    else {
        playSound("move");  // Phát âm thanh di chuyển thông thường
    }

    // Di chuyển quân cờ
    pieceNames[toRow][toCol] = pieceNames[fromRow][fromCol];  // Di chuyển tên quân cờ
    pieceNames[fromRow][fromCol] = "";  // Xóa tên quân cờ ở vị trí cũ

    // Cập nhật sprite
    pieces[toRow][toCol].setTexture(pieces[fromRow][fromCol].getTexture());
    pieces[toRow][toCol].setPosition(toCol * 64, toRow * 64);  // Đặt vị trí mới
    pieces[fromRow][fromCol].setTexture(sf::Texture());  // Xóa texture ở vị trí cũ
}

// Xử lý phong cấp quân cờ
void ChessBoard::promotePiece(int row, int col, const std::string& newPieceName) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;  // Kiểm tra giới hạn

    if (!pieceNames[row][col].empty()) {  // Nếu ô có quân cờ
        setPiece(row, col, newPieceName);  // Đặt quân cờ mới
        playSound("promote");  // Phát âm thanh phong cấp
    }
}

// Xử lý bắt đầu game
void ChessBoard::startGame() {
    playSound("game-start");  // Phát âm thanh bắt đầu game
}
