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

##  JS 版本查看

可以根据Node.js的版本和浏览器支持的ECMAScrip标准来查看JS版本

---

## 百分号编码 

百分号编码（Percent-encoding）就是把 URL 里不能明文出现的字符替换成 %HH 形式（HH 是十六进制 ASCII 码），这样：

- 浏览器/服务器不会把特殊符号当成分隔符；
- 任意 Unicode 字符都能安全地在 URL 中传输。

---

## OAuth 2.0 

OAuth 2.0 是一个**授权框架**，用于在不暴露用户凭证（如用户名和密码）的情况下，允许第三方应用访问用户存储在其他服务上的资源。它是一种授权机制，而不是认证机制。

在 OAuth 2.0 中，授权通常通过获取一个 访问令牌（access token） 来实现，应用可以使用这个令牌代表用户向服务发出请求。

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

 > 解析${}占位符

- 单引号 / 双引号 → 纯字符串；
- 反引号 → 模板字符串，会替换 ${变量}。

```js
const responUrl = `${domain}/auth`;
```

---

# 内容补充

> .mjs 和 .js 的区别 ？

> 本地编译检查语法错误 ？ 

```bash
node xxx.mjs
```

---

## request 和 axios

都是用于发送 HTTP 请求的 Java式库。

> axios

目前 axios 是一个活跃的项目，得到社区的广泛支持和维护。它是一个基于 Promise 的 HTTP 客户端，提供了很多现代化的功能。

基于 Promise 的，这意味着它原生支持 async/await 语法，使得处理异步请求时更加简洁和易读。Promise 使得链式调用变得容易，且与现代 JavaScript 语法更加兼容。

```js
// 使用 axios 的 async/await 示例
const response = await axios.get('https://api.example.com');
```

> request

request 库曾经非常流行，但官方已经宣布从 2020 年 2 月开始不再维护，并建议用户迁移到其他库（如 axios）。

基于 回调函数（callback），虽然它也支持 Promise 通过 request-promise 库，但原生是基于回调的。这在现代 JavaScript 开发中显得有些过时，且回调函数容易导致 回调地狱。

```js
// 使用 request 的回调函数示例
request.get('https://api.example.com', (err, response, body) => {
  if (err) { console.error(err); return; }
  console.log(body);
});
```

 ---

 ## fast-fuzzy

 模糊匹配
 
 官方文档 —— https://www.npmjs.com/package/fast-fuzzy

 > demo

 ```js
import { fuzzy } from "fast-fuzzy";

const similarityScore = fuzzy("front yard", "Bacj Yard");

console.log(similarityScore);
 ```

---

## package.json

package.json 是 Node.js 项目的配置文件，用于定义项目的元数据、依赖项、脚本、版本信息等。

它是 npm（Node.js 包管理工具）和其他构建工具的重要部分。

当执行完npm install后，会生成package-lock.json来记录依赖库版本信息，当你把整个工程推送到git，其他同事再执行npm install后，可以确保完全相同的以来环境。

如果package.json发生了变化或者package.json和package-lock.json不一致，都会优先使用package.json中的信息。

```json
{
  "name": "my-awesome-project",  // 项目名称，npm 包的名称，必须是唯一的
  "version": "1.0.0",  // 项目版本，遵循语义版本控制（semver）
  "description": "A project to manage tasks.",  // 简短的项目描述
  "main": "src/index.js",  // 项目入口文件，通常是应用的启动文件
  "scripts": {  // 定义可通过 npm run 执行的脚本
    "start": "node app.js",  // 启动项目的脚本
    "test": "mocha"  // 测试脚本
  },
  "dependencies": {  // 生产环境依赖，项目运行时需要的库
    "express": "^4.17.1",  // Web 框架
    "axios": "^0.21.1"  // HTTP 请求库
  },
  "devDependencies": {  // 开发环境依赖，仅用于开发和构建
    "webpack": "^5.38.1",  // 打包工具
    "jest": "^27.0.6"  // 测试框架
  },
  "peerDependencies": {  // 对其他包的兼容版本要求，通常用于插件或库
    "react": "^16.0.0"  // 项目需要与特定版本的 React 配合使用
  },
  "optionalDependencies": {  // 可选依赖，不会阻止安装失败
    "fsevents": "^2.3.2"  // 用于 MacOS 的文件系统事件监控
  },
  "engines": {  // 指定支持的 Node.js 和 npm 版本
    "node": ">=14.0.0",  // 支持的 Node.js 最低版本
    "npm": "^6.0.0"  // 支持的 npm 最低版本
  },
  "repository": {  // 项目的代码仓库地址
    "type": "git",
    "url": "https://github.com/username/project.git"  // Git 仓库 URL
  },
  "keywords": [  // 关键字，有助于在 npm 上搜索
    "node", "express", "api"
  ],
  "author": "John Doe",  // 项目的作者
  "license": "MIT",  // 项目的开源许可证
  "bugs": {  // 项目的 Bug 跟踪系统
    "url": "https://github.com/username/project/issues"  // Bug 提交页面
  },
  "homepage": "https://github.com/username/project",  // 项目的主页 URL
  "config": {  // 自定义配置项，npm 或其他工具可以使用
    "port": "8080"  // 配置项示例，项目所需端口
  },
  "private": true,  // 如果为 true，禁止将项目发布到 npm registry
  "publishConfig": {  // 配置项目发布时的 registry URL
    "registry": "https://npm.example.com"  // 指定发布到的 npm registry
  },
  "dependenciesMeta": {  // 关于依赖项的额外元数据
    "express": {
      "optional": true  // 指定某个依赖项为可选
    }
  }
}
```

---

## 格式化

```bash
npm i -g prettier
# package.json中配置
  "scripts": {
    "fmt": "prettier --write lambda/**/*.mjs"
  },
# 运行
npm run fmt
```

---



