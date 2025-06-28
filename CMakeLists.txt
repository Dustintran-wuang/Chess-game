cmake_minimum_required(VERSION 3.28)
project(ChessWorld LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Nơi output file exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Lấy toàn bộ file .cpp trong src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Dùng FetchContent để lấy SFML 2.6.2
include(FetchContent)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

# Tạo file chạy
add_executable(ChessWorld ${SOURCES})

# Link với SFML 2.6.2
target_link_libraries(ChessWorld PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Copy thư mục resource/ vào thư mục chứa file .exe
add_custom_command(TARGET ChessWorld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:ChessWorld>/assets)