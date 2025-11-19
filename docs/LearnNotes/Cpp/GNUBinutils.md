> GNUBinutils ——  内置的一些有关于C++的一些工具

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

# addr2line

主要用来把程序崩溃或者打印出来的 地址 转换成 源代码位置（文件名 + 行号）。

前提是你的程序带了调试信息（通常编译时加 -g）。

```bash
# Segmentation fault at 0x4005d4

addr2line -e test 0x4005d4
```

---


