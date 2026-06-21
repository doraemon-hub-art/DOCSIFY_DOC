> 快速上手Lua，简单学习

# Reference

- [菜鸟教程](https://www.runoob.com/lua/lua-environment.html)

---

# Hello World

> 运行

```lua
lua xx.lua
```

---

> 交互式编程

```bash
lua 
lua -i
```

---

> 数据类型

- 动态类型语言，变量在声明时不需要类型定义，直接赋值即可；

---

> 变量

默认值全部为nil。

- 全局变量: 不加local修饰，在函数内也是全局变量，同shell；
- 局部变量: local修饰；
- 表中的域： ？

---

> 赋值语句

- Lua中的赋值语句，会先计算右边所以的值，然后在进行赋值操作；

```lua
x,y = y, x -- 交换
```

- 当多变量赋值时：
  - 值个数不足会补nil；
  - 变量数不足，多余的值会被忽略；

---

> 循环

- while;
- for;
- repeat...until;

```lua
while(true)
do
    print("xx")
end
```

- 循环控制:
  - break；
  - goto;

---

> 流程控制

- Lua中，0 是 true;

```lua
if(0)
then
    print("xx")
end
```

---

> 函数

- 可返回多个值；
- ... 为可变参数；
  - 可以通过 select("#",...) 来获取可变参数的数量；
  - 固定参数必须放在可变参数之前；

```lua
function xx()
    print("xx")
    return 1,2
end
```

```lua
function average(...)
   result = 0
   local arg={...}    --> arg 为一个表，局部变量
   for i,v in ipairs(arg) do
      result = result + v
   end
   print("总共传入 " .. #arg .. " 个数")
   return result/#arg
end

print("平均值为",average(10,5,3,4,5,6))
```

---

> 运算符 

- 算数运算符 同 C/C++；
- 关系运算符，~= 为不相等，其他同C/C++；
- 逻辑运算符；
  - and 逻辑与；
  - or 逻辑或;
  - not 逻辑非;

---

> 字符串

- Lua有一些方便的内置方法；

---

> 数组

- {} 包裹，用索引访问；
  - 索引从1开始；
  - 超过索引范围返回nil；
    - “动态”数组，不用限制数组大小；
- 用# 计算长度；

```lua
-- 创建一个数组
local myArray = {10, 20, 30, 40, 50}

-- 循环遍历数组
for i = 1, #myArray do
    print(myArray[i])
end
```

---

> 迭代器

迭代器（iterator）是一种对象，它能够用来遍历标准模板库容器中的部分或全部元素，每个迭代器对象代表容器中的确定的地址。

在 Lua 中迭代器是一种支持指针类型的结构，它可以遍历集合的每一个元素。

- 泛型for迭代器；
  - for v1, v2 in pairs(t) do

......

---

> table

- 可以当数组用，也可以当哈希表用；
- 两个table 赋值操作为 引用赋值；

---

> 模块与包

- 创建模块;

```lua
-- 文件名为 module.lua
-- 定义一个名为 module 的模块
module = {}
 
-- 定义一个常量
module.constant = "这是一个常量"
 
-- 定义一个函数
function module.func1()
    io.write("这是一个公有函数！\n")
end
 
local function func2()
    print("这是一个私有函数！")
end
 
function module.func3()
    func2()
end
 
return module
```

- 在其他地方引入；

```lua
require("module")
 
print(module.constant)
 
module.func3()
```

- C包，Lua和C很容易结合工作；

......

> 元表

给table自定义一些方法。

---

> Coroutines 协程

其实就类似于在普通函数中，通过全局变量来控制每次的执行，只不过那样的话，需要很多else if。

示例: 在游戏开发中，某个 NPC 的行为控制。

......

```lua
-- 1. 创建一个协程
local co = coroutine.create(function()
    for i = 1, 2 do
        print("协程运行中", i)
        -- 2. 暂停执行，并交回控制权
        coroutine.yield() 
    end
    print("协程结束")
end)

-- 3. 启动/恢复执行
coroutine.resume(co) -- 输出：协程运行中 1
print("回到主线程")
coroutine.resume(co) -- 输出：协程运行中 2
coroutine.resume(co) -- 输出：协程结束
```

---

> 垃圾回收

Lua采用自动内存管理，运行了了一个垃圾收集器来收集所有的死对象。

---

> 面向对象

Lua 语言中并没有原生（Native）的 class 关键字。Lua 的面向对象（OOP）是基于 原型（Prototype） 的模式实现的，这与 JavaScript 类似。

```lua
-- 1. 定义一个表作为“类”的原型
Shape = { area = 0 }

-- 2. 定义“构造函数”
function Shape:new(o)
    o = o or {}       -- 如果用户没传表，就新建一个
    setmetatable(o, self) -- 将新表的元表设为 Shape
    self.__index = self   -- 关键：找不到属性时，回 Shape 找
    return o
end

-- 3. 定义“成员方法”
function Shape:printArea()
    print("面积为: ", self.area)
end

-- 4. 实例化对象
local myShape = Shape:new({area = 100})
myShape:printArea() -- 输出 100
```

---

# 应用场景

- 游戏开发脚本；
- 软件配置与扩展;

......
