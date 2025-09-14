> IO 多路复用相关内容，准备陆续的再整理一遍。

# 前言

> 什么是IO多路复用？

I/O 多路复用（I/O Multiplexing），指的是：
`让一个线程/进程同时监视多个 I/O 对象（文件描述符 fd），一旦某个 fd 就绪（可读/可写/异常）`，内核会通知我们。

> 常见的API有:

- select
- poll
- `epoll`（Linux 特有，高效）
- `kqueue`（BSD/macOS）

---

# Epoll

`epoll` 是 Linux 内核提供的 I/O 多路复用机制，可以让一个进程/线程同时监控很多 **文件描述符(fd)**，一旦某些 fd 就绪，内核会告诉你。

- 你只要调用一次 `epoll_wait()` 就能得到哪些 fd 可读/可写。
- 避免了开很多线程去阻塞式 `read()` 的情况。

在 Linux 中，**stdin、socket、pipe、timerfd 都是 fd**，所以都能放进 epoll。

## 示例

```c++

```

