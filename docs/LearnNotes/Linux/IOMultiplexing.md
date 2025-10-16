> IO 多路复用相关内容，准备陆续的再整理一遍。

# 前言

> 补充&参考文章

- [【Linux】I/O多路复用-SELECT/POLL/EPOLL_多路复用-半生瓜のblog](https://banshengua.blog.csdn.net/article/details/127041022)
- [(IO多路复用——深入浅出理解select、poll、epoll的实现 - 知乎](https://zhuanlan.zhihu.com/p/367591714)

> 什么是IO多路复用？

I/O 多路复用（I/O Multiplexing），指的是：
`让一个线程/进程同时监视多个 I/O 对象（文件描述符 fd），一旦某个 fd 就绪（可读/可写/异常）`，内核会通知我们。

> 有什么好处？

- 性能

事件驱动模式：只有在 fd 真正“就绪”时才处理。

不会像轮询(不同read/write看是否有数据)那样空转浪费 CPU。

- 统一事件源

不止网络：stdin、管道、文件、定时器、信号都能放进 epoll/kqueue。

> 常见的API有:

- select
- poll
- `epoll`（Linux 特有，高效）
- `kqueue`（BSD/macOS）

> 经典模型  **I/O 多路复用 + 事件驱动**，其实就是现在高并发服务器常用的经典架构

让一个线程用较少的资源处理海量连接。缺点是代码复杂，需要自己管理事件和状态。

---

# Select&Poll

- select 有上限通常是1024，poll无固定限制，受系统最大fd限制；
- 每次调用都需要拷贝整个集合(数组)；
- 水平触发；
- O(n) 遍历所有fd；

`select与poll每次调用后，都需要遍历整个用户关心的事件集合，无论其中的事件是否就绪，所以应用程序检索就绪文件描述符的时间复杂度为O(n)。`

---

# ET 和 LT

> 水平触发

- fd上只要有数据没读完，epoll_wait每次都会返回这个事件；

> 边缘触发

- 只在状态变化后通知，一次数据没读完，就不会再通知了；

---

# Epoll

`epoll` 是 Linux 内核提供的 I/O 多路复用机制，可以让一个进程/线程同时监控很多 **文件描述符(fd)**，一旦某些 fd 就绪，内核会告诉你。

- 你只要调用一次 `epoll_wait()` 就能得到哪些 fd 可读/可写。
- 避免了开很多线程去阻塞式 `read()` 的情况。

在 Linux 中，**stdin、socket、pipe、timerfd 都是 fd**，所以都能放进 epoll。

## 示例

- 主要调用	

```c++
epoll_create1 - 创建Epoll实例
epoll_ctl - EPOLL_CTL_ADD 添加关心对象&对应事件
epoll_wait - 等待事件 哪些 fd 发生了什么事件 收集到数组里
```

```c++
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <cstring>

int main() {
    // 1. 创建 epoll 实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return 1;
    }

    // 2. 添加 stdin (fd=0)
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl: stdin");
        return 1;
    }

    // 3. 创建 timerfd (每隔3秒触发)
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd == -1) {
        perror("timerfd_create");
        return 1;
    }

    itimerspec timerSpec{};
    timerSpec.it_interval.tv_sec = 3;  // 周期 3 秒
    timerSpec.it_value.tv_sec = 3;     // 首次 3 秒后触发
    if (timerfd_settime(tfd, 0, &timerSpec, nullptr) == -1) {
        perror("timerfd_settime");
        return 1;
    }

    ev.events = EPOLLIN; // 关心可读事件
    ev.data.fd = tfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, tfd, &ev) == -1) {
        perror("epoll_ctl: timerfd");
        return 1;
    }

    std::cout << "Epoll demo started. Type something or wait for timer..." << std::endl;

    // 4. epoll 循环
    while (true) {
        // 10 不是固定值，指定每次取出来的就绪事件个数
        epoll_event events[10];
        int nfds = epoll_wait(epfd, events, 10, -1); // 阻塞等待
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buf[256];
                int n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if (n > 0) {
                    buf[n] = '\0';
                    std::cout << "[stdin] You typed: " << buf;
                }
            } else if (events[i].data.fd == tfd) { // timerfd就绪
                uint64_t expirations;
                read(tfd, &expirations, sizeof(expirations)); // 必须读掉
                std::cout << "[timer] Timer ticked! (" << expirations << " times)" << std::endl;
            }
        }
    }

    close(tfd);
    close(epfd);
    return 0;
}

```

