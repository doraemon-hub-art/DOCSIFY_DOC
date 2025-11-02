#include <iostream>
#include <vector>
#include <utility> // std::move

class MyData {
public:
    std::vector<int> data;

    // 普通构造函数
    MyData(int n) : data(n, 0) {
        std::cout << "Construct with size " << n << "\n";
    }

    // 拷贝构造函数
    MyData(const MyData& other) : data(other.data) {
        std::cout << "Copy constructor\n";
    }

    // 移动构造函数
    MyData(MyData&& other) noexcept : data(std::move(other.data)) {
        std::cout << "Move constructor\n";
    }
};  


int main(void) {
    MyData a(10);

    MyData b = std::move(a);

    MyData c(std::move(b));

    MyData d(c);

    auto makeData = [](int val) -> MyData{
        MyData tmp(val);
        return tmp;
    };

    // rov 优化，直接返回内部的临时变量，或是说直接在e的内存上构造
    // 所以拷贝、移动构造都没走
    // rov —— 返回值优化,编译器直接在调用者的内存空间里构造这个返回对象。
    MyData e = makeData(1);

    return 0;
}


