#include <iostream>
#include <string>
#include <vector>
#include <utility>

// ==========================================================
// 1. 成员变量类：资源持有者 (比如一个文件句柄或大块内存)
// ==========================================================
class MyResource {
public:
    MyResource() : data_(nullptr) {}
    
    // 带参构造
    explicit MyResource(const std::string& info) {
        data_ = new std::string(info);
        std::cout << "  [Resource] 分配资源: " << *data_ << std::endl;
    }

    // 析构函数
    ~MyResource() {
        if (data_) {
            std::cout << "  [Resource] 释放资源: " << *data_ << std::endl;
            delete data_;
            data_ = nullptr;
        }
    }

    // --- 关键：移动构造函数 ---
    MyResource(MyResource&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr; // 偷走资源，并将原指针置空
        std::cout << "  [Resource] 移动构造 (所有权转移)" << std::endl;
    }

    // --- 关键：移动赋值运算符 ---
    MyResource& operator=(MyResource&& other) noexcept {
        std::cout << "  [Resource] 移动赋值" << std::endl;
        if (this != &other) {
            delete data_;       // 1. 先清理自己的旧资源
            data_ = other.data_; // 2. 拿走别人的资源
            other.data_ = nullptr; // 3. 将别人置空
        }
        return *this;
    }

    // 禁用拷贝（模拟不可拷贝资源，如 ofstream）
    MyResource(const MyResource&) = delete;
    MyResource& operator=(const MyResource&) = delete;

private:
    std::string* data_;
};

// ==========================================================
// 2. 父类：管理多个成员
// ==========================================================
class DataManager {
public:
    DataManager(const std::string& name) 
        : name_(name), resource_(name + "_data") {}

    // --- 关键：父类的移动构造 ---
    // 即使 name_ 是 string，也建议 move，因为它可能很大
    DataManager(DataManager&& other) noexcept 
        : name_(std::move(other.name_)), 
          resource_(std::move(other.resource_)) { // 必须显式 std::move 成员变量
        std::cout << "[Manager] 执行移动构造" << std::endl;
    }

    // --- 关键：父类的移动赋值 ---
    DataManager& operator=(DataManager&& other) noexcept {
        std::cout << "[Manager] 执行移动赋值" << std::endl;
        if (this != &other) {
            name_ = std::move(other.name_);
            resource_ = std::move(other.resource_); // 触发成员的移动赋值
        }
        return *this;
    }

    // 同样禁用拷贝
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

private:
    std::string name_;
    MyResource resource_; // 成员变量也必须支持移动
};

// ==========================================================
// 3. 测试
// ==========================================================
int main() {
    std::cout << "--- 创建 A ---" << std::endl;
    DataManager a("Topic_Camera");

    std::cout << "\n--- 移动 A 到 B (构造) ---" << std::endl;
    DataManager b = std::move(a); 

    std::cout << "\n--- 容器操作 (emplace_back) ---" << std::endl;
    std::vector<DataManager> vec;
    // emplace_back 会在内部移动临时对象
    vec.emplace_back(DataManager("Topic_IMU"));

    std::cout << "\n--- 结束运行 ---" << std::endl;
    return 0;
}