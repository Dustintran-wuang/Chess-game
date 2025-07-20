#include "UIManager.h"
#include <memory> 
using namespace std;

int main() {
    // Allocate UIManager on the heap using a smart pointer
    unique_ptr<UIManager> ui = std::make_unique<UIManager>();
    ui->run(); // Bắt đầu vòng lặp chính
    return 0;
}
