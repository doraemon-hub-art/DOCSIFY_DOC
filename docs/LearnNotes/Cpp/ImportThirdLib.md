> 引入第三方库

---

# 编译第三方库，应用到项目中

> 以 AWS KVS WEB RTC C SDK 为例

- https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c

```bash
# cmake打开静态库编译
cmake -DBUILD_STATIC_LIBS=TRUE ..
make -j14
```
> 输出静态库

- libkvsWebrtcClient.a
  - 库负责 WebRTC 核心（ICE、DTLS、SRTP、RTP、PeerConnection 等）

- libkvsWebrtcSignalingClient.a
  - 库负责连接 AWS 的 Signaling Server，拿 ICE 配置

> 如何查看lib的平台、架构

```bash
file xx.a
```



