> 一些常见的WebRTC 概念

> 流程

> WebRTC 简单入门 + AWS 实战解读
是什么——>干嘛的——>概念解读———>流程解析——> ICE&NTA 打洞——>开源Demo树莓派实战——>商业集成的服务AWS KVS——>个人经验分享

---

# 前言

在本文中，我们从WebRTC概念出发，并简单介绍一些相关名词，到最后的商业集成服务，做一个一站式的串联讲解，如果你准备要接触一下WebRTC，那么我想这会对你有一些帮助。

---

# Reference

- 

---

# 通讯流程

![](https://oss.banshengua.top/blogimages/202511261949664.png)

---

# 概念解读 
## WebRTC

> 是什么？

Web Real-Time Communication（网页即时通信）。

WebRTC 是一项让两个设备（浏览器、手机、摄像头）在不需要安装插件的情况下，建立点对点（P2P）连接，进行毫秒级低延迟音视频通话或数据传输的技术。

> 核心

**核心在于，尽量在传输数据时，P2P直连。**
---

> 能干什么？



## 信令服务器

> 干什么的？

- 用于 WebRTC 交换信息，建立连接；

> 交换什么信息？

- SDP（Offer/Answer）：定义音视频编解码格式、媒体轨道、方向等；
- ICE 候选节点： 公网 IP / NAT 信息，用于寻找最佳 P2P 路径；

> 实现

- 传统实现: WebSocket HTTP MQTT 等；
- 云服务: AWS KVS Signaling Channel 等；
- 直接点对点: 理论上如果能直接把信令喜信息传给对方，就可以不依赖服务器建立连接；

---

## ICE

> 是什么？

ICE（Interactive Connectivity Establishment）是 WebRTC 中 P2P 连接的核心机制。

负责在复杂的网络环境下(NTA、防火墙)，找到两端可以互相通信的路径。

找到最优直连路线的过工具。

> 作用

- 收集候选点
  - 每端都会收集自己的所有可能连接点;
    - Host candidate：本地 IP 地址（局域网直连）；
    - Server reflexive candidate：通过 STUN 获取的公网 IP；
    - Relayed candidate：通过 TURN 中继服务器的 IP；
- 相互交换候选点
  - 两端通过信令服务器把候选点交换给对方；
- 连接检查
  - 尝试每一对候选点，看哪一路可以走通；
    - 找到后，建立实际的音视频传输通道；

最后封装成SDP包，交由信令服务器，发送给另一端。

> ICE STUN TURN 的关系

| 名称 | 作用 |
| ---  | --- |
| ICE	| 整个“找路”流程，收集候选点、交换、验证 |
| STUN	| 获取公网 IP，尝试直接 P2P 连接 |
| TURN	| NAT / 防火墙阻挡时的中继服务器，保证通信 |


---

## STUN

> 是什么？

STUN（Session Traversal Utilities for NAT）是 WebRTC 和其他实时通信中用来帮助设备获取自己的公网 IP 和端口信息的协议。

> 做什么？

它的核心目的是实现 NAT 穿透，让两端可以尽量直接 P2P 通信。

---

## TURN

> 是什么？

TURN（Traversal Using Relays around NAT）是 WebRTC 和其他实时通信中用来在 NAT 或防火墙阻挡下保证音视频流可以传输的中继服务。

它是 STUN 的“备用方案”，当直接 P2P 连接失败时，音视频流就会经过 TURN 服务器中转。

---

## SDP 

> 是什么？

在建立连接前，用来描述“我有能力接收什么样的音视频数据”的文本格式。

- 媒体类型：音频（audio）、视频（video）、数据通道（data）；
- 编解码器：Opus、VP8、H264 等；
- 加密信息：DTLS 指纹（用于加密通信）；
- 流标识：哪路流是哪个轨道（track）；

> 在有些模式中，ICE 包不会放到 SDP 中，而是与 SDP 并行发送；

- 网络信息：IP、端口、候选的 ICE 路径

---

# DEMO实战

> 安装boost

```bash
wget https://downloads.sourceforge.net/project/boost/boost/1.83.0/boost_1_83_0.tar.gz
tar -xzf boost_1_83_0.tar.gz
cd boost_1_83_0
./bootstrap.sh --prefix=/usr/local
sudo ./b2 install -j14
```

---

# AWS KVS


---