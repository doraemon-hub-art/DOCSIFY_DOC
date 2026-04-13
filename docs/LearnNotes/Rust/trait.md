> Rust重要概念解析 —— trait

---

# 什么是Trait

即，定义一些方法，让不同的struct来实现。

类比于 C++ 中的多态，定义积累类，子类来实现其中的方法。Rust trait 如图C++继承纯虚类，要实现其中方法的struct(即，继承纯虚类)，必须将这些(纯虚)方法都实现，除非trait中对应的方法有默认实现。

---

# 示例

```rust
trait Animal {
    fn speak(&self);
    
    fn move_around(&self) {  // default
        println!("移动");
    }
}

struct Dog;

impl Animal for Dog {
    fn speak(&self) { println!("汪汪"); }
}

fn main() {
    let dog = Dog;
    dog.speak(); // 
    dog.move_around(); // Animal默认的
}
```

对应的C++代码。

```C++
class Animal {
public:
    virtual void speak() = 0;  // 纯虚函数，必须实现
    
    virtual void move_around() {  // 非纯虚函数，有默认实现
        cout << "移动" << endl;
    }
    
    void sleep() {  // 普通函数，有实现
        cout << "睡觉" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override { cout << "汪汪" << endl; }
    // move_around() 不重写也行，用基类的
    // sleep() 不重写也行，用基类的
};

```

---

# 进阶

