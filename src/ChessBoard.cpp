#include "Pawn.h"
#include "ChessBoard.h"
#include <iostream>

using namespace std;

// Hàm khởi tạo mặc định
Board::Board() {
    // Khởi tạo texture, pointer, v.v. nếu cần
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            board[y][x] = nullptr;
}

// Hàm khởi tạo sao chép (Copy Constructor)
Board::Board(const Board& other) {
    // Sao chép các thành phần đồ họa và âm thanh
    this->boardTexture = other.boardTexture;
    this->boardSprite.setTexture(this->boardTexture);
    this->pieceTextures = other.pieceTextures;
    this->soundBuffers = other.soundBuffers;
    this->sounds = other.sounds;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            // Sao chép tên và sprite
            this->pieceNames[y][x] = other.pieceNames[y][x];
            if (!this->pieceNames[y][x].empty()) {
                this->pieces[y][x].setTexture(this->pieceTextures[this->pieceNames[y][x]]);
                this->pieces[y][x].setPosition(x * 64, y * 64); // Giả sử ô cờ 64x64
            }

            // Quan trọng: Sao chép sâu (deep copy) trạng thái logic của quân cờ
            if (other.board[y][x]) {
                this->board[y][x] = other.board[y][x]->clone();
            } else {
                this->board[y][x] = nullptr;
            }
        }
    }
}

bool Board::loadAssets() {
    // Load bàn cờ
    if (!boardTexture.loadFromFile("assets/Board and pieces/brown.png")) {
        cout << "Không thể tải texture bàn cờ!" << endl;
        return false;
    }
    boardSprite.setTexture(boardTexture);  // Gán texture đã tải cho sprite bàn cờ

    // Load texture các quân cờ
    string pieces[] = { "wPawn", "wRook", "wKnight", "wBishop", "wQueen", "wKing",
                      "bPawn", "bRook", "bKnight", "bBishop", "bQueen", "bKing" };

    // Duyệt qua tất cả các loại quân cờ và tải texture tương ứng
    for (const auto& piece : pieces) {
        if (!pieceTextures[piece].loadFromFile("assets/Board and pieces/" + piece + ".png")) {
            cout << "Không thể tải texture " << piece << "!" << endl;
            return false;
        }
    }

    // Load âm thanh
    string soundNames[] = { "move", "capture", "castle", "check", "promote", "game-start", "game-end" };
    for (const auto& sound : soundNames) {
        if (!soundBuffers[sound].loadFromFile("assets/Sounds-chess/" + sound + ".mp3")) {
            cout << "Không thể tải âm thanh " << sound << "!" << endl;
            continue;  // Tiếp tục tải các âm thanh khác nếu có lỗi
        }
        sounds[sound].setBuffer(soundBuffers[sound]);  // Gán buffer âm thanh cho đối tượng sound
    }

    return true;  // Trả về true nếu tải tất cả assets thành công
}

void Board::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    int tileSize = std::min(windowSize.x, windowSize.y) / 8;
    int boardSize = tileSize * 8;

    // Tính vị trí để căn giữa bàn cờ (nếu cửa sổ không vuông)
    int offsetX = (windowSize.x - boardSize) / 2;
    int offsetY = (windowSize.y - boardSize) / 2;

    // Scale ảnh nền bàn cờ
    boardSprite.setScale(
        static_cast<float>(boardSize) / boardTexture.getSize().x,
        static_cast<float>(boardSize) / boardTexture.getSize().y
    );
    boardSprite.setPosition(static_cast<float>(offsetX), static_cast<float>(offsetY));
    window.draw(boardSprite);

    // Vẽ quân cờ
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!pieceNames[row][col].empty()) {
                sf::Sprite& piece = pieces[row][col];

                // Scale đúng kích thước ô
                piece.setScale(
                    static_cast<float>(tileSize) / piece.getTexture()->getSize().x,
                    static_cast<float>(tileSize) / piece.getTexture()->getSize().y
                );

                // Đặt vị trí căn theo offset
                piece.setPosition(
                    offsetX + col * tileSize,
                    offsetY + row * tileSize
                );

                window.draw(piece);
            }
        }
    }
}


void Board::setPiece(int row, int col, const string& name) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;  // Kiểm tra giới hạn bàn cờ

    pieceNames[row][col] = name;  // Gán tên quân cờ
    if (!name.empty()) {  // Nếu tên không rỗng
        pieces[row][col].setTexture(pieceTextures[name]);  // Gán texture tương ứng
        pieces[row][col].setPosition(col * 64, row * 64);  // Đặt vị trí (giả sử mỗi ô 64x64 pixel)
    }
}

string Board::getPieceName(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return "";  // Kiểm tra giới hạn
    return pieceNames[row][col];  // Trả về tên quân cờ tại vị trí [row][col]
}

void Board::playSound(const string& name) {
    if (sounds.find(name) != sounds.end()) {  // Nếu âm thanh tồn tại
        sounds[name].play();  // Phát âm thanh
    }
    else {
        cout << "Không tìm thấy âm thanh " << name << "!" << endl;
    }
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    // Kiểm tra giới hạn bàn cờ
    if (!is_inside_board({fromCol, fromRow}) || !is_inside_board({toCol, toRow})) return;

    // Lấy quân đang di chuyển
    BasePiece* movingPiece = board[fromRow][fromCol].get();
    if (!movingPiece) return;

    // Kiểm tra hợp lệ nếu cần
    Position to{toCol, toRow};
    Position from{fromCol, fromRow};
    if (!movingPiece->is_move_valid(*this, to)) return;

    Pawn* pawn = dynamic_cast<Pawn*>(movingPiece);
    if (pawn) {
        // Tính y (khoảng cách)
        int dy = to.y - from.y;
        dy = abs(dy);

        pawn->set_just_moved_2step(dy == 2);
        pawn->set_first_move(false);
    }

    // Âm thanh
    if (!pieceNames[toRow][toCol].empty()) {
        playSound("capture");
    } else {
        playSound("move");
    }

    // Di chuyển logic
    board[toRow][toCol] = std::move(board[fromRow][fromCol]);
    board[fromRow][fromCol] = nullptr;
    board[toRow][toCol]->set_pos(to);

    // Di chuyển hiển thị
    pieceNames[toRow][toCol] = pieceNames[fromRow][fromCol];
    pieceNames[fromRow][fromCol] = "";

    pieces[toRow][toCol].setTexture(*pieces[fromRow][fromCol].getTexture());
    pieces[toRow][toCol].setPosition(toCol * 64, toRow * 64);
    pieces[fromRow][fromCol].setTexture(sf::Texture());
}


void Board::promotePiece(int row, int col, const string& newPieceName) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;  // Giới hạn

    // Nếu không có quân nào ở vị trí này => bỏ qua
    if (!board[row][col]) return;

    // Kiểm tra quân hiện tại có phải tốt không
    if (board[row][col]->get_pieceType() != PieceType::Pawn) return;

    // Lấy màu quân cờ hiện tại
    Color color = board[row][col]->get_color();

    // --- Phần logic ---
    Position pos{col, row};

    if (newPieceName == "wQueen" || newPieceName == "bQueen") {
        board[row][col] = std::make_unique<Queen>(color, pos);
    } else if (newPieceName == "wRook" || newPieceName == "bRook") {
        board[row][col] = std::make_unique<Rook>(color, pos);
    } else if (newPieceName == "wBishop" || newPieceName == "bBishop") {
        board[row][col] = std::make_unique<Bishop>(color, pos);
    } else if (newPieceName == "wKnight" || newPieceName == "bKnight") {
        board[row][col] = std::make_unique<Knight>(color, pos);
    } else {
        return; // Không rõ tên quân được phong cấp => không làm gì
    }

    // --- Phần hiển thị ---
    setPiece(row, col, newPieceName);

    // Âm thanh phong cấp
    playSound("promote");
}

void Board::startGame() {
    playSound("game-start");  // Phát âm thanh bắt đầu game
}

void Board::setLogicPiece(int row, int col, std::unique_ptr<BasePiece> piece) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;
    board[row][col] = std::move(piece);
}

// NHỮNG HÀM CẦN TRIỂN KHAI TRONG PHẦN LOGIC CỦA BOARD: (Quang làm giùm phần này)
// Hàm này di chuyển quân cờ và trả về quân cờ đã bị ăn
std::unique_ptr<BasePiece> Board::move_piece_for_ai(Position from, Position to) {
    if (!is_inside_board(from) || !is_inside_board(to)) return nullptr;

    auto capturedPiece = std::move(board[to.y][to.x]);
    board[to.y][to.x] = std::move(board[from.y][from.x]);
    board[from.y][from.x] = nullptr;

    if (board[to.y][to.x]) {
        board[to.y][to.x]->set_pos(to);
    }

    return capturedPiece;
}

// Hàm này hoàn tác lại nước đi
void Board::undo_move_for_ai(Position from, Position to, std::unique_ptr<BasePiece> capturedPiece) {
    if (!is_inside_board(from) || !is_inside_board(to)) return;

    board[from.y][from.x] = std::move(board[to.y][to.x]);
    board[to.y][to.x] = std::move(capturedPiece);

    if (board[from.y][from.x]) {
        board[from.y][from.x]->set_pos(from);
    }
}

// Lấy quân cờ tại vị trí p
const BasePiece* Board::get_piece_at(Position p) const {
    if (!is_inside_board(p)) return nullptr;
    // Giả sử bạn có mảng board[8][8] lưu unique_ptr<BasePiece>
    return board[p.y][p.x] ? board[p.y][p.x].get() : nullptr;
}

// Kiểm tra vị trí có nằm trong bàn cờ không
bool Board::is_inside_board(Position p) const {
    return p.x >= 0 && p.x < 8 && p.y >= 0 && p.y < 8;
}

// Kiểm tra vua có bị chiếu không
bool Board::is_check(Color color) const {
    // Tìm vị trí vua
    Position kingPos{-1, -1};
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board[y][x].get();
            if (piece && piece->get_pieceType() == PieceType::King && piece->get_color() == color) {
                kingPos = {x, y};
                break;
            }
        }
    }
    if (kingPos.x == -1) return false; // Không tìm thấy vua

    // Kiểm tra ô vua có bị tấn công không
    return is_square_under_attacked(kingPos, color == Color::White ? Color::Black : Color::White);
}

// Kiểm tra xe có thể nhập thành không (chưa di chuyển)
bool Board::can_castle_rook(Position rookPos) const {
    if (!is_inside_board(rookPos)) return false;
    const BasePiece* piece = get_piece_at(rookPos);
    if (!piece || piece->get_pieceType() != PieceType::Rook) return false;
    // Giả sử Rook có hàm can_castling()
    const Rook* rook = dynamic_cast<const Rook*>(piece);
    return rook && rook->can_castling();
}

// Kiểm tra ô pos có bị quân đối phương tấn công không
bool Board::is_square_under_attacked(Position pos, Color byColor) const {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board[y][x].get();
            if (piece && piece->get_color() == byColor) {
                if (piece->is_move_valid(*this, pos)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) const {
    if (is_check(color)) {
        // Giả sử không còn nước đi hợp lệ nữa thì là chiếu hết
        return true; // Chỉ là giả lập
    }
    return false;
}

// phần minh thêm
sf::Sprite* Board::getPieceSpriteAt(Position pos) {
    if (!is_inside_board(pos)) return nullptr;
    if (pieceNames[pos.y][pos.x].empty()) return nullptr;
    return &pieces[pos.y][pos.x];
} // bổ trợ cho xử lý kéo thả quân
