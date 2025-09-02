#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> x{0}, y{0};

void f() {
    x.store(1, std::memory_order_seq_cst);
    std::cout << "f: y=" << y.load(std::memory_order_seq_cst) << std::endl;
}

void g() {
    y.store(1, std::memory_order_seq_cst);
    std::cout << "g: x=" << x.load(std::memory_order_seq_cst) << std::endl;
}

int main() {
    std::thread t1(f), t2(g);
    t1.join();
    t2.join();
}
