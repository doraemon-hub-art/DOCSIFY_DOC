# memory

## /proc 目录

> 是什么？

/proc 是 Linux 的虚拟文件系统（procfs）

它不是真实文件，而是内核提供的进程和系统信息接口

每个进程都有一个目录 /proc/<pid>，里面存放该进程的各种信息：

```bash
/proc/<pid>/status → 进程状态、内存、线程数量、UID/GID 等
/proc/<pid>/cmdline → 进程启动命令
/proc/<pid>/maps → 虚拟内存映射
/proc/<pid>/smaps → 每块内存段详细统计
```

> 查看某一进程的内存占用？

```bash
cat /proc/<PID>/status | grep -E 'VmPeak|VmSize|VmRSS|VmData|VmStk|VmExe|VmLib|VmSwap'
```

| 字段      | 含义 |
|----------|------|
| VmPeak   | 进程历史最大虚拟内存（VSZ） |
| VmSize   | 当前虚拟内存总量 |
| VmRSS    | 实际占用物理内存（RAM） |
| VmData   | 数据段（堆/全局变量）大小 |
| VmStk    | 主线程栈大小 |
| VmExe    | 进程可执行文件大小 |
| VmLib    | 加载库占用内存 |
| VmSwap   | 已经使用的 swap 空间 |

> 一个定时看实际内存占用的小命令

```bash
while true; do
    grep VmRSS /proc/<PID>/status
    sleep 2
done
```

---

## pmap

> 是什么？

—— pmap = process memory map

- 用于查看 指定进程的内存结构和内存占用情况的轻量工具；
- 显示虚拟地址空间的各个段（堆、栈、库、mmap 文件、匿名页等）；
- Linux/Unix 系统常用工具，轻量、无需修改程序；

> 示例:
> 列出指定进程 <pid> 中，实际占用物理内存（RSS）最大的 10 个内存区域。

```bash
pmap -x <pid> | sort -k3 -n | tail -10
```