> 系统相关内容

# memory

## 系统未自动回收未使用内存

> malloc_trim

- 主动给 glibc 的 ptmalloc 分配器一个“提示”，让它尝试把进程堆顶未使用的空闲内存，主动归还给操作系统。

malloc_trim(0) 会检查堆的 最高地址附近 是否存在足够大的连续空闲块。

如果有，就调用 sbrk() 把堆收缩，归还给内核；如果是 mmap 出来的大块，也会触发 munmap()。

但注意：不是所有 free() 的内存都能被 trim 掉，只有堆顶那部分才行。

```C++
    // call malloc_trim to release unuse memory
    if (malloc_trim(0) == 1) {
        LOG_INFO(logger, "Memory clear.Unused memory returned to the system.");
    } else {
        LOG_INFO(logger, "Memory clear.No memory was released.");
    }
```

> 补充，这并不是Linux4.9或者较老版本的BUG

Linux 内核从来没有负责“自动回收未用堆内存”，它只负责响应 brk/munmap。是不是归还，完全取决于用户态 allocator（glibc、jemalloc、tcmalloc）。

---

# glibc

> 是什么

glibc 是 GNU C Library，全称 GNU C 标准库。它是 `Linux 用户态里最核心的一块基础库`。

它提供了 `系统调用封装`(write -> printf)、标准 C API、运行时支持（malloc/线程/动态加载器）。

---





