#ifndef STOCKFISH_AI_H
#define STOCKFISH_AI_H

#include <string>
#include <memory>
#include <boost/process.hpp>
#include "ChessBoard.h"
#include "AIEngine.h" 

namespace bp = boost::process;

class StockfishAI {
private:
    // --- THAY ĐỔI: Sử dụng 2 pipe riêng biệt ---
    bp::pstream stockfish_input;  // Pipe để ĐỌC dữ liệu từ Stockfish (stdout của nó)
    bp::pstream stockfish_output; // Pipe để GHI dữ liệu tới Stockfish (stdin của nó)

    std::unique_ptr<bp::child> stockfish_process;

    Move parseMove(const std::string& moveStr);

public:
    StockfishAI(const std::string& path_to_exe);
    ~StockfishAI();

    Move findBestMove(const Board& board, Color aiColor, int moveTimeMs = 2000);
};

#endif // STOCKFISH_AI_H