#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include <vector>
#include <memory>
#include "ChessBoard.h"
#include "BasePiece.h"


class ChessBot {
private:
    int maxDepth;

public:
    ChessBot(int depth = 4);

    // Đánh giá điểm của bàn cờ cho AI
    int evaluateBoard(const Board& board, Color aiColor);

    // Tạo tất cả các nước đi hợp lệ cho một màu cờ
    std::vector<Move> generateAllMoves(const Board& board, Color color);

    // Thực hiện một nước đi trên bàn cờ và trả về quân cờ đã bị ăn (nếu có)
    std::unique_ptr<BasePiece> makeMove(Board& board, const Move& move);

    // Hoàn tác một nước đi
    void undoMove(Board& board, const Move& move, std::unique_ptr<BasePiece> capturedPiece);

    // Kiểm tra xem trò chơi đã kết thúc chưa (Chiếu hết hoặc hết nước đi)
    bool isGameOver(Board& board, Color color);
    
    // Thuật toán Minimax với cắt tỉa Alpha-Beta
    int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing, Color aiColor);

    // Tìm nước đi tốt nhất cho AI
    Move findBestMove(Board& board, Color aiColor);

    // Thiết lập độ khó (độ sâu tìm kiếm)
    void setDifficulty(int depth);
};

#endif // AI_ENGINE_H