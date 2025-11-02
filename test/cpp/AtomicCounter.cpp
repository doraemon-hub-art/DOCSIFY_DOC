#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter{0};  // 定义一个原子整型

// 每个线程执行的任务
void worker() {
    for (int i = 0; i < 100000; i++) {
        counter.fetch_add(1);  // 原子加 1
        // 等价写法： counter++;
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    // 读取最终结果
    std::cout << "Final counter = " << counter.load() << std::endl;
    return 0;
}