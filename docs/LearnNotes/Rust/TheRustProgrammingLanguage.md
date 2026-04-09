> 《The Rust Programming Language》 即 《Rust程序设计语言》内容学习

- 中文 https://kaisery.github.io/trpl-zh-cn/ch01-01-installation.html
- 英文 https://doc.rust-lang.org/book/

---

# 环境配置

- linux

```bash
# 安装Rust + 编译器
curl --proto '=https' --tlsv1.2 https://sh.rustup.rs -sSf | sh

# 安装编译时依赖的链接器
sudo apt install build-essential
```

- windows安装

按照这个即可 —— https://rust-lang.org/zh-CN/tools/install/

- 查看安装是否成功
  
```
rustc --version
```

---

# 基本语法

## hello world

```rust
fn main() {
  println!("hello world!");
}
```

- main函数依旧时程序的入口；
- 函数体用{}包括；
- 代码风格格式化同样有类似于lclang-format的rustfmt；
- ! 为调用宏，而不是函数；
- Rust为预编译静态类型语言；
  - 即，编译出来的程序，可以直接发送给别人，他们甚至不需要安装Rust就可以运行；

---

## hello cargo

- Rust的构建系统和包管理器；
  - 它可以构建代码、下载以来并编译这些库 —— 即依赖；
- cargo new xx;
  - 会默认生成toml配置文件，并默认初始化一个git仓库，可以用--vcs=git来覆盖当前目录已有的git；、
- 它期望源文件放到src目录中，项目根目录中只放README和license信息等跟代码无关的文件；
  - 如果开始没有用cargo创建项目，但是后来想用cargo来管理，可以调整为这个格式；

```bash
# 编译运行
cargo builld
cargo run
# 可以简写为b r
```

- cargo run 可以一键编译并运行；
- cargo本身就存在增量编译，如果它发现文件并没有被下修改，就不会重新构建；
- 并且 cargo提供一个check命令，用来快速检查代码是否可以编译通过，但并不产生可执行文件；
  - 在build的基础上，省略了可执行文件的生成；

```bash
cargo check
# 同样可以简写为c
```

- 无论时什么系统，cargo的命令都是一样的；

---

## guess game

```rust
use std::io;

fn main() {
    println!("Guess the number!");

    println!("Please input your guess.");

    let mut guess = String::new();

    io::stdin()
        .read_line(&mut guess)
        .expect("Failed to read line");

    println!("You guessed: {guess}");
}
```

- suing std:: 引入标准库；
  - 没有显示引入，下方使用时可以添加std::io::stdin来调用该函数；
- rust中变量默认是无法修改的，需要mut修饰来修改变量；
- ::new，表示new为String类型的一个关联函数；
  - 关联函数式针对某个类型实现的函数；
- &为引用，允许多处代码访问同一处数据，无需在内存中多次拷贝；
  - Rust的一个主要优势就是简单而安全的操纵引用；
  - 如果没有处理返回值的错误，在编译的时候Rust同样会发出警告；
- {} 可以进行格式化输出，或想类似于shell那样用变量替换；
- crate 类似于Rust中第三方库的概念；
- 构建成功后，cargo会在本地保留一个cargo.lock文件，用来记录一次的完整构建版本信息，用于回溯；
  - 只有当依赖配置发生变化时，才会更新；
- match表达式，即C/C++的switch case，默认添加了break，当满足一个条件后，就会终止；
- 重复创建一个同名的变量，可以进行覆盖 —— 遮蔽；
  - 除了让rust进行自动类型推导，我们也能在创建的时候通过:来进行手动声明；
- 返回Result类型的函数，可以通过调用expect方法，来处理异常情况，Err则打印expect中传递的字符串并终止程序，Ok则继续执行程序；
- _ 为通配符值；

```bash
  io::stdin()
      .read_line(&mut guess)
      .expect("Failed to read line");

  let guess: u32 = match guess.trim().parse() {
      Ok(num) => num,
      Err(_) => continue,
  };
```

- 实例两种Resulf返回值的用法；
  - 直接expect退出；
  - 自定义返回值逻辑判断处理；

---

## 变量与可变性

> 变量

- Rust编译器保证，如何声明一个值不会变，那么它就真的不会变；
  - 默认创建一个变量，是无法修改的，需要添加mut修饰，来这名变量可变；

> 常量

- 类似于不可变变量，但是无法对常量使用mut；
  - 它永远都是不可变的；
- Rust对常量的命名规范时大写 + _；
- 在声明它的作用域中，常量在整个程序生命周期中都有效；
  - 这个属性使得常量可以作为多处代码使用的全局范围的值；
  - 即，将遍布于应用程序中的硬编码值声明为常量，能帮助后来的开发了解值的意图；

> 遮蔽

- 即，重新定义一个重名的变量；
  - 生命周期限制在声明它的范围内；

```Rust
let c = 1;
let c = 2;
```

- 与mut不同的是，不只是能修改同名变量对应的值，还可以修改它的类型；

---

## 数据类型

- Rust是静态类型语言，在编译的时候必须指定所有变量的类型；
  - 根据值及其使用方式，编译器通常可以推断出我们想用的类型；
    - 当多种类型均有可能时，需要我们显式指名对应的类型，否则编译器就会报错；
      - 可以理解为，默认就是C++的AUTO，但是具有多种可能的时候需要我们显式声明；

```Rust
let guess: u32 = "42".parse().expect("Not a number!");
```

### 标量类型

- 标量类型，代表一个单独的值；
  - Rust的四种标量类型: 整型、浮点型、布尔类型、字符类型；

- 整型；
  - i开头为有符号，u开头为无符号整型；
    - 例如: i8 u16......
      - 有无符号，代表数字能否为负值；
  - 当发生整型溢出时，不同的编译模式，编译器的处理方式也不同；
    - 在Dbug模式下，发生整型溢出，编译器会直接抛出错误；
    - 在Release模式中，Rust不会抛出错误，当发生溢出时，比如超出最大值，会回到最小值重新开始计值；

- 浮点型；
  - f32和f64，64的精度更高，所有的浮点型都是有符号的；
  - 使用IEEE-754标准表示；
    - f32是单精度浮点数，f64是双精度浮点数；

- 数值运算；
  - Rust中所有的数字类型都支持基本的数学运算；
    - 整数出发会向零舍入到最近接的整数；

- 布尔类型；
  - 主要应用场景式条件表达式；

- 字符类型；
  - char 是Rust中最原始的字母类型；
  - Rust中用单引号声明字符，一个字符大小为4字节，代表一个Unicode标量值；

### 复合类型

- 复合类型；
  - 可以将多个值组合成一个类型；
  - Rust有两个原生的复合类型，元组，数组；

- 元组类型；
  - 是将多个不同类型的值组合进一个复合类型的主要方式；
  - 长度固定，一旦声明，长度无法增大或缩小；

```rust
let tup: (i32, f64, u8) = (500, 6.4, 1);

//  使用模式匹配来结构元组值
let (x, y, z) = tup;

// 或使用.来解析
let x = tup.1;
let y = tup.2;
let z = tup.3;
```

不带任何值的元组，叫做单元元组；

- 数组类型；
  - 与元组不同的是，数组中的每个元素类型必须相同；
    - Rust中的数组长度也是固定的；

```Rust
  let a = [1, 2, 3, 4, 5];
  let a: [i32; 5] = [1, 2, 3, 4, 5];
  // 3 3 3 3 3
  // 值，数量
  let a = [3; 5];
```

数组是在栈上分配的已知固定大小的单个内存块，可以使用索引来访问，从0开始。

当索引访问一个元素时，Rust会检查指定索引实际否小于数组的长度，当越界访问时，会抛出异常。

---

## 函数

- Rust中函数的命名使用小写字母 + _组合；

```Rust
fn main() {
    println!("Hello, world!");

    another_function();
}

fn another_function() {
    println!("Another function.");
}
```

- Rust不关心函数定义所在的位置，只要函数被调用时出现在调用处可见的作用域内即可；

- 参数；
  - 参数是特殊变量，是函数签名的一部分；

```Rust
fn another_function(x: i32) {
    println!("The value of x is: {x}");
}
```

函数签名中，必须声明每个参数的类型。

- 语句和表达式；
  - 语句是执行一些操作单不返回值的指令；
  - 表达式计算并产生一个值；

Rust中，赋值语句不会返回所赋的值。

```Rust
//  这样就是错误的
fn main() {
    let x = (let y = 6);
}
```

用大括号创建的一个新的块作用域也是一个表达式。

```Rust
{
    let x = 3;
    x + 1
}
```

其中，x + 1没有分号，因为表达式的结尾没有分号。

- 具有返回值的函数；
  - 通过， -> 声明返回值的类型；
  - Rust中，函数的返回值等同于函数体最后一个表达式的值；
  - 使用return关键字和指定值，可以从函数中提前返回；

```Rust
fn main() {
    let x = plus_one(5);

    println!("The value of x is: {x}");
}

fn plus_one(x: i32) -> i32 {
    x + 1
}
```

---

## 控制流

- if 语句；
  - 格式上相比于C/C++ 条件语句上没有 () 报告；

```Rust
fn main() {
  let number = 3;

  if number < 5 {
      println!("condition was true");
  } else {
      println!("condition was false");
  }
}
```

以及，条件必须是bool值，不像C/C++那样可以使用int型的值，比如非0，就都是true。

Rust不会尝试自动地将非布尔值转换为布尔值，必须显式地使用布尔值作为if条件。

感觉也是为了安全考虑吧。

```Rust
// 这样就是错的
if number {
  
}

// 对的
if number != 0 {

}
```

if是一个表达式，也可以给变量赋值，所以我们可以在let语句的右侧使用它。

但是需要注意的是，if表达式各个分支的值，类型必须要一致。

Rust需要在编译的时候就确切的知道 number 变量的类型，这样它就可以在编译时验证在每处使用的number变量的类型是有效的。

Rust无法在运行时确认number的类型。

```Rust
  let number = if condition { 5 } else { 6 };
```

- 循环；
  - Rust有三种循环，loop while for

可以在停止循环的break表达式后添加返回值，将结果赋值给变量。

```Rust
fn main() {
    let mut counter = 0;

    let result = loop {
        counter += 1;

        if counter == 10 {
            break counter * 2;
        }
    };

    println!("The result is {result}");
}
```

可以给循环添加标签，在嵌套循环中，操作指定循环。

如下所示，内存循环中，操作标签 counting_up ，在内部直接结束掉外层循环。

```Rust
fn main() {
    let mut count = 0;
    'counting_up: loop {
        println!("count = {count}");
        let mut remaining = 10;

        loop {
            println!("remaining = {remaining}");
            if remaining == 9 {
                break;
            }
            if count == 2 {
                break 'counting_up;
            }
            remaining -= 1;
        }

        count += 1;
    }
    println!("End count = {count}");
}
```

while 条件循环，根据条件表达式结果判断循环是否继续。

如下所示，这种结构消除了很多使用 loop、if、else 和 break 时所必须的嵌套，这样更加清晰。当条件为 true 就执行，否则退出循环。

```Rust
fn main() {
    let mut number = 3;

    while number != 0 {
        println!("{number}!");

        number -= 1;
    }

    println!("LIFTOFF!!!");
}
```

for 循环遍历元素，使用非索引方式访问，避免index访问越界问题。

```Rust
fn main() {
    let a = [10, 20, 30, 40, 50];

    for element in a {
        println!("the value is: {element}");
    }
}
```

---

# 所有权

Rust最与众不同的特性，使其无需垃圾回收机制(GC)，即可保障内存安全。

通过所有权系统管理内存，编译器在编译时根据一系列的规则进行检查。如果违反了任何这些规则，程序都不能编译。

运行时，所有权系统的任何功能都不会减慢程序的运行。

> 总结

- 对于编译器已知大小的变量，即在栈上开辟空间的变量；
  - 对其进行的同类型变量赋值操作，为正常的值拷贝；
- 对于需要再堆上开辟的变量，例如String；
  - 一个String类型变量，赋值给另一个时，会进行C++的移动(std::move),详见下方；
    - 原本的String变量不在有效，不能对其进行任何操作；
    - 离开作用域后，编译器会默认调用其drop方法，释放掉堆上的内存；
  - 如果需要进行深拷贝，需要调用.clone()方法；

与C++ std::move的对比

- Rust：编译期强制，move 后原变量不可用；
- C++：运行期约定，move 后原对象处于"合法但不确定"状态，程序员自己保证不误用；

调用函数传参，与上述所说的赋值同理。

> 示例

```rust
fn main() {
    let s1 = gives_ownership();        // gives_ownership 将它的返回值传递给 s1

    let s2 = String::from("hello");    // s2 进入作用域

    let s3 = takes_and_gives_back(s2); // s2 被传入 takes_and_gives_back, 
                                       // 它的返回值又传递给 s3
} // 此处，s3 移出作用域并被丢弃。s2 被 move，所以无事发生
  // s1 移出作用域并被丢弃

fn gives_ownership() -> String {       // gives_ownership 将会把返回值传入
                                       // 调用它的函数

    let some_string = String::from("yours"); // some_string 进入作用域

    some_string                        // 返回 some_string 并将其移至调用函数
}

// 该函数将传入字符串并返回该值
fn takes_and_gives_back(a_string: String) -> String {
    // a_string 进入作用域

    a_string  // 返回 a_string 并移出给调用的函数
}
```

---

## 引用和借用

& 符号表示 引用，它们让你引用某个值而不取得它的所有权。

引用变量离开作用域后什么也不会发生。

创建一个引用的行为称为借用。

变量默认是不可变的，引用默认也是不可变的。

- 可以添加mut修饰即可修改，即 —— 可变引用，但是同一个作用域内:
  - 对于同一个变量，只能存在一个可变引用；
    - 可变引用和不可变引用，会根据使用范围进行限制；
    - 不可变引用在最后一次使用之前，不可以出现可变引用；
  - 可以同时存在多个不可变引用；
    - 例如只读不需要加锁，**这是RUST控制数据安全的一种方式；**
    - 

> 示例

```C++
fn main() {
    let s1 = String::from("hello");

    let len = calculate_length(&s1);

    println!("The length of '{s1}' is {len}.");
}

fn calculate_length(s: &String) -> usize {
    s.len()
}
```

可变引用

```rust
fn main() {
    let mut s = String::from("hello");

    change(&mut s); // 创建可变引用
}

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}
```

可变引用不可变引用混用错误

```rust
    let mut s = String::from("hello");

    let r1 = &s; // 没问题
    let r2 = &s; // 没问题
    let r3 = &mut s; // 大问题

    println!("{r1}, {r2}, and {r3}");
```

可变引用不可变引用正确同时使用

```rust
   let mut s = String::from("hello");

    let r1 = &s; // 没问题
    let r2 = &s; // 没问题
    println!("{r1} and {r2}");
    // 此位置之后 r1 和 r2 不再使用

    let r3 = &mut s; // 没问题
    println!("{r3}");
```

---

### 悬垂引用

悬垂引用，即类似于C++中的悬垂指针，即，指针只指向一个已经释放了的一块内存。

如下示例所示，函数内部返回变量引用是无效的，因为离开函数作用域后变量已经无效，即悬垂引用。

rust会在编译期就发现这个问题。

```rust
fn dangle() -> &String { // dangle 返回一个字符串的引用

    let s = String::from("hello"); // s 是一个新字符串

    &s // 返回字符串 s 的引用
} // 这里 s 离开作用域并被丢弃。其内存被释放。
  // 危险！
```

---

## 切片

切片（slice）允许你引用集合中一段连续的元素序列，而不用引用整个集合。slice 是一种引用，所以它不拥有所有权。

字符串切片是String中一部分值的引用。

字符串字面值就是 slice，如下所示，s为指向二进制程序中特定位置的slice，不可变引用。

```rust
let s = "Hello, world!";
```

获取字符串切片后，后续使用该切片时必须保证其有效的。

> 示例

一个返回字符串切片的函数

```rust
fn first_word(s: &String) -> &str {
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }

    &s[..]
}
```

rust中，拥有数据的所有者，在离开作用域后，自动清除其数据的功能意味着你无需额外编写和调试相关控制代码。

**所有权系统系统，贯穿余下的内容。**

---

# 结构体

结构体是一个自定义数据，通过组装多个变量，从而形成一个有意义的组合类型。

结构体中的数据的名字类型，称为字段。

实例时的赋值顺序不需要和在结构体定义中的声明顺序一致。

  - 如果结构体实例是可变的，可以通过.来给其中某个字段赋值；
    - 可变，只能时整个实例，而不是其中的某个字段；
  - 为函数参数使用与结构体字段相同的名字是很合理的，例如: username : username;
    - 更简洁的写法，如果外部参数与结构体内部关键字名字相同，可以直接写成 username；
    - 为了复用，可以直接使用结构体更新语法来创建新的结构体实例；
      - 会发生数据移动，具体数据变化，需要根据具体的类型来定，例如:
        - 原本的栈空间上的变量，Copy trait类型，仍可以使用；
        - 堆空间上的，例如: String类型变量发生了复用，则旧的实例无法使用；
          - 发生了所有权转移；

```rust
struct User {
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}

fn main() {
    let user1 = User {
        active: true,
        username: String::from("someusername123"),
        email: String::from("someone@example.com"),
        sign_in_count: 1,
    };
    user1.email = String::from("anotheremail@example.com");
    // 使用 结构更新语法 通过已有的User实例创建新的实例
    let user2 = User {
        email: String::from("another@example.com"),
        ..user1
    };
}
```

---

## 元组结构体

类似于元祖的结构体，即，省略结构体的成员变量名。

```rust
struct Color(i32, i32, i32);

fn main() {
  let black = Color(0, 0, 0);
}
```

---

## 类单元结构体


即没有任何字段的结构体，被称为类单元结构体。

---

## 方法

使用 fn 关键字 + 名称声明，可以拥有参数和返回值。第一个参数为self，代表调用该方法的结构体实例。

- self 实际上是 selft : &Self 的缩写；

### impl块

impl块中，可以定义与你类型相互关联的函数，方法是一种相关联的函数，让指定结构体的实例所具有的行为。
- 每个结构体都允许拥有多个impl块；
- impl块中的方法，通过::方法；

```rust
impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }
}
```

---

# 枚举和模式匹配

列举某个类型所有可能的变体来定义这个类型。

## 枚举

一般定义，可以不指定默认的第一个起始值。

```rust
enum IpAddrKind {
    V4,
    V6,
}
```

进一步，与C++不同的是，枚举中的可选值，可被附加数据，例如:

  - 并且每个变体可以处理不同类型和数量的数据；
  - 和结构体一样，可以使用impl来为枚举定义方法；

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

impl Message {
    fn call(&self) {
        // 在这里定义方法体
    }
}

let m = Message::Write(String::from("hello"));
m.call();

enum IpAddr {
  V4(String),
  V6(String),
  v41(u8, u8, u8, u8),
}

let home = IpAddr::V4(String::from("127.0.0.1"));
```

---

### Option

在 C++ 或 Java 中，一个变量可以是 null，这经常导致“空指针异常”。

Rust 为了安全，规定如果你觉得一个变量可能为空，你必须把它包在 Option 容器里：

- 如果你确定有值，用 Some(数据)。
- 如果你确定没值，用 None。

---

## match 控制流结构

不同于 C++ ，rust不允许"贯穿",不必写break来终止case的执行。

=> 来进入执行分支，可写可不写 {}。

可以根据根据枚举绑定值的定义，获取对应的绑定值。

match的分支，必须覆盖了所有的可能性，即满足穷尽性要求。

  - 没有显式指定的可选值，可用other来表明其他情况；
    - 或者使用 _ 来匹配这些情况；
      - 这是一个特殊的模式，匹配任意值而不绑定到该值，即不使用这个值，可以做其他处理；
      - 在次基础上使用()，不做任何处理；
```rust
enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(u8),
}

fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => {
            println!("Lucky penny!");
            1
        }
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(value) => {
          println!("{value}");
          25
        }
    }
}
fn main() {
    value_in_cents(Coin::Penny);
    value_in_cents(Coin::Quarter(10));
}
```

---

## if let 和 let else 简洁控制流

if let 语法让我们以一种不那么冗长的方式结合 if 和 let，来处理只匹配一个模式的值而忽略其他模式
的情况。

使用 if let 意味着更少的输入、更少的缩进，也更少的样板代码。然而，这样也会失去 match 所强制的穷尽性检查。

```rust
let config_max = Some(3u8);
// max绑定为Some中的值
if let Some(max) = config_max {
    println!("The maximum is configured to be {max}");
}
```

### let...else

这种写法能让函数主体沿着“愉快路径”（“Happy Path”）继续向下，而不必像 if let 那样让两个分支具有明显不同的控制流。

```rust
let Coin::Quarter(state) = coin else {
      return None;
};
```

---

# 包、Crates 与模块

- 包(Packages): Cargo的功能，它允许你构建、测和分享Crate;
- Crates: 一个模块树，可以产生一个库或可执行文件；
- 模块(Modules) 和 use: 允许你控制作用域和路径的私有性；
- 路径(path): 一个为，例如: 结构体、函数或模块等项命名的方式；

---

## 包和Crate

- crate 是 Rust编译器每次处理的最小代码单位；
- crate 可以包含模块，而这些模块也可以定义在其他文件中，并与该 crate 一起编译；
- crate 有两种形式，二进制crate和库crate（C++ lib）;

- 包是提供一系列功能的一个或者多个crate的捆绑；
  - 一个包会包含一个Cargo.toml问及那，其中会阐述如何去构建这些crate；
  - crate实际上就是一包，它包含了用于构建你代码的命令行工具的二进制crate；

---

## 作用域与私有性

TODO: 模块相关。

本章适用于更大的项目进行模块拆分，暂时跳过，太无聊了。

---


---

# 常见集合

## Vector

- 在堆空间存储数据；
- 连续的内存，存储多个同一类型的数据；
- 同样默认是不可更改的，需要再创建的时候mut修饰；
- 元素访问:
  - 可通过[]索引，或者get方法，主要&，防止掠夺所有权；
    - get方法返回Option，指定的所有超过作用域后不会抛异常，只会返回None;
    - []访问超过范围的索引直接抛异常 panic 终止程序；
- 注意: 同一个作用域内仍不可同时存在可变引用和不可变引用；
  - vector会把元素放到相邻的内存空间中，当当前存放的位置，内存空间不够了，就会将原本的vector搬走，找到更大的一块连续空间放下，这时原来元素的引用，指向的就是一块已经释放的内存；
  - **借用规则，就是为了防止这个问题**；
- 存储多个类型的值:
  - 可以在Vector中存储枚举以实现；
  - 同时仍要注意，**编码时，要考虑到所有可能的类型；**

```rust
// 空vector
let v: Vec<i32> = Vec::new();

// 初始值 + 自动推导
let v = vec![1, 2, 3];

// 可修改 + 创建后添加变量
let mut v = Vec::new();
v.push(5);

// 遍历
let v = vec![100, 32, 57];
for i in &v {
    println!("{i}");
}
let mut v = vec![100, 32, 57];
for i in &mut v {
    *i += 50;
}
```

---

## 字符串

- String 是一个 Vec<u8> 的封装；
  - 无法使用index索引[xx]访问；
    - 以utf-8编码存储，每个unicode标量需要值需要两个字节存储，索引访问返回的并不是第一个字符，会有误会，所有Rust为了杜绝这种误会，禁止索引访问字符串；
    - 有效的 Unicode 标量值可能会由不止一个字节组成;
    - 并且，索引操作预期的时间复杂度通常是常数时间O(1)，但是String无法保证这样的性能，因为Rust必须从开头到索引为止遍历来确定有多少有效字符；
- 操作字符串的最好办法，是明表示需要字符还是直接；

```rust
let hello = String::from("Hola");
let hello1 = String::from("Здравствуйте");
let hello2 = "Здравствуйте";

for c in "Зд".chars() {
    println!("{c}");
}
for b in "Зд".bytes() {
    println!("{b}");
}
```

---

## Hash Map

- 通过哈希函数来实现映射，用于存储键值对 HashMap<K, V>；
- 遍历哈希 map 会以任意顺序进行;
- 存储在栈空间的元素，会被拷贝进Map，堆空间上的元素，会被移动进Map;
  - 一旦键值对被插入后就为哈希 Map 所拥有;
  - 将键值对引用插入Map，该键值对需要在Map有效的那段时间中一直有效；
- entry方法， 传入一个指定的key，返回一个Entry，它表示一个可能存在也可能不存在的值；
  - entry 上的 or_insert 方法被定义为：如果对应 Entry 的键已经存在，就返回该值的可变引用；如果不存在，就把参数作为这个键的新值插入，并返回这个新值的可变引用;
- 可以自定义Hash函数，以实现更快的性能；

```rust
use std::collections::HashMap;

let mut scores = HashMap::new();
scores.insert(String::from("Blue"), 10);
scores.insert(String::from("Yellow"), 50);

let team_name = String::from("Blue");
let score = scores.get(&team_name).copied().unwrap_or(0);

// 依旧引用访问 key 和 value 已经是引用了
for (key, value) in &scores {
  // println 自动会解引用，当然也可以手动显式解引用
  println!("{key}: {value}");
}

scores.entry(String::from("Yellow")).or_insert(50);
scores.entry(String::from("Blue")).or_insert(50);
```
---

# 错误处理

- Rust 没有异常；
  - 它使用 Result<T, E> 类型来处理可恢复错误；
  - 使用 panic! 宏在程序遇到不可恢复错误时停止执行；

---

## painic! 处理不可恢复的错误

(TODO: 在编译阶段RUST编译器可以很大程度上减少错误，为什么仍会发生类似于数组越界访问的问题?)

- painic时，有两种动作，可通过toml [profile.release] 配置其行为；
  - 展开，清理所有资源；
  - 直接终止；
- 可以根据触发 panic! 的函数 backtrace，来分析问题；
  - backtrace 是一份到达当前执行点之前所有被调用函数的列表；
  - 要获取带有详细信息的backtrace，需要再debug模式下,即不带 --release 参数运行cargo build；

```rust
RUST_BACKTRACE=1 cargo run
```

---

## Result处理可恢复的错误

- 为了避免处理返回值时的过多match匹配，可以使用闭包来使其更加优雅；
- Result<T, E> 类型定义了很多辅助方法来处理各种更为特定的任务；
  - unwarp 如果 Result 值是变体 Ok，unwrap 会返回 Ok 中的值。如果 Result 是变体 Err，unwrap 会为我们调用 panic!；
  - expect 方法也允许我们自定义 panic! 的错误信息；
  - 在生产级别的代码中，大部分 Rustaceans 选择 expect 而不是 unwrap 并提供更多关于为何操作期望是一直成功的上下文；

```rust
use std::fs::File;

fn main() {
    let greeting_file_result = File::open("hello.txt");

    let greeting_file = match greeting_file_result {
        Ok(file) => file,
        Err(error) => panic!("Problem opening the file: {error:?}"),
    };
}
```

---

## 错误传播

- 返回错误给调用者；
- ？ 运算符可以进一步简化 match表达式；
  - 如果 Result 的值是 Ok，这个表达式就会返回 Ok 中的值，程序继续执行；
  - 如果值是 Err，Err 就会像使用了 return 关键字一样，作为整个函数的返回值提前返回；
  - 可以在 ？ 运算符后面继续执行操作，进一步简化代码，如果？前错误，则会提前返回，否则继续执行？后的内容；
  - ? 运算符只能被用于返回值与 ? 作用的值相兼容的函数；

```rust
use std::fs::File;
use std::io::{self, Read};

fn read_username_from_file() -> Result<String, io::Error> {
    let mut username_file = File::open("hello.txt")?;
    let mut username = String::new();
    username_file.read_to_string(&mut username)?;
    Ok(username)
}
```

---

## 要不要返回painc!

- 返回 Result 是定义可能会失败的函数的一个好的默认选择;
- 在当有可能会导致有害状态（bad state）的情况下建议使用 panic!;

Rust 的错误处理功能被设计为帮助你编写更加健壮的代码。

---


开始做东西了，这本书就看到这吧......





