cmake_minimum_required(VERSION 3.28)
project(ChessWorld LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thư mục chứa file thực thi (.exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# --- TÌM KIẾM CÁC GÓI THƯ VIỆN (Cú pháp SFML 2.x) ---
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# --- Tìm kiếm các gói thư viện ---
# Sử dụng vcpkg để tự động quản lý
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()
find_package(Boost REQUIRED COMPONENTS filesystem system process)

# --- TỰ ĐỘNG TÌM KIẾM FILE NGUỒN ---
# Lệnh này sẽ tự động tìm tất cả các file có đuôi .cpp trong thư mục 'src'
# và các thư mục con của nó.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Tạo file thực thi từ các file nguồn đã tìm thấy
add_executable(ChessWorld ${SOURCES})

# --- LIÊN KẾT THƯ VIỆN ---
# Link chương trình với các thư viện SFML và Boost
target_link_libraries(ChessWorld PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    Boost::filesystem
    Boost::system
    Boost::process
)

# --- GỘP CÁC ĐỊNH DANH BIÊN DỊCH ---
# Thêm các cờ cần thiết cho trình biên dịch vào một khối lệnh duy nhất
if(WIN32)
    # Trên Windows, thêm cả cờ Boost v1 và cờ Winsock
    target_compile_definitions(ChessWorld PRIVATE
        BOOST_PROCESS_V1_COMPATIBLE
        WIN32_LEAN_AND_MEAN
    )
else()
    # Trên các hệ điều hành khác, chỉ cần cờ Boost v1
    target_compile_definitions(ChessWorld PRIVATE
        BOOST_PROCESS_V1_COMPATIBLE
    )
endif()

# --- COPY THƯ MỤC ASSETS ---
add_custom_command(TARGET ChessWorld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:ChessWorld>/assets
)