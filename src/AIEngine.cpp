#include "AIEngine.h"
#include <climits>
#include <algorithm>

ChessBot::ChessBot(int depth) : maxDepth(depth) {}

int ChessBot::evaluateBoard(const Board& board, Color aiColor) {
    int score = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board.get_piece_at({x, y});
            if (!piece) continue;
            
            int value = 0;
            switch (piece->get_pieceType()) {
                case PieceType::Pawn:   value = 100; break;
                case PieceType::Knight: value = 320; break;
                case PieceType::Bishop: value = 330; break;
                case PieceType::Rook:   value = 500; break;
                case PieceType::Queen:  value = 900; break;
                case PieceType::King:   value = 20000; break;
            }
            score += (piece->get_color() == aiColor ? value : -value);
        }
    }
    return score;
}

std::vector<Move> ChessBot::generateAllMoves(const Board& board, Color color) {
    std::vector<Move> moves;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board.get_piece_at({x, y});
            if (piece && piece->get_color() == color) {
                for (const Position& to : piece->get_all_moves(board)) {
                    moves.push_back(Move{{x, y}, to});
                }
            }
        }
    }
    return moves;
}

std::unique_ptr<BasePiece> ChessBot::makeMove(Board& board, const Move& move) {
    std::unique_ptr<BasePiece> capturedPiece = board.move_piece_for_ai(move.from, move.to);
    return capturedPiece;
}

void ChessBot::undoMove(Board& board, const Move& move, std::unique_ptr<BasePiece> capturedPiece) {
    board.undo_move_for_ai(move.from, move.to, std::move(capturedPiece));
}

bool ChessBot::isGameOver(Board& board, Color color) {
    if (!board.is_check(color)) return false;

    std::vector<Move> moves = generateAllMoves(board, color);
    for (const auto& move : moves) {
        Board tempBoard = board;
        makeMove(tempBoard, move);
        if (!tempBoard.is_check(color)) {
            return false; // Vẫn còn nước đi hợp lệ để thoát khỏi thế bị chiếu
        }
    }
    return true; // Bị chiếu hết
}

int ChessBot::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing, Color aiColor) {
    if (depth == 0 || isGameOver(board, isMaximizing ? aiColor : (aiColor == Color::White ? Color::Black : Color::White))) {
        return evaluateBoard(board, aiColor);
    }

    Color currentColor = isMaximizing ? aiColor : (aiColor == Color::White ? Color::Black : Color::White);
    std::vector<Move> allMoves = generateAllMoves(board, currentColor);

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& move : allMoves) {
            std::unique_ptr<BasePiece> capturedPiece = makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, false, aiColor);
            undoMove(board, move, std::move(capturedPiece));
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : allMoves) {
            std::unique_ptr<BasePiece> capturedPiece = makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, true, aiColor);
            undoMove(board, move, std::move(capturedPiece));
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

Move ChessBot::findBestMove(Board& board, Color aiColor) {
    std::vector<Move> allMoves = generateAllMoves(board, aiColor);
    if (allMoves.empty()) {
        return Move{{-1,-1}, {-1,-1}}; // Không có nước đi nào
    }

    Move bestMove = allMoves[0];
    int bestValue = INT_MIN;

    for (const auto& move : allMoves) {
        Board tempBoard = board; // Tạo bản sao của bàn cờ để thử nước đi
        std::unique_ptr<BasePiece> capturedPiece = makeMove(tempBoard, move);
        int moveValue = minimax(tempBoard, maxDepth - 1, INT_MIN, INT_MAX, false, aiColor);
        
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    return bestMove;
}

void ChessBot::setDifficulty(int depth) {
    maxDepth = std::max(1, std::min(depth, 8)); // Giới hạn độ sâu từ 1 đến 8
}