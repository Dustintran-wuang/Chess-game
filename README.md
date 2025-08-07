# ♟️ ChessWorld

**ChessWorld** là một trò chơi cờ vua viết bằng C++ sử dụng **SFML** và **Boost.Process**, tích hợp trí tuệ nhân tạo **Stockfish**.

---

## 🛠 Yêu cầu hệ thống

- CMake ≥ 3.28
- C++17 compiler (Visual Studio, GCC, Clang...)
- [SFML](https://www.sfml-dev.org/) ≥ 2.5
- [Boost](https://www.boost.org/) (bắt buộc: `process`, `filesystem`, `system`)
- [Stockfish](https://stockfishchess.org/download/) (bản `.exe` trên Windows)

---

## 📦 Thiết lập dự án

### 1. Clone repository

```bash
git clone https://github.com/your-username/ChessWorld.git
cd ChessWorld

2. Cài thư viện bằng vcpkg (khuyến nghị)
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat
./vcpkg/vcpkg install sfml boost-filesystem boost-system boost-process

***LỆNH BUILD VỚI CMAKE
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=.path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
tải stockfish về và đưa stockfish.exe vào trong thư mục chung với ChessWorld.exe (build/bin/Debug)
 
****LƯU Ý: NẾU TRÊN MÁY CÓ SFML TỪ TRƯỚC THÌ NÊN XÓA ĐI TRÁNH XUNG ĐỘT VỚI VCPKG KHI BUILD