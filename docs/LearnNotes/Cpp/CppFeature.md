> 用于记录一些工作上遇到的一些C++特性，作为简单积累。

TODO: 可以定期整理几条内容，拿出来出一期视频做一做分享；
(但是，感觉我得首先在哔哩哔哩上开始看一些技术的内容，要不只单纯的输出，有些无法从观众视角出发。)

---

# std::async 异步任务

C++11 引入的一个模板函数，用于**启动异步任务**，并返回一个 `std::future` 对象，用于获取任务结果。它把**线程创建和异步结果管理封装起来**，非常适合简单的并发场景。

> 函数原型

```c++
std::future<ReturnType> f = std::async(std::launch::policy, func, args...);
```

**`std::launch::policy`**：启动策略

- `std::launch::async` → 马上创建一个线程开始执行；
- `std::launch::deferred` → 延迟调用（只有在 `future.get()` 时才执行，且在调用线程里执行）；
- 默认情况是两者的组合（实现可以选择开新线程，也可以延迟执行）。

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

# 单例模式实现

> 经典懒汉模式

```c++
class A {
public:
    static A* getInstance() {
        if (!m_a) {
            m_a = new A();
        }
        return m_a;
    }

private:
    A() {}  // 构造函数私有化
    A(const A&) = delete;
    A& operator=(const A&) = delete;

    static A* m_a;
};

// 类外定义并初始化
A* A::m_a = nullptr;

```

> 现代C++写法

```C++
static KVSController& GetInstance() {
    static KVSController instance; 
    return instance;
}

// 不需要外部初始化
// 用法
auto& g_kvs_controller = AWSKVS::KVSController::GetInstance();
```

> 为什么要引用？

1. 编译器认为你要创建一个 新的局部变量（新对象）。
   
- 创建时：它试图调用拷贝构造函数 —— ❌ 报错：禁止拷贝。
- 销毁时：它试图在函数结束时调用析构函数 —— ❌ 报错：禁止私有析构。
  -  结论：路被堵死了。

2. 如果你加了引用 (auto& ctrl = ...)

编译器认为你只是给那个唯一的单例起了一个 外号（别名）。
- 创建时：不需要创建新对象，只是指过去 —— ✅ 通过。
- 销毁时：引用只是个外号，外号消失不需要销毁对象本身 —— ✅ 通过。

---

## 成员变量必须为指针的情况

如下图所示:

![](https://oss.banshengua.top/blogimages/202512011619655.png)

Class KVSController的成员变量没有默认构造，在调用获取单例类对象，GetInstance时，编译器无法构造成员KVSSignaling，所以可以改成指针成员变量。

以推迟成员变量构造实际，例如放到Init()函数中，对成员变量完成初始化。

指针（包括 std::shared_ptr）默认初始化是 nullptr（空指针），不需要参数，这样 KVSController 就可以被无参构造了。具体的对象创建推迟到 Init 函数里去做。


---

# std::optional 空值异常情况

std::optional 是 C++17 引入的模板类，用来表示：“一个值可能存在，也可能不存在”。

> 存储一个类型 T 的变量，或者存储一个空值 nullopt

```C++
#include <optional>
#include <string>
#include <iostream>

int main() {
    std::optional<std::string> optStr; // 默认空 (nullopt)

    if (optStr.has_value()) {           // 检查是否有值
        std::cout << "Value: " << optStr.value() << "\n";
    } else {
        std::cout << "No value\n";
    }

    // 赋值
    optStr = "hello";

    // 另一种访问方式
    if (optStr) {                       // 与 has_value() 等价
        std::cout << *optStr << "\n";   // 解引用获取值
    }

    // 设置为空
    optStr = std::nullopt;
}
```

> 我的问题，当用封装syslog打印的日志宏，打印一个为空的optional变量时，发生了异常情况，但是系统并没有崩溃；

类似于:

```C++
 std::optional<std::string> str;
    // std::cout << str.value() << std::endl;
    syslog(LOG_INFO, "Test null optional str value is: %s", str.value().c_str());
```

程序并没有终止并抛出如下异常:

```bash
terminate called after throwing an instance of 'std::bad_optional_access'
  what():  bad optional access
Aborted (core dumped)
```

业务代码中中没有try catch的地方，chatgpt帮忙分析，可能是系统哪个底层框架注册了 set_terminate，然后这个没人捕获的异常被别的`hook`给吃掉了。他并没有让程序崩溃。

---

# 环形缓冲区的实现

```c++

```

---

# C++20协程

TODO


---

# 移动构造

> 示例:

```C++
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
    MyData e = makeData(1);

    return 0;
}
```

> 补充

`ROV （Return Value Optimization）` —— 返回值优化,编译器直接在调用者的内存空间里构造这个返回对象。

---

# TODO: 这是啥？继续下方的补充，回顾下这要写啥来着？


在录音线程里调用回调（比如 callback(buffer, size)）时，建议用局部拷贝方式：

---

# promise + future 做线程同步

> 示例: 

```c++
#include <iostream>
#include <thread>
#include <future>
#include <chrono>


void doWork(std::promise<int>&& prom) {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时操作
    int result = 42;
    prom.set_value(result);
    std::cout << "Task completed and result set." << std::endl;
}

int main() {
    // 创建一个 promise 对象，它将在工作线程完成时传递一个整数结果
    std::promise<int> prom;

    // 从 promise 获取 future，另一个线程会使用它来获取结果
    std::future<int> fut = prom.get_future();

    // 创建并启动工作线程
    std::thread worker(doWork, std::move(prom));

    // 主线程等待工作线程完成并获取结果
    std::cout << "Main thread waiting for result..." << std::endl;
    int result = fut.get();  // 等待并获取线程的结果
    std::cout << "Received result: " << result << std::endl;

    // 等待工作线程结束
    worker.join();

    return 0;
}
```

类似于用条件变量+锁的方式来等待某个条件到达，不过pormise + future 的等待方式没有那么多，但是有一点不错。
就是，不会像光用cv.wait()那样，notify在wait前到达，从而错过消息。
cv还需要加一个flag用于双重检查。

而当 future.get() 执行时，如果值已经被设置后，则会终止等待。不会错过消息。
否则，在此阻塞，当值被设置，终止阻塞。

---

# 一个无聊的话题，抽象类私有化纯虚函数

override 不看访问权限，只看声明可见性。

```C++
class A {
private:
    virtual void foo() = 0;
};

class B : public A {
    virtual void foo() override {  /* 可以写实现 */ }
};

```

但是如果，子类private继承父类，会看不到父类的私有纯虚函数，也就无法继承。

---

# bind 和 lambda 

> 使用场景

TODO

---

# 同一个命名空间，区分同名变量

> 例如: 打印日志的logger

在.cpp 文件中，添加匿名空间。

```c++
namespace test{
namespace {
    constexpr const char* LOGGER_ID = "kvs";
}
}
```

---

# char* 判空

> 用 == NULL，不能用strlen

strlen 的工作原理是：从传入的内存地址开始，一个字节一个字节地往后读，直到遇到 \0 为止。

---

# C 语言处理 “未知长度数据” 的标准 “两步走（Two-Pass）” 模式

> 来源: AWS KVS WebRTC C SDK, 格式化数据时的 两次 serializeSessionDescriptionInit 调用

```C++
RtcSessionDescriptionInit local_sdp_info;
MEMSET(&local_sdp_info, 0x00, SIZEOF(RtcSessionDescriptionInit));

setLocalDescription(rtc_peer_conn_pointer_, &local_sdp_info);
ret_status = createAnswer(rtc_peer_conn_pointer_, &local_sdp_info);

UINT32 serializedLen = 0;
// 第一次调用
serializeSessionDescriptionInit(&local_sdp_info, NULL, &serializedLen);

char* json_buf = (char*)malloc(serializedLen + 1);
if (json_buf != NULL) {
    // 第二次调用
    serializeSessionDescriptionInit(&local_sdp_info, json_buf, &serializedLen);
```

其中：
- 第一次调用，获取吧这个数据，转成JSON所需要的空间大小；
- 第二次调用，在用第一次获取的长度够，开辟buffer，给SDK调用传递下去，SDK会将格式化后的结果，写入到这个buffer中；

或者直接给最大长度 MAX_SIGNALING_MESSAGE_LEN ，一次调用即可。

---

# 宏不是函数，不会被命名空间限定

> 例如: 在另一个头文件，的同一个命名空间中，再添加一层命名空间，定义一个LOG宏；

调用这个宏的时候，不需要指明任何命名空间，因为命名空间不被空间限定。

```C++
namespace A {
    #define MAGIC 42
}

namespace B {
    int x = MAGIC;   // 啪！直接展开成 42
}
```

像一只藏在角落乖乖睡觉的小猫。

---

# 智能指针的impl问题

如果一个类，函数uniqut_ptr管理的一个对象，这个类型只做了前置声明，在构造这个类的时候，编译器需要知道完整的结构类型，所以这个类的构造和析构，都需要再.h中只做前置声明，实现都要放到.cpp中，且在这个成员变量class下。

---

# std::thread 操作概览

```C++
// 定义直接初始化，或者定义后，单独赋值，都会直接启动线程
std::thread t1([](){
    // some action
});

// 子线程分离，之后无法对子线程进行操作了
t1.detach();

// 阻塞，等待子线程结束
t1.join();

```

# std::condition_variable 操作概览

```C++
// 唤醒后，会再次检查后面的状态，这两天误以为notify后，会直接略过这个wait， :)

cv_.wait(lock, [&] { return !deq_.empty() || !is_running_.load(); });
```

---

# 强弱作用域枚举

> 非强作用域枚举（C++98/03 风格）

```C++
enum RTC_PEER_CONNECTION_STATE {
    CONNECTING,
    CONNECTED,
    ...
};
```

枚举值直接暴露在定义枚举的作用域中，无需指明作用域。

> 强作用域枚举（C++11 起）

```C++
enum class RTC_PEER_CONNECTION_STATE {
    CONNECTING,
    CONNECTED,
    ...
};
```

枚举值被限制在枚举类的作用域内，必须加前缀xx::。

---

# 多线程处理观察者/客户端列表

```C++
bool PushVideoFrames4AllClient(const Frame& frame, bool cache_flag) {
  std::vector<std::shared_ptr<KVSPeerConnection>> peers_copy;

  {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& [_, client_instance] : kvs_peers_) {
      peers_copy.push_back(client_instance);
    }
  }

  for (auto& client_instance : peers_copy) {
    client_instance->DirectlyWriteVideoFrame(frame);
  }

  return true;
}
```

> 好处

- 避免长时间锁住kvs_peers_,其他线程可以自由增删连接；
- 极大的减小锁的粒度；
- 防止死锁；
  - 如果 DirectlyWriteVideoFrame 引起某些内部操作也会获取锁，从而会引起死锁；
- 利用shard_ptr，保证对象声明周期的安全；

![](https://oss.banshengua.top/blogimages/202512051507679.png)

---

# 结构化绑定声明，忽略不关心的对象

> 如下示例，在遍历一个pair的时候

```C++
for (auto& [_, client_instance] : kvs_peers_) {
    peers_snapshot.emplace_back(client_instance);
}
```

可以使用 _ 来表明自己不关心的变量，并非官方语法，只是一种人们的写法。

---

# std::shard_ptr 析构时机

> 由最后一个持有引用计数的线程，做析构处理。

---

# explicit

> 禁止，隐式构造，避免编译器帮倒忙。

---

# 最小作用域原则

如果一个类（SmartFrame）只是某个特定模块（KVSPeerConnection）的“私有实现细节”，那么它确实不应该暴露在全局通用的 common.h 中，以免污染全局命名空间，或者增加不必要的编译依赖。

---

# 常量引用传参

```C++
// 例如
const SmartFrame&
```

1. & (引用 / Reference) —— 省去拷贝，高性能

- 含义：它不是创建一个新的对象，而是给原来的对象起了一个“别名”。函数内部使用的 sf，本质上就是调用者手里的那个 sf。
- 作用：避免拷贝。

如果不加 &（即 SmartFrame sf），函数调用时会触发拷贝构造函数。虽然 SmartFrame 拷贝很轻（只是增加引用计数），但这涉及到原子操作（Atomic Operation），在高频调用下依然有微小的开销。
加上 &，完全没有拷贝动作，开销为 0。

2. const (常量 / Constant) —— 只读，安全
   
- 含义：函数承诺 “我只看，我不改”。
- 作用：
  - 安全性：函数内部不能修改 sf 指向的地址，也不能重置它。调用者可以放心地把数据传进来，不用担心被改坏。
  - 通用性：允许你传入临时对象（R-value）。比如 Func(SmartFrame(...))。如果不加 const，C++ 规则不允许把临时对象绑定到非 const 引用上。

---

# C++ 封装 C no-cost API 接口

```C++
    // safe control - copy a temp object for no-cost C API
    // (PS: In such a project, there seems to be some overdesign :)
    const Frame* source_data = frame.GetRawData();
    Frame mutable_temp = *source_data;
    AWS_KVS_FUNC_CALL(
        writeFrame(video_transceiver_, &mutable_temp));  // send frame
```

- const 安全的C++接口，但是需要调用C no const 的接口，可以像这样，做一层临时浅拷贝。

---

#  C++ RAII 封装接管 C 语言裸指针资源

```C++
class DeepCopyFrame {
 public:
  DeepCopyFrame() { MEMSET(&frame_, 0x00, SIZEOF(Frame)); }

  DeepCopyFrame(const Frame* frame) : DeepCopyFrame() {
    if (!frame) return;
    CopyFrom(frame);
  }

  DeepCopyFrame(const DeepCopyFrame&) = default;
  DeepCopyFrame& operator=(const DeepCopyFrame&) = default;
  ~DeepCopyFrame() = default;

  const Frame* GetRawData() const { return &frame_; }

 private:
  void CopyFrom(const Frame* frame) {
    // copy base data
    frame_ = *frame;
    // copy byte data
    if (frame->size && frame->frameData) {
      data_ = std::shared_ptr<BYTE>(new BYTE[frame->size],
                                    std::default_delete<BYTE[]>());
      MEMCPY(data_.get(), frame->frameData, frame->size);
      frame_.frameData = data_.get();
    } else {
      frame_.frameData = nullptr;
      frame_.size = 0;
    }
  }

 private:
  Frame frame_;
  std::shared_ptr<BYTE> data_;
};
```

> 核心思想

- 深拷贝；
- std::shared_ptr 管理堆内存 ；
- C 结构体管理元数据；

> 补充说明

- 使用默认的拷贝构造函数（Default Copy Constructor），目的就是为了实现“数据的浅拷贝”和“引用计数的增加”;
  - 默认拷贝构造 = 浅拷贝数据 + 共享所有权。
    - 这就是我们想要的效果：数据只有一份，大家都持有钥匙（shared_ptr），最后一个离开房间的人负责关灯（free）。
---

# inline 修饰的全局变量重复包含问题

(全局变量的 ODR 问题)

> 例如: 取消下方公共头文件中的这个开关的inline修饰，在C++17以下就会报警告

```C++
inline bool g_local_debug_enabled = false;
```

因为，如果这个头文件被多个.cpp include

- 每个翻译单元都会定义一个全局变量；
- 链接阶段触发 multiple definition of symbol；
- 属于典型的 ODR（One Definition Rule）违规；

> C++ 14及以下解决方法:

- 头文件加extern声明，源文件中唯一一次定义；

如果是Header Only 则无法解决。

> C++ 17 解决：

inline修饰即可。

---

> 补充哪些东西允许多重定义

- inline函数，从C++98起；
  - 每个翻译单元一份，由链接器合并；
- 模板；
  - 本质上只按需生成；
- inline变量；
  - 
- static全局；
  - 内部链接；

---

# 一些编译时的名词

TODO: 

- 链接器：
- 翻译单元：

...

---

# wait_for 谓词判定时机

```C++
bool ret = cv_.wait_for(lock, 30ms, [&] {
    return !deq_.empty();
});
```

1. 代码刚刚执行到这行；
2. notify后；
3. 超时后；

都会多一次谓词的判断，谓词的结果为wait_for的返回值。

---

# 单例模式的类作为类成员

TODO:

---

# TODO: 加上昨天的几个内容


---

# 头文件重复包含

如果对应的头文件没有重复展开限制，则会发生重定义问题，例如:

```bash
/home/xuan/workspace/code/aws_kvs_demo/impl/amazon-kinesis-video-streams-webrtc-sdk-c/demo/aws_kvs_types.hpp:23:12: error: multiple definition of ‘enum class AWSKVS::types::SignalingRecvMsgType’
   23 | enum class SignalingRecvMsgType { OFFER = 0, ANSWER, ICE_CANDIDATE, UNKNOWN };
      |            ^~~~~~~~~~~~~~~~~~~~
```

```c++
#pragma once
```
**所有头文件都必须自带 include guard / #pragma once。**

---

# make_shared  和 shared_ptr 

- std::make_shared<T>(args...) 的作用是：申请一块内存存放 T，并在上面用 args... 构造 T。
  - 如果你写 make_shared<uint8_t>(ptr, deleter)，它会尝试构造一个 uint8_t 字符，把 ptr 和 deleter 强行转成字符赋值给它，这当然会报错。

- std::shared_ptr<T>(ptr, deleter) 的作用是：接管你已经 new 出来的 ptr，并在析构时调用 deleter。这是两个完全不同的机制。

```c++
    data_ = std::shared_ptr<uint8_t>(new uint8_t[size], std::default_delete<uint8_t[]>());
```

---

# header only的.hpp工具函数 “multiple definition”

问题发生在被多个.cpp包含，链接器发现同一批函数，被两个cpp各自定义了一遍，且他们不是 inline or static。

其中，头文件中的 #pragma once，只保证“一个 .cpp 内部”不重复展开。
**不能阻止，不同 .cpp 各自生成一份同名函数。**

> 解决方法 1

- 头文件拆分，将定义放到.cpp中；
  
> 解决方法 2

- 添加 inline修饰；
  - 告诉链接器，允许多个定义，只要他们完全一致；

> 解决方法 3

- static 修饰；
  - 会让函数变成内部链接，每个.cpp一份，互不冲突；
  
但代价是：

- 符号只在当前翻译单元可见；
- 不利于复用；
- 不利于调试和结构清晰；

一般只适合 .cpp 内部 helper，不适合公共 utils。

**（ODR / include / linkage 问题，只有在模块化时才会出现。）**

---

# unique_ptr 的对象管理

必须要看得见它管理对象的完整类型，所自定义的析构和构造，必须要实现。

（感觉unique_ptr给我找了不少问题了）

---

# 聚合初始化可能会导致的默认值覆盖问题

```C++
struct test {
    std::string index_;
    std::string regin_ {"ap-east"};
    ...
};

test {""};

// 这样初始化的是按顺序定义的第一个成员变量

// 保留所有默认值，不要传任何参数或什么都不写

test {};
```

---

# do while宏

TODO:

---

# const + 引用，有时候并不是最优解

例如: 函数参数

```C++
void UpdateState(const types::ControllerState new_state);
```

- 如果传递引用，底层维护的是指针，在64位操作系统上，指针占8个字节，而枚举实际上本身才占用4字节。

---

