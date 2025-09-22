# 单例模式

> 特点

保证一个类在全局范围内只有一个实例，并且提供一个全局访问点。

> 示例: 传统写法

```bash
class A {
public:
    static A* getInstance() {
        if (!m_a) { // 经典的懒汉模式
            std::lock_guard<std::mutex> lock(mtx);  // 每次都加锁
            m_a = new A();
        }
        return m_a;
    }

private:
    A() {}  // 构造函数私有化
    A(const A&) = delete;
    A& operator=(const A&) = delete;
	
	static std::mutex mtx;  // 互斥锁
    static A* m_a;
};

// 类外定义并初始化
A* A::m_a = nullptr;
```

> C++11 之后推荐用 静态局部变量写法

从 **C++11 标准开始**，C++ 保证了 **函数内部静态局部变量（local static）的初始化是线程安全的**。

```c++
class A {
public:
    static A& getInstance() {
        static A instance;  // 第一次调用时构造，之后直接返回引用
        return instance;
    }

private:
    A() {}  // 构造函数私有
    A(const A&) = delete;
    A& operator=(const A&) = delete;
};
```

---

# 工厂模式

