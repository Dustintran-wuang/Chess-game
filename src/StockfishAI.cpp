#include "StockfishAI.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

StockfishAI::StockfishAI(const std::string& path_to_exe) {
    try {
        // --- THAY ĐỔI: Cấu hình 2 pipe riêng biệt ---
        stockfish_process = std::make_unique<bp::child>(
            path_to_exe,
            bp::std_in  < stockfish_output, // Gửi dữ liệu từ output của ta -> stdin của Stockfish
            (bp::std_out & bp::std_err) > stockfish_input  // Nhận dữ liệu từ stdout/stderr của Stockfish -> input của ta
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (!stockfish_process->running()) {
            throw std::runtime_error("Stockfish process failed to start. Check path/permissions.");
        }

        // --- Gửi lệnh và chờ phản hồi ---
        stockfish_output << "uci" << std::endl;
        std::cout << "C++: Sent 'uci'" << std::endl;

        std::string line;
        bool uci_ok = false;
        auto startTime = std::chrono::steady_clock::now();

        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() < 5) {
            if (std::getline(stockfish_input, line)) { // Đọc từ pipe input
                std::cout << "Stockfish: " << line << std::endl;
                if (line.find("uciok") != std::string::npos) {
                    uci_ok = true;
                    break;
                }
            }
        }

        if (!uci_ok) {
            throw std::runtime_error("Failed to get 'uciok' from Stockfish.");
        }

        stockfish_output << "isready" << std::endl;
        std::cout << "C++: Sent 'isready'" << std::endl;
        bool is_ready = false;
        startTime = std::chrono::steady_clock::now();

        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() < 5) {
            if (std::getline(stockfish_input, line)) { // Đọc từ pipe input
                std::cout << "Stockfish: " << line << std::endl;
                if (line.find("readyok") != std::string::npos) {
                    is_ready = true;
                    break;
                }
            }
        }

        if (!is_ready) {
            throw std::runtime_error("Failed to get 'readyok' from Stockfish.");
        }
        
        std::cout << "Stockfish is ready." << std::endl;

    } catch (const bp::process_error& e) {
        throw std::runtime_error("Failed to start Stockfish process: " + std::string(e.what()));
    }
}

StockfishAI::~StockfishAI() {
    if (stockfish_process && stockfish_process->running()) {
        stockfish_output << "quit" << std::endl; // Ghi vào pipe output
        stockfish_process->wait();
    }
}

Move StockfishAI::parseMove(const std::string& moveStr) {
    if (moveStr.length() < 4) return {{-1, -1}, {-1, -1}};
    Position from, to;
    from.x = moveStr[0] - 'a';
    from.y = 8 - (moveStr[1] - '0');
    to.x = moveStr[2] - 'a';
    to.y = 8 - (moveStr[3] - '0');
    return {from, to};
}

Move StockfishAI::findBestMove(const Board& board, Color aiColor, int moveTimeMs) {
    std::string fen = board.toFen(aiColor);
    stockfish_output << "position fen " << fen << std::endl; // Ghi vào pipe output
    stockfish_output << "go movetime " << moveTimeMs << std::endl; // Ghi vào pipe output

    std::string line;
    std::string bestMoveStr = "";
    while (stockfish_process->running() && std::getline(stockfish_input, line)) { // Đọc từ pipe input
        std::cout << "Stockfish (thinking): " << line << std::endl;
        if (line.rfind("bestmove", 0) == 0) {
            std::stringstream ss(line);
            std::string command;
            ss >> command;
            ss >> bestMoveStr;
            break;
        }
    }

    if (bestMoveStr.empty()) {
        throw std::runtime_error("Stockfish did not return a bestmove.");
    }
    
    return parseMove(bestMoveStr);
}