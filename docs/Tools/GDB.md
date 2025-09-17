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



