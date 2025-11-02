// leak_single.cpp
// 极简内存泄漏示例：程序退出时会有内存未释放，方便用 valgrind 检测。

#include <iostream>
#include <unistd.h>


int main() {
    std::cout << "leak_single start\n";

    // 故意泄漏：分配后不 delete
    int loop_cnt = 1;
    while (loop_cnt != 10) {
        int* p = new int(12345);
        std::cout << "do something" << std::endl;
        sleep(3);
    }

    // 做点事（避免编译器优化掉）

    std::cout << "leak_single end\n";
    return 0;
}
