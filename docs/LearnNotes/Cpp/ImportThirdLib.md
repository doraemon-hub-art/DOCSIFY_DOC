> 引入第三方库

---

# 编译第三方库，应用到项目中

> 以 AWS KVS WEB RTC C SDK 为例

- https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c

```bash
# cmake打开静态库编译 设置make install输出目录
cmake -DBUILD_STATIC_LIBS=TRUE -DCMAKE_INSTALL_PREFIX=../../install ..
make -j14
make install
```
> 输出静态库

- libkvsWebrtcClient.a
  - 库负责 WebRTC 核心（ICE、DTLS、SRTP、RTP、PeerConnection 等）

- libkvsWebrtcSignalingClient.a
  - 库负责连接 AWS 的 Signaling Server，拿 ICE 配置

> 如何查看lib的平台、架构

```bash
ar -t libfoo.a
ar -x libfoo.a some_object.o
file some_object.o
```

为什么检查静态库要拆到.o这一集？

- .a 不是为某个平台编译的程序，而是很多“半成品”的打包袋；
  - 真正携带有用的信息的，在袋子中的每个零件中；

```bash
file xx.a
```



