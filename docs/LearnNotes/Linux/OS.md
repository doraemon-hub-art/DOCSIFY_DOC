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

# 概念

## eMMC

eMMC = embedded Multi-Media Card（嵌入式多媒体卡）

它把三样东西封装在一起：
1. NAND Flash 颗粒（真正的存储介质，类似 U 盘里的芯片）
2. 控制器芯片（负责读写管理、坏块处理、擦写均衡）
3. 接口协议（MMC 协议，通过几个引脚跟 CPU 通信）

eMMC 就是一个"焊在板子上的硬盘"，CPU 通过读写它的不同地址来访问不同分区。

---

## 内核运行和分区挂载

> 挂载 

把存储区域关联到目录树，让用户/应用能读写文件。

> 内核运行（不需要挂载）

上电 → CPU 执行固化在芯片内的 bootrom 代码
    → bootrom 从 eMMC boot 分区（固定物理地址）
        把内核镜像读到内存 → CPU 跳转到内存执行
    → 内核开始运行

---

## Linux发行版和Linux内核的区别和关系

 > Linux 内核

- 是一个单独的软件项目，只提供操作系统最核心的功能;
  - 进程管理、内存管理、文件系统、网络、设备驱动;
- 它本身不是一个完整的操作系统，不能直接"使用";

> Linux 发行版

- 是内核 + 大量其他软件打包在一起组成的完整操作系统：
  - 发行版 = 内核 + GNU 工具 + 包管理器 + 应用软件 + 配置管理

> 关系

- 发行版包含内核;
- 同一个内核可以被不同发行版使用;
- 换发行版可以不换内核，换内核可以不换发行版;

---

## Buildroot & Yocto

✦ ## 一句话

│ Buildroot 和 Yocto 都是"嵌入式 Linux 定制工厂"，你告诉它你要什么，它帮你编译出一整套完整系统。

---

## 为什么需要它们

标准 Linux 发行版（Ubuntu、Debian）面向通用场景，体积大、组件多。但嵌入式设备需要的是：

- 体积小（几 MB 到几百 MB，而不是几个 GB）；
- 功能精准（不需要的功能一个都不要）；
- 特定硬件（Soc驱动、编解码库等；）
- 定制化（你自己的应用、UI、启动流程）；

要手动去交叉编译内核、编译 busybox、编译 libc、编译所有的应用和依赖库……太痛苦了。

Buildroot 和 Yocto 就是来解决这个问题的。

---





