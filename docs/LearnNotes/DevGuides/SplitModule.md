> 将AWS KVS 示例Sample拆成C++Class，以融进项目。

> 这个就是最好的应用示例。

---

# 主题

- 复杂系统的拆分，靠的不是“按文件分”，而是“按职责 和 边界 分”。
- 拆分核心标准；
  - 职责是否单一；
  - 是否可以在别处重复使用；

> 关注点分离（Separation of Concerns）**的设计原则。

这样做的好处是代码可读性强、易于调试，且方便后期扩展（比如支持多路流）。

TODO: 完善这篇文章，看其中是否有值得分享的部分。

目前是将，原本的KVS master sample 示例

拆成了三个部分:

- kvs_signaling 作为连接信令服务器；
- kvs_peer_connect  建立 WebRTC 链路，发送音视频帧；
- kvs_controll 单例模式，协调信令和连接；

