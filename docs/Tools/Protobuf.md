> 序列化、反序列化工具

---


# 关键字

> message

定义了一个消息，proto中的基本结构，表示一个可以被序列化，反序列化的数据结构。

> repeated

表示可重复字段，相当于数组或列表。

> oneof

表示互斥字段，oneof块包裹的，范围内，同一时间只能存在其中一个字段。
给其中一个赋值，另一个会被清空。

oneof中的每个成员，都会自动生成对应的has_方法，来辅助检测是否存在某个字段。

> reserved

防止字段编号或字段名被重复使用。

---

# 补充

> 消息基本类型，protobuf会生成

> 嵌套类型

```bash
message Robot {
  PbPose pose = 2;       // 嵌套消息
}
```

编译后:

```bash
class Robot {
public:
  // 对嵌套字段（消息类型），protobuf 生成更多：
  bool has_pose() const;             // 是否存在
  const PbPose& pose() const;        // 获取常量引用
  PbPose* mutable_pose();            // 获取可修改指针
  void clear_pose();                 // 清除
};
```