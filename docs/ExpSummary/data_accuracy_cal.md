> 一个精度计算问题

# 

AWS KVS 中，调用延迟的单位是 100ns，为什么？

详见源码 —— [amazon-kinesis-video-streams-webrtc-sdk-c/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/ef4649473b2d1dc25215eca9d21cda6c802c06f2/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h#L605)

```C++
UINT64 cpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the control plane APIs
UINT64 dpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the data plane APIs
```

> 可能的原因

- 与 Windows FILETIME 对齐；

大多数 SDK 需要跨平台支持，这个也不例外。

详见 —— [FILETIME structure](https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime)

包含一个64位值，表示自1601年1月1日（UTC）以来的100纳秒间隔数。

```C++
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
```

---

> 区分： 每个单位代表100ns和以100为单位

例如值为1，对应的就是100ns，而不是0.01ns；

> 精度损失 

用 100ns 为单位，意味着你丢失了 100ns 以下的精度。

UINT64为无符号整数，无法表示小数。

例如: 无法表示 10ns。



---