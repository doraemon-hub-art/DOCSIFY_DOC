> 看菜鸟教程的一点JS笔记 https://www.runoob.com/js/js-syntax.html

---

# 概念补充

## Node.js 和 JavaScript 区别 & 联系

- JavaScript 是 Node.js的主要变成语言，Node.js提供这样一个环境，使得开发者可以使用JavaScript来开发后端应用；
- Node.js 是事件驱动和飞阻塞I/O模型，适合处理高并发、I/O密集型的任务；
- 传统服务器端编程语言如 PHP、Java、C# 等会运行在各自的服务器环境中，使用各自的编译器或解释器来执行代码。
  - Node.js 通过让 JavaScript 运行在服务器端，简化了前后端共享代码的工作，特别是在 全栈 JavaScript 开发中，前端和后端都能使用相同的语言。

---

## ES6 跟 JavaScript 什么关系 

ES6 是 JavaScript 的一个版本，是对 JavaScript 语言的一次大规模更新。
JavaScript 是基于 ECMAScript 标准的，ES6 是这个标准的一个版本，后续还有 ES7、ES8 等版本。每个版本都是对 JavaScript 语言进行更新和改进。

---



> JS 版本查看

可以根据Node.js的版本和浏览器支持的ECMAScrip标准来查看JS版本

---

# JavaScript 基本语法

> 变量定义

var 类似于 C++ 的 auto ，根据=后面表达式类型，定义var 后这个变量的类型。

- C++ 的 auto 是在编译时根据初始化表达式的类型来推导变量类型（静态类型语言）。

- JavaScript 中的 var 是动态类型的，类型是在运行时根据变量赋值来决定的。

`var`: 函数作用域，变量作用范围仅限于函数内部，会被提升函数或全局作用域的顶部，仅仅提升声明，并初始化为undefined,访问不会抛出错误。

`let`: 块级作用域，变量作用范围仅限于块级代码，例如 if语句，循环等，会提升到块级顶部，但不初始化，访问会抛出错误（暂时性死区）。

`const`: 块级作用于，同let，定义常量一但赋值无法修改，但是修饰的对象或者数组，可以修改对象的属性或是数组中的元素。

如果没用var、left、const声明，则自动转换为全局变量。

- 如果把值赋给尚未声明的变量，该变量将被自动作为 window 的一个属性。
  - window 这个概念是特指在 浏览器环境 下的全局对象，用于表示全局作用域。如果我们讨论的是 前端 JavaScript，那么 window 就是全局对象。但如果是 Node.js 等其他 JavaScript 环境，window 这个对象并不存在，它会被替代为 global 对象。

---

> 对象 —— 一个包含多个值的变量，类似于C/C++结构体

```js
const person = {
  // 属性
  name: 'Alice',
  age: 25,

  // 方法
  greet: function() {
    console.log(`Hello, my name is ${this.name} and I am ${this.age} years old.`);
  },

  // 更新属性的方法
  setAge: function(newAge) {
    this.age = newAge;
    console.log(`Age updated to ${this.age}`);
  }
};

// 使用对象的方法
person.greet();  // 输出：Hello, my name is Alice and I am 25 years old.

// 更新对象的属性
person.setAge(30);  // 输出：Age updated to 30
person.greet();  // 输出：Hello, my name is Alice and I am 30 years old.

```

---

> 变量生命周期

JavaScript 变量生命周期在它声明时初始化。

- 局部变量在函数执行完毕后销毁。
- 全局变量在页面关闭后销毁。- 

---

> 对象所有权

JavaScript中，根据数据类型的不同，赋值采用拷贝或者引用的方式。

- 原始数据类型（基本类型）：比如 number、string、boolean、null、undefined 和 symbol，赋值时是拷贝；
- 引用数据类型：比如 object、array 和 function(函数本质上是一个对象)，赋值时是引用；

---

> 闭包

`形成条件`

- 函数嵌套: 一个函数内定义另一个函数；
- 外部函数的变量被内部函数引用，即使外部函数执行完毕，内部函数仍能访问外部函数的局部变量；

```js
function outer() {
    let count = 0;  // 外部函数的局部变量
    return function inner() {
        count++;  // 内部函数修改外部函数的变量
        console.log(count);
    };
}

const counter = outer();

counter(); // 1
counter(); // 2
counter(); // 3
```

---

> 函数定义


```js
fcuntion testFunc(a, b) {
    return a * b;
}
```

---

> 条件语句

 语法同C++，不赘述。

 ```js
  if () {

  } else if () {

  } else {

  }
 ```

---

# 内容补充

> .mjs 和 .js 的区别 ？

> 本地编译检查语法错误


```bash
node xxx.mjs
```

......

 ---





