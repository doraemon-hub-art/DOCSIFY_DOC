> 看菜鸟教程的一点JS笔记 https://www.runoob.com/js/js-syntax.html

---

# 补充

## Node.js 和 JavaScript 区别 & 联系

- JavaScript 是 Node.js的主要变成语言，Node.js提供这样一个环境，使得开发者可以使用JavaScript来开发后端应用；
- Node.js 是事件驱动和飞阻塞I/O模型，适合处理高并发、I/O密集型的任务；
- 传统服务器端编程语言如 PHP、Java、C# 等会运行在各自的服务器环境中，使用各自的编译器或解释器来执行代码。
  - Node.js 通过让 JavaScript 运行在服务器端，简化了前后端共享代码的工作，特别是在 全栈 JavaScript 开发中，前端和后端都能使用相同的语言。

---

## ES6 跟 JavaScript 什么关系 

ES6 是 JavaScript 的一个版本，是对 JavaScript 语言的一次大规模更新。
JavaScript 是基于 ECMAScript 标准的，ES6 是这个标准的一个版本，后续还有 ES7、ES8 等版本。每个版本都是对 JavaScript 语言进行更新和改进。

> JS 版本查看

可以根据Node.js的版本和浏览器支持的ECMAScrip标准来查看JS版本

---

# JavaScript 基本语法

> 变量定义

var 类似于 C++ 的 auto ，根据=后面表达式类型，定义var 后这个变量的类型。

- C++ 的 auto 是在编译时根据初始化表达式的类型来推导变量类型（静态类型语言）。

- JavaScript 中的 var 是动态类型的，类型是在运行时根据变量赋值来决定的。

> 函数定义

```JS
fcuntion testFunc(a, b) {
    return a * b;
}
```