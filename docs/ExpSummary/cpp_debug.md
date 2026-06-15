> 一些调试方法，看具体情况是否整合到其他为止

> 看进程加载的具体SO路径

```bash
/proc/<pid>/maps
```

> 加载特定路径的so

```bash
LD_PRELOAD=xx.so ./test_bin
```

---

# addr2line

> 背景

没有 core dump 文件，但幸运的是，日志中已经打印出了崩溃时的相对偏移量，这些相对偏移量不受内存地址随机化（ASLR）的影响，可以直接用于定位代码。

```bash
aarch64-linux-gnu-addr2line -e ./xx -f -C <address>
```

要对应二进制的内部偏移量。

```bash
# 即这个 0x16dw54
stack dump [6]  /path/to/your_bin+0x16dw54 [0x52318be54]
```

> 前置条件

- 编译优化一致；
- 工具链版本一致；
- 源码一致；

> 补充

- striped 则无法解析；
  - file ./file 查看；

```cmake
# 注意strip了，无法解析 去掉-s 添加-g,或者显式CMKAE指定
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror=return-type -O2 -s")
```


---

# Traffic Control

在 Linux 环境下，最常用的网络模拟工具是内核自带的 tc (Traffic Control) 工具，配合 netem (Network Emulator) 模块。它可以模拟延迟、丢包、抖动、重复包及包乱序等多种网络状况。


TODO: 补充

> 延迟验证

```bash
ping -c 5 <目标IP>
```