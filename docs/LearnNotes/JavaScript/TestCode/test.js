// 随便打印一些东西,写到控制台
console.log("Hello, World!");
console.log(1 + 3.14);
console.log(16 + "Volvo") // 16Volvo



// 创建一个简单的 HTTP 服务器
// const http = require('http');

// const server = http.createServer((req, res) => {
//     res.writeHead(200, { 'Content-Type': 'text/plain' });
//     res.end('Hello, Node.js!');
// });

// server.listen(3000, () => {
//     console.log('Server running at http://localhost:3000/');
// });

var testStr = 'Test';
var testStrDouble = "Test";

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