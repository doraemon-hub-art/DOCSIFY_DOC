> 查看是否有符号表

```bash
sh-4.4# readelf -S /usr/bin/TestServer | grep debug
  [29] .debug_aranges    PROGBITS         0000000000000000  00303480
  [30] .debug_info       PROGBITS         0000000000000000  0031a090
  [31] .debug_abbrev     PROGBITS         0000000000000000  01dbc609
  [32] .debug_line       PROGBITS         0000000000000000  01e0f9b6
  [33] .debug_str        PROGBITS         0000000000000000  01f97c7b
  [34] .debug_loc        PROGBITS         0000000000000000  027fc876
  [35] .debug_ranges     PROGBITS         0000000000000000  03741000
```

---

> 死锁相关解决思路

```bash
# 查看所有线程信息
info threads
# 多个线程都处于 futex_wait 状态，意思发生相互等待 —— 死锁
```

```bash
* 1    Thread 0x7ffff7e8a740 (LWP 1947447) "deadlock_exampl" 0x00007ffff7898d71 in __futex_abstimed_wait_common64 (private=128, cancel=true, abstime=0x0, op=265, 
    expected=1947448, futex_word=0x7ffff7600990) at ./nptl/futex-internal.c:57
  2    Thread 0x7ffff76006c0 (LWP 1947448) "deadlock_exampl" futex_wait (private=0, expected=2, futex_word=0x5555555591a0 <mtx2>) at ../sysdeps/nptl/futex-internal.h:146
  3    Thread 0x7ffff6c006c0 (LWP 1947449) "deadlock_exampl" futex_wait (private=0, expected=2, futex_word=0x555555559160 <mtx1>) at ../sysdeps/nptl/futex-internal.h:146
```

- 切换到可疑线程

```
thread 2
```

- 查看调用栈

```bash
bt
```

- C++ test demo —— 两个线程相互等待，一个锁A请求B，一个锁B请求A。

```C++
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void threadA() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread A trying to lock mtx2\n";
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "Thread A acquired both locks\n";
}

void threadB() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread B trying to lock mtx1\n";
    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "Thread B acquired both locks\n";
}

int main() {
    std::thread t1(threadA);
    std::thread t2(threadB);

    t1.join();
    t2.join();

    return 0;
}
```

---

> 调试coredump问题相关命令

- 打印某个变量

```bash
print xx
```

- 打印相关变量

```bash
info locals
```

- 打开coredump

```bash
gdb your_program core
```

- 是否允许生成core文件

```bash
ulimit -c unlimited        # 临时允许生成 core 文件
ulimit -c                  # 查看当前 core 文件大小限制（unlimited 表示无限制）
```

- linux系统有关coredump的配置

```bash
cat /proc/sys/kernel/core_pattern
```

内核会根据这个规则决定 **core 文件的名字**、**保存位置**，甚至 **是否交给外部程序处理**。

- 现代linux发行版中会将coredump交给systemd管理

```bash
coredumpctl list #
......
```

---

> coredump常见错误原因

- SIGSEGV (Segmentation fault)

最常见：访问了非法内存地址（野指针、越界访问、解引用空指针）。

栈上显示一般是 Segmentation fault (core dumped)。

- SIGABRT (Abort)

程序调用 abort()，通常是 断言失败（assert）、内存检查库发现错误时触发。

报错：Aborted (core dumped)。

---

> o2 o3 优化导致调用栈不完整原因

在 `-O2` 及以上，编译器默认启用 `-fomit-frame-pointer`。

传统上，每个函数会在栈上保存上一个函数的栈基址（frame pointer），这样才能顺着链表往上回溯。

一旦省略，编译器会用寄存器或栈偏移量直接访问局部变量，没有统一的回溯链 → **backtrace 可能断掉或错位**。

---

> 没有崩溃栈定位coredump思路

1. 查看是否常见错误，例如 SIGABRT断言、SIGSEGV非法访问，结合代码查看；
2. 关键位置添加日志打印；
3. 如果在自测调试阶段，保留符号表和调试信息，gdb运行，打印调用栈等；

 ---

 # 交叉编译源码在aarch64系统上运行

 