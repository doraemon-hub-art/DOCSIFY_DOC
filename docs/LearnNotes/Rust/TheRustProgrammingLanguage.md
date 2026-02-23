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
# 变异运行
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

### 复合类型；

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
  - 参数时特殊变量，是函数签名的一部分；

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

Rust最与众不同的特性，是其无需垃圾回收机制，即可保障内存安全。

通过所有权系统管理内存，编译器在编译时根据一系列的规则进行检查。如果违反了任何这些规则，程序都不能编译。

运行时，所有权系统的任何功能都不会减慢程序的运行。