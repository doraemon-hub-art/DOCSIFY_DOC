> GUN 内置的一些有关于C++的一些工具

---

# c++filt

交叉编译后，二进制程序起不来。

> 专门用来 还原 C++ 的符号混淆

例如这种错误: symbol lookup error: xx

```bash
c++filt xx
```

会把编译器生成的乱码符号还原成原始 C++ 函数签名 的工具，方便你在 nm、objdump、gdb 或报错信息里看懂函数名。

---
