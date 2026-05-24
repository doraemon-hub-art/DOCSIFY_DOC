---
marp: true
---

<style>
section {
  align-content: start;
}
</style>

> 一个精度计算问题

AWS KVS WebRTC C SDK 中，调用延迟的单位是 100ns（纳秒），为什么？

详见源码 —— [amazon-kinesis-video-streams-webrtc-sdk-c/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/ef4649473b2d1dc25215eca9d21cda6c802c06f2/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h#L605)

```C++
UINT64 cpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the control plane APIs
UINT64 dpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the data plane APIs
```

---

> 可能的原因

- 与 Windows FILETIME 对齐，即跨平台设计；

详见 —— [FILETIME structure](https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime)

包含一个64位值，表示自1601年1月1日（UTC）以来的100纳秒间隔数。

```C++
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
```

---

> 问题: 精度损失 

用 100ns 为单位，使用UINT64为类型，意味着你丢失了 100ns 以下的精度。

UINT64，为无符号整数，无法表示小数。

例如: 无法表示 10ns。

---

> 粒度和精度

粒度: 最小能区分的刻度。

精度: 测量值离真实值有多近。

> 时间单位选择的取舍

粒度越大 → 能表示的范围越大 → 但"精度窗口"也越大 → 丢弃的细节越多。

| 选择 | 单位 | 代价 |
| --- | --- | --- |
| 高精度（测短时间） |	小单位（ns）|	范围受限 |
| 大范围（覆盖长时间） |	大单位（ms、s）	| 精度降低 |


---

> 小结

一个小单位引起的思考，在本案例中，最核心内容，并不是单位SDK跨平台设计本身，而是通过这个具体案例，我们可以看到其选择后背的通用设计原则和取舍思维。

---




补充


> 区分： 每个单位代表100ns和以100为单位

例如: 在单位100ns下，值为1，对应的就是100ns，而不是0.01ns；