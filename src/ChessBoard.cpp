#include "ChessBoard.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool ChessBoard::loadAssets() {
    // Load bàn cờ
    if (!boardTexture.loadFromFile("assets/Board and pieces/brown.png")) {
        std::cerr << "Không thể tải texture bàn cờ!" << std::endl;
        return false;
    }
    boardSprite.setTexture(boardTexture);

    // Load texture các quân cờ
    std::string pieces[] = { "wPawn", "wRook", "wKnight", "wBishop", "wQueen", "wKing",
                           "bPawn", "bRook", "bKnight", "bBishop", "bQueen", "bKing" };

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
            continue;
        }
        sounds[sound].setBuffer(soundBuffers[sound]);
    }

    return true;
}

void ChessBoard::draw(sf::RenderWindow& window) {
    window.draw(boardSprite);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!pieceNames[row][col].empty()) {
                window.draw(pieces[row][col]);
            }
        }
    }
}

void ChessBoard::setPiece(int row, int col, const std::string& name) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;

    pieceNames[row][col] = name;
    if (!name.empty()) {
        pieces[row][col].setTexture(pieceTextures[name]);
        pieces[row][col].setPosition(col * 64, row * 64);
    }
}

std::string ChessBoard::getPieceName(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return "";
    return pieceNames[row][col];
}

void ChessBoard::playSound(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play();
    }
    else {
        std::cerr << "Không tìm thấy âm thanh " << name << "!" << std::endl;
    }
}

void ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) return;

    if (!pieceNames[toRow][toCol].empty()) {
        playSound("capture");
    }
    else {
        playSound("move");
    }

    pieceNames[toRow][toCol] = pieceNames[fromRow][fromCol];
    pieceNames[fromRow][fromCol] = "";

    pieces[toRow][toCol].setTexture(pieces[fromRow][fromCol].getTexture());
    pieces[toRow][toCol].setPosition(toCol * 64, toRow * 64);
    pieces[fromRow][fromCol].setTexture(sf::Texture());
}

void ChessBoard::promotePiece(int row, int col, const std::string& newPieceName) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;

    if (!pieceNames[row][col].empty()) {
        setPiece(row, col, newPieceName);
        playSound("promote");
    }
}

void ChessBoard::startGame() {
    playSound("game-start");
}
