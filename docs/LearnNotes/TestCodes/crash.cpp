#include <iostream>
using namespace std;

int main() {
    int *p = nullptr;  // 野指针（空指针）
    std::cout << "Before crash" << std::endl;
    *p = 42;  // ❌ 这里会触发 Segmentation Fault
    std::cout << "After crash" << std::endl;
    return 0;
}
