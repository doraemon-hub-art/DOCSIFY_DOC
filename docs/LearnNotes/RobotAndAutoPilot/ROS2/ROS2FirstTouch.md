> 前言

本篇笔记来源于:

 [古月·ROS2入门21讲 | 带你认识一个全新的机器人操作系统](https://www.bilibili.com/video/BV16B4y1Q7jQ/?share_source=copy_web&vd_source=ada30d7f7831e09a49f20956452f6da9)

[本书使用说明 - 图书资源](https://book.guyuehome.com/ROS2_Book/0.前言/)

---

# 前言

> 机器人发展史

电气时代->数字时代->智能时代

> ROS的诞生——标准的**机器人操作系统**

发源于斯坦福的想法。目标——**提高软件复用率**。目前是机器人领域的普遍标准。

![](http://oss.banshengua.top//blogimages/20250820002123583.png)

ROS2——在ROS的基础上发展的更加标准化。

> ROS1的局限性 —— PR2家庭服务机器人

- 需要在资源有限的嵌入式系统中运行；
- 需要在有干扰的地方保证可靠通信；
- 做成产品走向市场；

> ROS2的诞生 —— 需要满足更多的需求

- 多机器人系统；
- 跨平台；
- 实时性；
- 网络连接；
- 产品化；
- 项目管理；

![image-20250907190432446](http://oss.banshengua.top//blogimages/image-20250907190432446.png)

与ROS1相比，ROS2最大的变化是，由Master节点同一管理，变为各个节点通过自发现机制，找到彼此，进行稳定连接。

通信协议由TCP&UDP，变为DDS系统 —— DDS是物联网中广泛应用的一种通信协议。

---

# 核心概念

![image-20250907191013496](http://oss.banshengua.top//blogimages/image-20250907191013496.png)

## 工作空间:

> 存放项目开发相关文件的文件夹，是开发过程的大本营。

- ROS2的典型工作空间

![image-20250906165817038](https://oss.banshengua.top//blogimages/image-20250906165817038.png)

> rosdepc —— 国内作者开发的ROS环境配置工具

很方便用于安装依赖

> colcon —— ROS2 优化后的编译器

> 指定环境变量以便 ros2 run命令能找到对应的功能包

## 功能包

> 创建功能包 —— 用于复用

```bash
ros2 pkg create --build-type <build-type> <package_name>
```

- **pkg**：表示功能包相关的功能；
- **create**：表示创建功能包；
- **build-type**：表示新创建的功能包是C++还是Python的，如果使用C++或者C，那这里就跟ament_cmake，如果使用Python，就跟ament_python；
- **package_name**：新建功能包的名字。

例如: 创建C++和Python版本的功能包

```bash
cd ~/dev_ws/src
ros2 pkg create --build-type ament_cmake learning_pkg_c       # C++
ros2 pkg create --build-type ament_python learning_pkg_python # Python
```

---

## 节点

> 节点的特征

- 执行具体任务的进程；
- **独立运行**的可执行文件；
- 可使用不同的编程语言；
- 可**分布式**运行在不同主机；
- 通过**唯一节点名**称进行管理；

> 节点不是孤立的 —— **节点间有很多机制使其保持联系**(我们，同舟共济) 

![image-20250906175119414](http://oss.banshengua.top//blogimages/image-20250906175119414.png)

这些节点(功能)可能位与不同的计算机中，他们都是这个机器人的一部分，多个节点，组成了一个完整的机器人系统。

> 节点运行

```bash
# ros2 run 包名 节点名
ros2 run learning_node node_helloworld
```

> 代码实现 Python or C++，面向过程 or 面向对象

Python面向对象实现示例:

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
@作者: 古月居(www.guyuehome.com)
@说明: ROS2节点示例-发布“Hello World”日志信息, 使用面向对象的实现方式
"""

import rclpy                                     # ROS2 Python接口库
from rclpy.node import Node                      # ROS2 节点类
import time

"""
创建一个HelloWorld节点, 初始化时输出“hello world”日志
"""
class HelloWorldNode(Node):
    def __init__(self, name):
        super().__init__(name)                     # ROS2节点父类初始化
        while rclpy.ok():                          # ROS2系统是否正常运行
            self.get_logger().info("Hello World")  # ROS2日志输出
            time.sleep(0.5)                        # 休眠控制循环时间

def main(args=None):                               # ROS2节点主入口main函数
    rclpy.init(args=args)                          # ROS2 Python接口初始化
    node = HelloWorldNode("node_helloworld_class") # 创建ROS2节点对象并进行初始化
    rclpy.spin(node)                               # 循环等待ROS2退出
    node.destroy_node()                            # 销毁节点对象
    rclpy.shutdown()    
```

除此之外，还需要让系统知道Python程序的入口。

```python
entry_points={
    'console_scripts': [
        'node_helloworld       = learning_node.node_helloworld:main',
        'node_helloworld_class = learning_node.node_helloworld_class:main',
    ],
```

> 实现节点的流程

- 编程接口初始化；
- 创建节点并初始化；
- 实现节点功能；
- 销毁节点并关闭接口；

---

## 话题

> 节点间传递数据的桥梁 —— 使用 基于DDS的发布/订阅模型。

- 订阅者或发布者可以不唯一；
- 异步通信机制；
- .msg文件定义通信的消息结构；

![image-20250907001114677](http://oss.banshengua.top//blogimages/image-20250907001114677.png)

> 创建话题发布者的程序流程

- 编程接口初始化；
- 创建节点并初始化；
- 创建发布者对象；
- 创建并填充话题消息；
- 发布话题消息；
- 销毁节点关闭接口；

> 创建订阅者的程序流程；

- 编程接口初始化；
- 创建节点并初始化；
- 创建订阅者对象；
- 回调函数处理话题数据；
- 销毁节点并关闭接口；

> 话题特点

- 单向性、周期性传输数据 —— 具有**局限性；**

---

## 服务

> 实现节点间你问我答的，同步通讯机制 —— C/S模型。

- 同步通信机制；
- 服务器唯一，客户端可以不唯一；

- .srv文件定义请求和应答数据结构；

![image-20250907003521505](http://oss.banshengua.top//blogimages/image-20250907003521505.png)

> 客户端实现流程

- 编程接口初始化；
- 创建节点并初始化；
- 创建客户端对象；
- **创建并发送请求数据；**
- **等待服务器端应答数据；**
- 销毁节点并关闭接口；

> 服务端实现流程

- 编程接口初始化；
- 创建节点并初始化；
- 创建服务器端对象；
- **通过回调函数处进行服务；**
- **向客户端反馈应答结果；**
- 销毁节点并关闭接口；

> 示例:

结合 话题 + 服务，可以实现通过3个节点，请求获取图像位置；

- 相机节点，获取相机外设的图像数据，创建一个发布者；
- 服务端节点，创建一个订阅者，订阅相机节点的数据，收到后进行处理，并创建服务端对象，等待客户端消息，等待请求，返回位置；
- 客户端节点，请求服务端，得到结果；

---

## 通信接口

> 数据传递的标准结构 —— 彼此匹配，才能建立连接

![image-20250907112552698](http://oss.banshengua.top//blogimages/image-20250907112552698.png)

> 通信接口定义方式

三种通信方式，消息定义的不同:

- 话题；
- 服务；
- 动作；

![image-20250907110515786](http://oss.banshengua.top//blogimages/image-20250907110515786.png)

---

## 动作

> 完整行为的流程管理 —— C/S 模型

- 服务端唯一，客户端可不唯一；
- 同步通信机制；
- .action文件定义通信接口的数据结构；

![image-20250907113103427](http://oss.banshengua.top//blogimages/image-20250907113103427.png)

应用层的通信机制，底层是依赖话题和服务实现的。

> 特点

- 持续一段时间，过程中会有反馈；

> 示例

![image-20250907113601139](http://oss.banshengua.top//blogimages/image-20250907113601139.png)

> 消息格式 .action

```bash
bool enable     # 定义动作的目标，表示动作开始的指令
---
bool finish     # 定义动作的结果，表示是否成功执行
---
int32 state     # 定义动作的反馈，表示当前执行到的位置
```

---

## 参数

> 机器人系统的全局字典 —— 类似于C++中的全局变量，其实就是运行时的可变参数，可见性为ROS2运行全局。并具有原子性，修改其他节点可见。

- 全局共享字典；
- key value 结构；
- 可实现动态监控 (atomic?)；

---

## 分布式通信

> 多计算平台的任务分配

![image-20250907140942664](http://oss.banshengua.top//blogimages/image-20250907140942664.png)

> 设备处于同一局域网内即可

通过 ROS_DOMAIN_ID 可以划分小组，进行指定设备间的通信。

---

## DDS

> ROS2相对于ROS的最大变化 —— 机器人的神经网络

> 常用通信模型

![image-20250907142509866](http://oss.banshengua.top//blogimages/image-20250907142509866.png)

- **点对点模型**，许多客户端连接到一个服务端，每次通信时，通信双方必须建立一条连接。

- **Broker模型**，针对点对点模型进行了优化，由Broker集中处理所有人的请求，并进一步找到真正能响应该服务的角色
- **广播模型**，所有节点都可以在通道上广播消息，并且节点都可以收到消息。
- **以数据为中心的DDS模型**，这种模型与广播模型有些类似，所有节点都可以在DataBus上发布和订阅消息。节点只需要关心自己感兴趣的消息。

> DDS

![image-20250907142529105](http://oss.banshengua.top//blogimages/image-20250907142529105.png)

比如在自动驾驶领域，通常会存在感知，预测，决策和定位等模块，这些模块都需要**非常高速和频繁地交换数据**。借助DDS，可以很好地满足它们的通信需求。

> 什么是DDS?

DDS的全称是**Data Distribution Service**，也就是**数据分发服务**，2004年由**对象管理组织OMG**发布和维护，是一套专门为**实时系统**设计的**数据分发/订阅标准**，最早应用于美国海军， 解决舰船复杂网络环境中大量软件升级的兼容性问题，现在已经成为强制标准。

DDS强调**以数据为中心**，可以提供丰富的**服务质量策略**，以保障数据进行实时、高效、灵活地分发，可满足各种分布式实时通信应用需求。

> DDS在ROS2中的应用

红色和蓝色即为DDS部分，所有的ROS上层应用都基于DDS之上。

![image-20220528020937717](http://oss.banshengua.top//blogimages/image-20220528020937717.jpg)

> DDS的兼容性

为了实现对多个DDS的兼容，ROS设计了一个**Middleware中间件**，也就是一个统一的标准，不管我们用那个DDS，保证上层编程使用的函数接口都是一样的。

> DDS的 质量服务策略——QoS

接收端和发送端，两个节点，需要在相同Domain小组下，且，需要的QoS配置，才能进行通信。

QoS是一种**网络传输策略**，应用程序指定所需要的网络传输质量行为，QoS服务实现这种行为要求，尽可能地满足客户对通信质量的需求，可以理解为**数据提供者和接收者之间的合约**。

策略示例:

- **DEADLINE**策略，表示通信数据必须要在每次截止时间内完成一次通信；
- **HISTORY**策略，表示针对历史数据的一个缓存大小；
- **RELIABILITY**策略，表示数据通信的模式，配置成BEST_EFFORT，就是尽力传输模式，网络情况不好的时候，也要保证数据流畅，此时可能会导致数据丢失，配置成RELIABLE，就是可信赖模式，可以在通信中尽量保证图像的完整性，我们可以根据应用功能场景选择合适的通信模式；
- **DURABILITY**策略，可以配置针对晚加入的节点，也保证有一定的历史数据发送过去，可以让新节点快速适应系统。

---

# 常用工具

## Launch

> 实现多节点的配置和启动

为了避免节点name唯一冲突，可以添加namespace做隔离。



---

## TF

> 机器人坐标系管理神器



---

## URDF

> 机器人建模



---

## Gazebo

> 三维物理仿真平台

![image-20250907192235370](http://oss.banshengua.top//blogimages/image-20250907192235370.png)

---

## Rviz

> 三维可视化显示平台



---

## RQT

> 模块化的可视化工具

![image-20250907192128214](http://oss.banshengua.top//blogimages/image-20250907192128214.png)



---

# 常用命令

```bash
ros2 node list               # 查看节点列表
ros2 node info <node_name>   # 查看节点信息

ros2 topic list                # 查看话题列表
ros2 topic info <topic_name>   # 查看话题信息
ros2 topic hz <topic_name>     # 查看话题发布频率
ros2 topic bw <topic_name>     # 查看话题传输带宽
ros2 topic echo <topic_name>   # 查看话题数据
ros2 topic pub <topic_name> <msg_type> <msg_data>   # 发布话题消息

rqt_graph # 图形化节点间通信

......
```

---

# 相关进阶学习

> 自动驾驶

- https://www.autoware.org/

> 路径规划

- https://moveit.ros.org/



To be continue......
