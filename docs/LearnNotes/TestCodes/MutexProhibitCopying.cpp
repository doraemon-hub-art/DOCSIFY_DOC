#include <mutex>
#include <string>
#include <iostream>

class Data {
public:
    Data(std::string s) : value(std::move(s)) {}

    // 禁止拷贝
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

    // 允许移动
    // Data(Data&&) = default;
    // Data& operator=(Data&&) = default;

private:
    std::string value;
    std::mutex mtx;
};

int main(void) {
    std::cout << __cplusplus << std::endl;
    Data data("test");

    return 0;
}