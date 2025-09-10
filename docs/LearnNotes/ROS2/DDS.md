> 记录DDS相关内容

# 参考资料

- [FastDDS](https://fast-dds.docs.eprosima.com/en/stable/)



---

# 概念补充

## 什么是DDS?

**DDS（Data Distribution Service，数据分发服务）**
 是 **OMG（Object Management Group）** 制定的一种 **实时数据通信中间件标准**，其核心目标是为分布式系统提供 **高性能、低延迟、可扩展、可靠** 的数据交换机制。

主要特点：

1. **发布-订阅模式（Pub-Sub）**
   - 数据的生产者（Publisher）和消费者（Subscriber）之间 **解耦**，无需直接连接。
   - 支持“按主题（Topic）”和“按内容（Content-Filter）”订阅。
2. **实时性与可靠性**
   - 提供不同 **QoS（服务质量）策略**，例如：
     - **可靠传输（Reliable）** vs **尽力而为（Best-Effort）**
     - **历史数据（History）** 策略
     - **持久性（Durability）** 策略
   - 可以根据应用场景（汽车、工业、金融、国防）调整通信可靠性和延迟。
3. **互操作性**
   - DDS 是一个 **标准协议**，不同厂商实现（RTI Connext DDS、eProsima Fast DDS、OpenDDS、Cyclone DDS 等）之间可以互通。
4. **高性能与可扩展性**
   - 设计上更偏底层，能绕过 TCP/IP，支持 UDP、共享内存、RDMA 等传输方式。
   - 可以扩展到数千甚至数万个节点同时通信。
5. **应用场景**
   - **车载系统（自动驾驶感知/控制数据）**
   - **工业物联网（IIoT）**
   - **机器人系统（ROS 2 默认中间件就是 DDS）**
   - **金融交易、国防雷达与指挥系统**

一句话总结：

- DDS 就是一个 **面向实时系统的“分布式数据总线”**，解决了机器之间在复杂网络环境下的数据交换问题。

---

## DDS 与 ROS2

> DDS的核心通信模型为订阅者/发布者模型，那么基于它实现的ROS2中的Server和Aciton是如何实现的？

`其实都是基于发布订阅模型，继续进行封装以实现。`

**ROS 2 的 Topic、Service、Action 全部建立在这套机制上**，避免了不同通信模式使用不同底层机制带来的复杂性。

同时，同样可以使用，DDS 强大的 **QoS 策略**。

---

### ROS2的 request_id 和 DDS 的 Domain 区分理解

> 续接上面那个问题，ROS2的Service(C/S模型)，也是基于Pub/Sub模型实现的。例如: (ChatGPT解释)

ROS 2 为了支持同步 RPC 风格的调用（像 C/S 模型），在 DDS 的基础上做了一层封装：

- **内部机制**：
  - 定义两个隐含的 Topic：
    - `service_request` Topic
    - `service_response` Topic
  - **Client**：
    - 发布请求到 `service_request`
    - 订阅 `service_response`
  - **Server**：
    - 订阅 `service_request`
    - 发布响应到 `service_response`
  - ROS 2 框架会给每个请求分配 **唯一 ID（request_id）**，保证响应能和请求对应。

所以 ROS 2 的 Service 其实就是 **两对 DDS Pub/Sub** 的组合。

> 所以引出了这个问题，我混淆了一个概念，`这里实现的唯一 request_id 和 DDS 配置的 Domain是不是一个东西`？有什么区别？

> ROS2 Service 的 request_id

当你调用一个 Service 时（Client -> Server）：

- ROS 2 会在请求消息里自动加上一个 **唯一的 `request_id`**。
- 这个 ID 用来保证 **请求和响应能一一对应**，即使多个 Client 同时调用同一个 Service，也不会串。
- 本质上它是 `一个消息级别的标识符`。

> DDS 的 Domain

- DDS Domain 是 `逻辑上的通信隔离区`。

- 所有 DDS 节点（Publisher、Subscriber）必须加入同一个 **Domain ID**，才能互相发现并通信。

---

# 核心要点

## Discovery发现机制





---

# Qos策略





---

## FastDDS





