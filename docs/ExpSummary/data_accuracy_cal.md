---
marp: true
---

<style>
section {
  align-content: start; 
}
</style>

# 一个精度计算问题

AWS KVS WebRTC C SDK 中，调用延迟的单位是 100ns（纳秒），为什么？

详见源码 —— [amazon-kinesis-video-streams-webrtc-sdk-c/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/ef4649473b2d1dc25215eca9d21cda6c802c06f2/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h#L605)

```C++
UINT64 cpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the control plane APIs
UINT64 dpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the data plane APIs
```

---

# 可能的原因

> 跨平台设计

- 与 Windows FILETIME 对齐，即SDK跨平台兼容设计/历史原因；
  - 避免单位转换，以减少精度丢失；

详见 —— [FILETIME structure](https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime)

“Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC)."

包含一个64位值，表示自1601年1月1日（UTC）以来的100ns间隔数。

```C++
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
```
 阿达阿瓦的
---

- 那 Linux(Unx)中呢？

在 Unix 系列系统（Linux, macOS, BSD 等）中，时间单位的设计与 Windows 有所不同，它主要经历了从微秒（μs）到纳秒（ns）的演进。

1. 传统标准：微秒级 (Microseconds, 1,000ns)

```C++
struct timeval {
    time_t      tv_sec;     // 秒
    suseconds_t tv_usec;    // 微秒
};
```

2. 现代标准：纳秒级 (Nanoseconds, 1ns)

```C++
struct timespec {
    time_t  tv_sec;  // 秒
    long    tv_nsec; // 纳秒
};
```

---

- 总结: 

| 特性 | `struct timeval` (Unix) | `struct timespec` (Unix) | `FILETIME` (Windows) |
| :--- | :--- | :--- | :--- |
| **最小单位(粒度)** | 1 微秒 ($10^{-6}$s) | 1 纳秒 ($10^{-9}$s) | **100 纳秒** ($10^{-7}$s) |
| **数值“1”代表** | 1 微秒 | 1 纳秒 | 100 纳秒 |
| **起始时间** | 1970-01-01 | 1970-01-01 | 1601-01-01 |
| **数据布局** | 秒 + 微秒 (两个成员) | 秒 + 纳秒 (两个成员) | 64位整数 (拆分为高低位) |

---

## 选择的取舍

- 时间单位选择的取舍;

粒度越大 → 能表示的范围越大 → 丢失的细节越多。

| 选择 | 单位 | 代价 |
| --- | --- | --- |
| 高精度 |	小单位（ns）|	范围受限 |
| 大范围 |	大单位（ms、s）	| 精度降低 |

同理，在粒度这块，我们选择1还是100，跟选择ns还是ms/s是一个道理，都是小、大单位的区别。

---

> 问题: 精度损失 

用 100ns 为单位，使用无符号整型UINT64为类型，意味着你丢失了 100ns 以下的精度。

因为他，无法表示小数。

例如: 无法表示 10ns。

---

> 粒度和精度

粒度: 最小能区分的刻度。

精度: 测量值离真实值有多近。

> 区分

每个单位代表100ns和以100为单位。

例如: 在单位100ns下，值为1，对应的就是100ns，而不是0.01ns；

---

> 小结

一个小单位引起的思考，在本案例中，最核心内容，并不是单位SDK跨平台设计本身，而是针对单位的选择，我们可以看到其选择背后的通用设计原则和取舍思维。
