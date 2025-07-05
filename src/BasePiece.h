#ifndef BASEPIECE_H
#define BASEPIECE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// ----------- ENUMS -----------
enum class Color { White, Black };
enum class PieceType { King, Queen, Knight, Bishop, Rook, Pawn };

// ----------- POSITION STRUCT -----------
struct Position {
    int x;
    int y;

    // Để so sánh vị trí quân cờ
    bool operator == (const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator != (const Position& other) const {
        return !(*this == other);
    }
};

// Forward declaration
class Board;

// ----------- BASE CLASS -----------
class BasePiece {
protected:
    PieceType type;
    Position pos;
    Color color;

public:
    BasePiece() = default;
    BasePiece(Color color, PieceType type, Position pos)
        : color(color), type(type), pos(pos) {
    }
    virtual ~BasePiece() = default;

    // -------- Getter --------
    Position get_pos() const { 
        return pos; 
    }
    Color get_color() const { 
        return color; 
    }
    PieceType get_pieceType() const { 
        return type; 
    }

    // -------- Setter --------
    void set_pos(Position new_pos) { 
        pos = new_pos; 
    }

    // -------- Logic --------
    virtual bool is_move_valid(const Board& board, Position dest) const = 0; // Kiểm tra nước đi có hợp lí hay không
    virtual std::vector<Position> get_all_moves(const Board& board) const = 0; // Danh sách các nước đi mà quân cờ có thể đi (Để check chiếu cho dễ)
    virtual std::unique_ptr<BasePiece> clone() const = 0; // Để undo/redo
};

#endif
