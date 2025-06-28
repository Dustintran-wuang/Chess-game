#ifndef BASEPIECE_H
#define BASEPIECE_H

#include <iostream>
#include <string>
#include <vector>

// ----------- ENUMS -----------
enum class Color { White, Black };
enum class PieceType { King, Queen, Knight, Bishop, Rook, Pawn };

// ----------- POSITION STRUCT -----------
struct Position {
    int x;
    int y;

    // Để so sánh vị trí quân cờ
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Forward declaration để tránh include vòng lặp
class Board;

// ----------- BASE CLASS -----------
class BasePiece {
protected:
    PieceType type;
    Position pos;
    Color color;

public:
    BasePiece() = default;
    BasePiece(PieceType type, Position pos, Color color)
        : type(type), pos(pos), color(color) {
    }
    virtual ~BasePiece() = default;

    // -------- Getter --------
    Position get_pos() const { return pos; }
    Color get_color() const { return color; }
    PieceType get_pieceType() const { return type; }

    // -------- Setter --------
    void set_pos(Position new_pos) { pos = new_pos; }

    // -------- Logic --------
    virtual bool is_move_valid(const Board& board, Position dest) const = 0;
    virtual std::vector<Position> get_all_moves(const Board& board) const = 0;
    virtual std::unique_ptr<BasePiece> clone() const = 0;
};

#endif
