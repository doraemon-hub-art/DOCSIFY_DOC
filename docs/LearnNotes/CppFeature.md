> 用于记录一些工作上遇到的一些C++特性，作为简单积累。

# std::async 异步任务

C++11 引入的一个模板函数，用于**启动异步任务**，并返回一个 `std::future` 对象，用于获取任务结果。它把线程创建和异步结果管理封装起来，非常适合简单的并发场景。

> 示例

```C++
#include <future>
#include <iostream>

int compute(int x) {
    return x * 2;
}

int main() {
    // 异步执行 compute(10)
    std::future<int> fut = std::async(std::launch::async, compute, 10);

    // 其他操作可以继续进行...

    // 获取结果（会阻塞直到 compute 完成）
    int result = fut.get();
    std::cout << "result = " << result << std::endl;
}

```

---

# 聚合初始化

在 C++ 里，如果一个类型满足以下条件，就叫 **聚合类型**（C++11 定义，C++17/20 有细微变化）：

- 是 **数组** 或 **类（struct/union）**；
- 没有用户定义的构造函数（即没有写 `A(int)` 这种）；
- 没有私有或受保护的非静态数据成员；
- 没有虚函数，也没有虚基类。

简单理解：就是 **"纯数据结构"**（类似 C 语言里的 struct）。

> 示例

```C++
#include <iostream>
#include <string>

struct Point {
    int x;
    int y;
};

struct Person {
    std::string name;
    int age;
};

int main() {
    Point p1 = {1, 2};  // 聚合初始化
    Person p = {"Alice", 20};

    std::cout << p1.x << ", " << p1.y << "\n";
    std::cout << p.name << ", " << p.age << "\n";
}

```

---

# 智能指针传引用 TODO





---

# std::variant

C++17 引入的 **类型安全的联合体（type-safe union）**。

可以在一个变量里存储 **多种可能的类型**，但同一时刻只能存一个。

比 `union` 安全得多，因为会自动管理构造/析构，避免类型错误。

> 示例

```C++
#include <variant>
#include <iostream>
#include <string>

int main() {
    std::variant<int, double, std::string> v; // 可以存 int / double / string

    v = 10;       // 存 int
    std::cout << std::get<int>(v) << "\n";

    v = 3.14;     // 存 double
    std::cout << std::get<double>(v) << "\n";

    v = "hello";  // 存 string
    std::cout << std::get<std::string>(v) << "\n";
}

```

---

# std::memory_order_seq_cst (TODO ?)

> C++ 内存模型里最强的顺序保证

`std::memory_order_seq_cst` 是 **原子操作** 的一种内存序（memory order），意思是 **sequentially consistent（顺序一致性）**。

seq_cst 只能保证操作顺序一致，但不能保证“同时看到对方的更新”

**seq_cst 保证全局逻辑顺序不冲突**，为程序可组合性和多线程推理提供基础。

```C++
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
```



---

# string_view

> 避免进行隐式拷贝，在一些只读不修改的场景

`std::string_view` 从 C++17 引入以后，在 **日志打印、配置解析、协议处理** 等场景里非常常用。它的优势是 **避免拷贝字符串**，只用一个 `(ptr, length)` 的“窗口”来查看已有的字符串。

> 特点

**不会拷贝数据**，只是引用现有内存。

接受的来源比较灵活，比如：

- `std::string`
- `const char*`（字面量或 C 字符串）
- 原始 `char[]` 缓冲区
- 另一个 `string_view` 的子串

> string_view 和 string& 有啥区别？

`string_view`

- **零拷贝**：只保存地址和长度，不分配内存。

- **可绑定多种来源**：`std::string`、`char*`、字面量、数组、甚至另一个 `string_view`。

- **不可修改**：`string_view` 永远是“只读的视图”。

- **生命周期敏感**：如果底层内存释放/变动，`string_view` 就悬空。

`string`

- **绑定唯一来源**：只能绑定到一个现有 `std::string` 对象上。

- **不占空间**（一般只是一个指针的开销，编译器可能优化掉）。

- **可修改**：引用和原对象完全一样，可以调用非 const 成员函数（如 `push_back`、`clear`）。
  - 注意 const 修饰的实例化对象，只能调用const成员函数，防止修改。

- **生命周期安全**：引用和对象一样长（前提是引用没被滥用，比如返回局部变量引用）。

---

# 定义 unique_ptr 变量时，需要完整类型定义

> 为什么unique_ptr有这个问题？

`std::unique_ptr<T>` 是一个 **类模板**。

编译器在实例化 `unique_ptr<Foo>` 时，会直接把 `~unique_ptr<Foo>() { delete ptr; }` 生成出来。

当编译器在头文件里看到 `delete ptr;`，它必须确认：

- `ptr` 指向的类型大小是多少（才能生成 `delete` 代码）。
- 但是如果此时 `T` 只是 **前向声明（incomplete type）**，编译器就没法确定大小，所以报错。

这就是 `unique_ptr` 的痛点。

> 为什么shared_ptr就没有这个问题？

`shared_ptr<T>` 的实现方式不一样：

- 它的析构函数只是减少引用计数，不会立即写 `delete ptr`。
- 真正的删除动作是通过运行时的 **控制块（deleter）** 调用的。
- 所以编译器生成 `shared_ptr<T>` 时，不需要知道 `T` 的完整大小，等运行时再处理。

----

# inline 修饰的函数

在 C++ 中，`inline` 函数 **必须在头文件里同时写实现**，否则链接器找不到定义，就会报你之前的 `undefined reference` 错误。

---

# std:mutex 禁止拷贝

`std::mutex` 内部封装了操作系统的互斥量资源，这个资源和一个具体的内核对象或句柄绑定（比如 Linux 的 `pthread_mutex_t`，Windows 的 `CRITICAL_SECTION`）。

- 如果允许拷贝，会出现两个 `mutex` 对象指向同一个底层资源，但两个对象并不知道彼此共享。
- 这样可能导致：
  - **重复析构**：多个 `mutex` 析构时释放同一个资源，造成未定义行为。
  - **死锁/同步混乱**：多个对象操作同一锁，状态难以维护。
- 所以标准库明确禁用了：

```c++
std::mutex(const std::mutex&) = delete;
std::mutex& operator=(const std::mutex&) = delete;
```

> 编码时，要注意，可以将具有std::mutex成员的类，拷贝和赋值禁用掉。
>
> **但是可以移动。** ——  **约定俗成的最佳实践**

```C++
#include <mutex>
#include <string>

class Data {
public:
    Data(std::string s) : value(std::move(s)) {}

    // 禁止拷贝，允许移动
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

    Data(Data&&) = default;
    Data& operator=(Data&&) = default;

    void setValue(std::string v) {
        std::lock_guard<std::mutex> lk(mtx);
        value = std::move(v);
    }

    std::string getValue() {
        std::lock_guard<std::mutex> lk(mtx);
        return value;
    }

private:
    std::string value;
    mutable std::mutex mtx;
};

```

---

# std::lock_guard 和 std::unique_lock 的区别

> std::lock_guard 

- **轻量级封装**互斥量。

- **RAII** 风格：构造时加锁，析构时解锁。

- **不可解锁/重新锁定**，作用域结束才释放锁。

- **只能管理 `std::mutex`（或兼容 mutex 类型）**。

```C++
#include <mutex>

std::mutex mtx;

void foo() {
    std::lock_guard<std::mutex> lk(mtx); // 构造时加锁
    // 临界区
} // 析构时自动解锁
```

> std::unique_lock

- **更灵活**的锁封装。

- RAII 风格。

- **可解锁/重新加锁**，支持 `lock()` 和 `unlock()`。

- **可以延迟加锁或采用 `try_to_lock`、`adopt_lock`**。

- 必须支持 **move**（不可拷贝）。

- 支持 **条件变量**等待。

```C++
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lk(mtx); // 默认加锁
    cv.wait(lk, []{ return ready; });     // 自动解锁等待，唤醒后再加锁
    // 被唤醒时 wait 会 自动调用 lk.lock()
        
    // 临界区
} // 析构时自动解锁
```

---

