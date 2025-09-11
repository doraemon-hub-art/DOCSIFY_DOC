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

# int 定义初始化问题

问题源于我在一次刷题中，连续定义两个int导致 (其实一般我不用这个写法的)

```C++
int zero_cnt, one_cnt = 0;
for (auto& c : str) {
    if (c == '0') zero_cnt ++;
    else one_cnt ++;
}
```

C++ 里 int zero_cnt, one_cnt = 0; 其实只会把 one_cnt 初始化为 0，而 zero_cnt 没有初始化，它会是一个随机值（未定义行为）。

之后下方zero_cnt++后，再在下方通过此值访问数组，很可能发生越界和未定义行为。

---

# std::atomic

> 是什么？

`std::atomic` 是 C++11 引入的 **原子类型**，它保证对某个变量的 **读写/更新操作是原子的**（不可分割）。

它的实现通常依赖于 CPU 的原子指令（例如 x86 的 `lock cmpxchg`）。

用于多线程共享变量时，避免数据竞争（data race）。

> 特性

`std::atomic` 提供 **单变量的原子操作**（读写、加减、CAS）。

特性：原子性、可见性、通常无锁、不可拷贝。

- 任何单个 `atomic` 的读/写/修改操作都是原子的，不会被中断。
- 多线程同时操作同一个 `atomic<int>`，结果是确定的，不会“写一半”。

常用于计数器、标志位、无锁数据结构。

但它不能替代 `mutex`，因为不能保证多变量/多步操作的原子性。

> 基础示例

```C++
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
```

---

# 头文件中直接实现函数 被工程中多个cpp包含，引起的重复定义问题

> 问题场景

工程中 `filesystem.h`中定义并实现了 一些工具函数，例如 `ListSubdirs()`，这个.h被工程中多个.cpp中包含。

每个 `.cpp` 包含这个头文件时，编译器会在每个 `.o` 文件里都生成一份函数定义。链接器会看到 **多个定义**，就报 `multiple definition`。

> 解决

`inline` 修饰 告诉链接器：可以多次定义，但每个定义必须一致。

> 引申，为什么单独编这个.a没问题，但是往别的模块链接就会报错

我的这个工程最后会被编译成`ProjectLib`静态库。当你只编译 `ProjectLib` 静态库时，每个 `.o` 文件里都生成了 `filesystem.h` 的函数符号。

**静态库本身**不会报错，即使多个 `.o` 文件里有重复符号，因为 `.a` 文件只是打包了对象文件。

只要你不在同一链接命令里把多个 `.o` 或静态库重复引用，编译没问题。

当你把 `ProjectLib.a` 链接到另一个模块（比如最终可执行文件或另一个库）时，链接器会把 `.a` 文件里的所有 `.o` 文件符号展开到最终链接目标。

- 此时：
  - `authorization.o` 里有 `EnsureDir`
  - `test.o` 里也有 `EnsureDir`
- 链接器发现同名符号重复，就报：

```bash
multiple definition of `utils::filesystem::EnsureDir(char const*)'
```

---

# ::修饰调用外部C API

> 目的

明确告诉编译器器 **全局命名空间**（global namespace）里找 `testSDKFunction` 这个函数，而不是当前类里、或者其他命名空间里的同名函数。

> 为什么

- **避免歧义**
  假设类里或者某个命名空间里也有 `testSDKFunction`，直接写可能会优先匹配到错误的版本。加 `::` 就强制指定用全局的函数。

- **明确调用外部 C API**
  通常 `testSDKFunction` 这种名字，很像是 **外部 SDK 提供的 C 接口函数**，不属于类或者 C++ namespace，所以需要用 `::` 指定全局

---

# Impl实现风格

**Impl 实现风格**（也叫 **PImpl Idiom / Pointer to Implementation**）在 C++ 里很常见，主要用于：

> **隐藏实现细节**

对于引用这个头文件的人，防止他窥探其中的实现细节。

> **减少头文件依赖**

```C++
// Foo.h
#include <vector>
#include <string>
#include <map>

class Foo {
public:
    void doSomething();
private:
    std::vector<std::string> data_;
    std::map<int, std::string> dict_;
};
```

因为成员里直接用了 `std::vector` 和 `std::map`，所以头文件必须包含 `<vector>`、`<map>`。

那么，所有 `#include "Foo.h"` 的文件，都会间接依赖 `<vector>`、`<map>`。

如果以后你把 `dict_` 改成 `std::unordered_map`，那么所有依赖 `Foo.h` 的文件都得 **重新编译**。

- 用Impl实现: 

用户只看到 `Foo` 的接口，不会依赖 `FooImpl` 的实现细节。

如果以后把 `dict_` 改成 `std::unordered_map`，只需要重新编译 `Foo.cpp`，**不会影响**使用 `Foo.h` 的用户。

(实际上加快编译速度和减少头文件依赖，是一回事，减少头文件依赖是手段，加快编译速度是结果。)

> **加快编译速度**

编译速度和 **依赖数量** 强相关。

在大型工程里，某个类的实现可能依赖一堆头文件（`<vector>`、`<map>`、第三方库头文件）。

如果这些依赖直接出现在 `.h` 里，那么凡是包含这个 `.h` 的地方，都会被迫编译这些庞大的头文件。

当项目很大时，这种连锁效应会让编译非常慢。

- 而 PImpl 的做法是：

`.h` 文件只暴露接口，不暴露实现 → 依赖最小化。

`.cpp` 文件才包含真正的实现依赖。

这样，改动实现时，只需重新编译 `.cpp`，而不是所有依赖这个 `.h` 的地方。

> 实例

```C++
#ifndef FOO_H
#define FOO_H

#include <memory>
#include <string>

// 前置声明（forward declaration）
// 告诉编译器有一个 FooImpl 类存在，但不需要知道它的完整定义
class FooImpl;

class Foo {
public:
    Foo();
    ~Foo(); // 注意这里虚构函数不能在头文件中实现，涉及到unique_ptr对不完整类型的析构。

    void doSomething(const std::string& msg);

private:
    // 用智能指针（unique_ptr）指向内部实现
    std::unique_ptr<FooImpl> impl_;
};

#endif // FOO_H
```

```C++
#include "Foo.h"
#include <iostream>

// 内部实现类定义，只在实现文件里可见
class FooImpl {
public:
    void doSomethingImpl(const std::string& msg) {
        std::cout << "FooImpl: " << msg << std::endl;
    }
};

Foo::Foo() : impl_(std::make_unique<FooImpl>()) {}

Foo::~Foo() = default;

void Foo::doSomething(const std::string& msg) {
    impl_->doSomethingImpl(msg);
}
```

> 补充

- 为什么一定要用指针或者智能指针，不能用内部类对象？

这里 `FooImpl` 只是 **前置声明**，编译器并不知道它的大小。

但类的成员对象 `impl_` 必须在编译期确定大小（对象布局固定），所以编译器会报错：

而指针的大小是固定的（通常 8 字节 on x64）。

即使 FooImpl 只是前置声明，编译器也知道 unique_ptr< FooImp l> 自身的大小。

因此可以把它放进对象布局里，等到析构/构造时再去处理真正的 FooImpl。

---

# CMAKE报找不到库的错误，如何解决？

分两种情况讨论

> 第三方库找不到

- 库不在系统默认路径（`/usr/lib`, `/usr/local/lib`）；

- 没有安装对应的开发包（只装了 `.so`，没装头文件 `.h`）；

- CMake 没有找到路径；

```cmake
find_package(fmt REQUIRED)
target_link_libraries(myapp PRIVATE fmt::fmt)
```

> 当前工程其他模块的库找不到

- 上下游模块之间没有正确声明依赖关系；

- 用了 `PRIVATE`，但上层 target 需要依赖；

- 没有 `add_subdirectory`，导致 target 根本没编译；

```cmake
add_library(core core.cpp)
add_library(utils utils.cpp)
target_link_libraries(utils PUBLIC core)   # utils 依赖 core，且向外公开
add_executable(app main.cpp)
target_link_libraries(app PRIVATE utils)   # app 依赖 utils，自动继承 core
```

---

# 单线程的消费任务队列，执行顺序没有严格按照FIFO



---
# lambda表达式无法显示捕获类私有方法，直接捕获this即可

把成员变量按 局部变量名 捕获，这在 C++ 里是非法的，因为 成员变量 不是独立的局部变量，而是 this->成员变量。

**捕获 `this` 指针**

- 写法：`[this]` 或 `[=]`（隐式包含 `this`）。
- 含义：捕获当前对象指针 → 在 Lambda 内可以访问对象的所有成员（包括私有成员）。

```C++
class A {
    int x = 42;
public:
    void f() {
        auto lam = [this]() {
            return x; // 合法，访问 this->x
        };
    }
};
```

---

