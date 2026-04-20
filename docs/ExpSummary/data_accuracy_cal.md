> 一些精度计算问题

# 

AWS KVS 中，调用延迟的单位是 100ns，为什么？

详见源码 —— [amazon-kinesis-video-streams-webrtc-sdk-c/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h](https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c/blob/ef4649473b2d1dc25215eca9d21cda6c802c06f2/src/include/com/amazonaws/kinesis/video/webrtcclient/Stats.h#L605)

```C++
UINT64 cpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the control plane APIs
UINT64 dpApiCallLatency;         //!< Latency (in 100 ns) incurred per backend API call for the data plane APIs
```

---