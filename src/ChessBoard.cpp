#include "Pawn.h"
#include "King.h"
#include "ChessBoard.h"
#include <sstream>
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

    // Load UI chọn quân để phong
    if (!wPromotionTexture.loadFromFile("assets/Board and pieces/wPromotion.png")) {
        cout << "Không thể tải ảnh chọn phong cấp!" << endl;
        return false;
    }
    wPromotionSprite.setTexture(wPromotionTexture);

    if (!bPromotionTexture.loadFromFile("assets/Board and pieces/bPromotion.png")) {
        cout << "Không thể tải ảnh chọn phong cấp!" << endl;
        return false;
    }
    bPromotionSprite.setTexture(bPromotionTexture);


    return true;  // Trả về true nếu tải tất cả assets thành công
}

void Board::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    int tileSize = std::min(windowSize.x, windowSize.y) / 8;
    int boardSize = tileSize * 8;

    int offsetX = (windowSize.x - boardSize) / 2;
    int offsetY = (windowSize.y - boardSize) / 2;

    // Vẽ bàn cờ
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int drawCol = col;
            int drawRow = row;

            if (rotation) {
                drawCol = 7 - col;
                drawRow = 7 - row;
            }

            sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));
            square.setPosition(offsetX + drawCol * tileSize, offsetY + drawRow * tileSize);

            if ((row + col) % 2 == 0) {
                square.setFillColor(sf::Color(240, 217, 181)); // màu sáng
            }
            else {
                square.setFillColor(sf::Color(181, 136, 99));  // màu tối
            }

            window.draw(square);
        }
    }

    // Vẽ quân cờ
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::Sprite* sprite = getPieceSpriteAt(Position{ col, row });
            if (sprite) {
                int drawCol = col;
                int drawRow = row;

                if (rotation) {
                    drawCol = 7 - col;
                    drawRow = 7 - row;
                }

                sprite->setPosition(offsetX + drawCol * tileSize, offsetY + drawRow * tileSize);
                sprite->setScale(
                    (float)tileSize / sprite->getTexture()->getSize().x,
                    (float)tileSize / sprite->getTexture()->getSize().y);

                window.draw(*sprite);
            }
        }
    }

    // Vẽ UI chọn quân để phong
    if (showingPromotion) {
        sf::Vector2u windowSize = window.getSize();
        int tileSize = std::min(windowSize.x, windowSize.y) / 8;
        int size = tileSize * 4; // dùng 4 ô = 2x2

        if (wPromotion == true) {

            wPromotionSprite.setScale(static_cast<float>(size) / wPromotionTexture.getSize().x, static_cast<float>(size) / wPromotionTexture.getSize().y);

            wPromotionSprite.setPosition((windowSize.x - size) / 2.f, (windowSize.y - size) / 2.f);

            window.draw(wPromotionSprite);
        }
        else {
            bPromotionSprite.setScale(static_cast<float>(size) / bPromotionTexture.getSize().x, static_cast<float>(size) / bPromotionTexture.getSize().y);

            bPromotionSprite.setPosition((windowSize.x - size) / 2.f, (windowSize.y - size) / 2.f);

            window.draw(bPromotionSprite);
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

void Board::handlePromotionClick(int mouseX, int mouseY, sf::RenderWindow& window) {
    if (!showingPromotion) return;

    sf::Vector2u windowSize = window.getSize();
    int tileSize = std::min(windowSize.x, windowSize.y) / 8;
    int size = tileSize * 4; // Promotion UI vẽ bằng 4 ô (2x2)

    int offsetX = (windowSize.x - size) / 2;
    int offsetY = (windowSize.y - size) / 2;

    // Tính vị trí click trong promotionSprite
    int relX = mouseX - offsetX;
    int relY = mouseY - offsetY;

    if (relX < 0 || relY < 0 || relX >= size || relY >= size) return;

    int col = relX / (size / 2);
    int row = relY / (size / 2);

    string namePrefix = (promotionColor == Color::White) ? "w" : "b";
    string newPiece;

    if (row == 0 && col == 0) newPiece = namePrefix + "Knight";
    else if (row == 0 && col == 1) newPiece = namePrefix + "Queen";
    else if (row == 1 && col == 0) newPiece = namePrefix + "Bishop";
    else if (row == 1 && col == 1) newPiece = namePrefix + "Rook";

    if (!newPiece.empty()) {
        promotePiece(promotionRow, promotionCol, newPiece);
        showingPromotion = false;
    }
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    // Kiểm tra giới hạn bàn cờ
    if (!is_inside_board({ fromCol, fromRow }) || !is_inside_board({ toCol, toRow }))
        return;

    BasePiece* movingPiece = board[fromRow][fromCol].get();
    if (!movingPiece) return;

    Position from{ fromCol, fromRow };
    Position to{ toCol, toRow };

    // Kiểm tra nước đi hợp lệ
    if (!movingPiece->is_move_valid(*this, to)) return;

    // ================= Xử lý Castling =================
    King* king = dynamic_cast<King*>(movingPiece);
    bool castlingMove = false;
    if (king && std::abs(toCol - fromCol) == 2 && fromRow == toRow) {
        castlingMove = true;
        bool kingSide = (toCol > fromCol);

        int rookFromCol = kingSide ? 7 : 0;
        int rookToCol = kingSide ? toCol - 1 : toCol + 1;

        // Logic rook
        board[toRow][rookToCol] = std::move(board[toRow][rookFromCol]);
        board[toRow][rookToCol]->set_pos({ rookToCol, toRow });
        board[toRow][rookFromCol] = nullptr;

        // Hiển thị rook
        pieceNames[toRow][rookToCol] = pieceNames[toRow][rookFromCol];
        pieceNames[toRow][rookFromCol].clear();
        pieces[toRow][rookToCol].setTexture(*pieces[toRow][rookFromCol].getTexture());
        pieces[toRow][rookToCol].setPosition(rookToCol * 64, toRow * 64);
        pieces[toRow][rookFromCol].setTexture(sf::Texture());

        // Mất quyền nhập thành
        Rook* rook = dynamic_cast<Rook*>(board[toRow][rookToCol].get());
        if (rook) rook->set_castling(false);
        king->set_castling(false);
    }
    // ===================================================

    Pawn* pawn = dynamic_cast<Pawn*>(movingPiece);
    if (pawn) {
        int dy = std::abs(to.y - from.y);
        pawn->set_just_moved_2step(dy == 2);
        pawn->set_first_move(false);
    }

    // ================= Xử lý En Passant =================
    bool enPassant = false;
    int capturedRow = -1, capturedCol = -1;

    if (pawn) {
        int dy = toRow - fromRow;
        int dx = toCol - fromCol;
        if (abs(dx) == 1 && dy == (pawn->get_color() == Color::White ? -1 : 1)) {
            if (pieceNames[toRow][toCol].empty()) { // không ăn trực tiếp
                int sidePawnRow = fromRow;
                int sidePawnCol = toCol;

                BasePiece* sidePiece = board[sidePawnRow][sidePawnCol].get();
                if (sidePiece &&
                    sidePiece->get_pieceType() == PieceType::Pawn &&
                    sidePiece->get_color() != pawn->get_color()) {

                    Pawn* targetPawn = dynamic_cast<Pawn*>(sidePiece);
                    if (targetPawn && targetPawn->did_just_move_2step()) {
                        enPassant = true;
                        capturedRow = sidePawnRow;
                        capturedCol = sidePawnCol;
                    }
                }
            }
        }
    }

    if (enPassant) {
        board[capturedRow][capturedCol] = nullptr;
        pieceNames[capturedRow][capturedCol].clear();
        pieces[capturedRow][capturedCol].setTexture(sf::Texture());
        playSound("capture");
    }
    else if (!pieceNames[toRow][toCol].empty()) {
        playSound("capture");
    }
    else {
        playSound("move");
    }
    // =====================================================

    // Di chuyển quân
    board[toRow][toCol] = std::move(board[fromRow][fromCol]);
    board[fromRow][fromCol] = nullptr;
    board[toRow][toCol]->set_pos(to);

    // Mất quyền nhập thành nếu là king hoặc rook
    if (king) king->set_castling(false);
    if (Rook* rookMoved = dynamic_cast<Rook*>(board[toRow][toCol].get()))
        rookMoved->set_castling(false);

    // Cập nhật hiển thị
    pieceNames[toRow][toCol] = pieceNames[fromRow][fromCol];
    pieceNames[fromRow][fromCol].clear();
    pieces[toRow][toCol].setTexture(*pieces[fromRow][fromCol].getTexture());
    pieces[toRow][toCol].setPosition(toCol * 64, toRow * 64);
    pieces[fromRow][fromCol].setTexture(sf::Texture());

    // ================= Xử lý Promotion =================
    if (pawn) {
        if ((pawn->get_color() == Color::White && toRow == 0) ||
            (pawn->get_color() == Color::Black && toRow == 7)) {
            showingPromotion = true;
            wPromotion = (pawn->get_color() == Color::White);
            promotionRow = toRow;
            promotionCol = toCol;
            promotionColor = pawn->get_color();
        }
    }
    // ====================================================

    // Reset just_moved_2step cho các pawn khác
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (y == toRow && x == toCol) continue;
            if (Pawn* otherPawn = dynamic_cast<Pawn*>(board[y][x].get()))
                otherPawn->set_just_moved_2step(false);
        }
    }

    // Âm thanh chiếu
    Color currentColor = movingPiece->get_color();
    Color opponentColor = (currentColor == Color::White) ? Color::Black : Color::White;
    if (is_check(opponentColor))
        playSound("check");
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
	m_currentTurn = Color::White; //Bắt đầu với quân trắng
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

bool Board::is_square_under_attacked(Position pos, Color byColor) const {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = board[y][x].get();
            if (piece && piece->get_color() == byColor) {

                // ---- Xử lý riêng cho TỐT ----
                if (piece->get_pieceType() == PieceType::Pawn) {
                    Position pawnPos = piece->get_pos();
                    int direction = (byColor == Color::White) ? -1 : 1;
                    // Chỉ kiểm tra 2 ô chéo, đây là logic TẤN CÔNG
                    if (pawnPos.y + direction == pos.y &&
                        (pawnPos.x + 1 == pos.x || pawnPos.x - 1 == pos.x)) {
                        return true;
                    }
                }
                // ---- Xử lý riêng cho VUA (để tránh đệ quy) ----
                else if (piece->get_pieceType() == PieceType::King) {
                    Position kingPos = piece->get_pos();
                    // Kiểm tra 8 ô xung quanh Vua
                    if (std::abs(kingPos.x - pos.x) <= 1 && std::abs(kingPos.y - pos.y) <= 1) {
                        return true;
                    }
                }
                // ---- Dùng is_move_valid cho các quân còn lại (Xe, Mã, Tượng, Hậu) ----
                else {
                    if (piece->is_move_valid(*this, pos)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) const {
    if (is_check(color)) {
        return true; // Chỉ là giả lập
    }
    return false;
}

//hàm bổ trợ cho Stockfish engine
std::string Board::toFen(Color nextTurn) const {
    std::stringstream fen;
    for (int y = 0; y < 8; ++y) {
        int emptySquares = 0;
        for (int x = 0; x < 8; ++x) {
            const BasePiece* piece = get_piece_at({x, y});
            if (piece) {
                if (emptySquares > 0) {
                    fen << emptySquares;
                    emptySquares = 0;
                }
                char p_char;
                // Lấy ký tự quân cờ
                switch (piece->get_pieceType()) {
                    case PieceType::Pawn:   p_char = 'p'; break;
                    case PieceType::Rook:   p_char = 'r'; break;
                    case PieceType::Knight: p_char = 'n'; break;
                    case PieceType::Bishop: p_char = 'b'; break;
                    case PieceType::Queen:  p_char = 'q'; break;
                    case PieceType::King:   p_char = 'k'; break;
                }
                // Chuyển thành chữ hoa cho quân trắng
                if (piece->get_color() == Color::White) {
                    fen << (char)toupper(p_char);
                } else {
                    fen << p_char;
                }
            } else {
                emptySquares++;
            }
        }
        if (emptySquares > 0) {
            fen << emptySquares;
        }
        if (y < 7) {
            fen << '/';
        }
    }

    // Thêm lượt đi (w cho trắng, b cho đen)
    fen << (nextTurn == Color::White ? " w" : " b");

    // Chú ý: Đây là phiên bản đơn giản hóa. Một chuỗi FEN đầy đủ
    // cần thông tin về nhập thành, bắt tốt qua đường, v.v.
    // Với mục đích giao tiếp cơ bản, như này là đủ.
    fen << " - - 0 1";

    return fen.str();
}

// phần minh thêm
sf::Sprite* Board::getPieceSpriteAt(Position pos) {
    if (!is_inside_board(pos)) return nullptr;
    if (pieceNames[pos.y][pos.x].empty()) return nullptr;
    return &pieces[pos.y][pos.x];
} // bổ trợ cho xử lý kéo thả quân



// Hàm này trả về lượt đi hiện tại
Color Board::getCurrentTurn() const {
    return m_currentTurn;
}

// Hàm này sẽ được gọi khi người dùng thực hiện một nước đi
void Board::makeMove(const Move& move) {
    // Lấy tọa độ từ đối tượng Move bằng cách truy cập trực tiếp
    Position from = move.from; // Sử dụng move.from thay vì move.get_start_pos()
    Position to = move.to;   // Sử dụng move.to   thay vì move.get_end_pos()

    // Gọi hàm movePiece đã có sẵn của bạn để xử lý logic
    // Tọa độ đang dùng là (row, col) nên cần tráo đổi x, y
    movePiece(from.y, from.x, to.y, to.x);

    //Chuyển lượt cho người chơi tiếp theo
    m_currentTurn = (m_currentTurn == Color::White) ? Color::Black : Color::White;
}
