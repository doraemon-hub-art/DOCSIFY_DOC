# Apple Magic Trackpad 驱动板 - Linux 用户空间驱动

## 项目概述

本项目为一块模拟 Apple Magic Trackpad (05ac:0265) 的驱动板编写 Linux 用户空间驱动。该驱动板在 Ubuntu 下被识别为 Apple 设备，但内核自带的 `magicmouse` 驱动无法正常工作。我们通过读取 HID 原始数据，解析触摸信息，然后通过 `uinput` 模拟鼠标输入。

## 目录结构

```
test/hid_trackpad/
├── README.md              # 本文档
├── hid_probe.c            # HID 设备探测工具 v1 - 基础探测
├── hid_probe2.c           # HID 设备探测工具 v2 - 高级探测
├── hid_analyze.c          # HID 设备分析工具
├── hid_monitor.c          # HID 设备监控工具
├── trackpad_driver.c      # 用户空间驱动主程序
└── trackpad_driver        # 编译后的可执行文件
```

## 相关 Linux 概念

### 1. HID (Human Interface Device)

HID 是 USB 设备的一种标准协议，用于人机交互设备（键盘、鼠标、触摸板、游戏手柄等）。

- **HID 报告描述符 (Report Descriptor)**: 描述设备数据格式的元数据，定义了每个字节的含义
- **HID 报告 (Report)**: 设备发送给主机的实际数据包
- **HID 设备有两个接口 (Interface)**:
  - **Interface 0**: 通常用于按键/基础功能（如鼠标左键、右键）
  - **Interface 1**: 通常用于触摸/多点触控数据

### 2. hidraw

`hidraw` 是 Linux 内核提供的一个字符设备接口，允许用户空间程序直接读写 HID 设备的原始数据。

- 设备节点: `/dev/hidraw0`, `/dev/hidraw1`, ...
- 优点: 可以绕过内核驱动，直接获取设备的原始 HID 报告
- 缺点: 需要自己解析数据格式

**如何找到正确的 hidraw 设备：**
```bash
# 查看所有 hidraw 设备及其对应的 USB 设备路径
for f in /dev/hidraw*; do
    echo "--- $f ---"
    udevadm info --query=all --name="$f" | grep DEVPATH
done
```

### 3. uinput

`uinput` 是 Linux 内核提供的一个虚拟输入设备接口，允许用户空间程序创建和发送输入事件。

- 设备节点: `/dev/uinput`
- 功能: 可以模拟键盘、鼠标、触摸板、游戏手柄等任何输入设备
- 使用流程:
  1. 打开 `/dev/uinput`
  2. 设置设备支持的事件类型 (`UI_SET_EVBIT`)
  3. 设置设备支持的按键/相对轴/绝对轴 (`UI_SET_KEYBIT`, `UI_SET_RELBIT`, `UI_SET_ABSBIT`)
  4. 创建设备 (`UI_DEV_CREATE`)
  5. 发送输入事件 (`write` 写入 `struct input_event`)
  6. 销毁设备 (`UI_DEV_DESTROY`)

### 4. input 子系统

Linux 输入子系统是处理所有输入设备的内核框架。

- **设备节点**: `/dev/input/event*` (事件设备), `/dev/input/mouse*` (鼠标设备)
- **事件类型**:
  - `EV_KEY`: 按键事件 (BTN_LEFT, BTN_RIGHT, BTN_TOUCH 等)
  - `EV_REL`: 相对坐标事件 (REL_X, REL_Y, REL_WHEEL 等)
  - `EV_ABS`: 绝对坐标事件 (ABS_X, ABS_Y, ABS_MT_POSITION_X 等)
  - `EV_SYN`: 同步事件 (SYN_REPORT)

### 5. 内核驱动 vs 用户空间驱动

| 特性 | 内核驱动 | 用户空间驱动 |
|------|---------|-------------|
| 开发难度 | 高 | 低 |
| 调试难度 | 高 | 低 |
| 性能 | 高 | 中 |
| 灵活性 | 低 | 高 |
| 安全性 | 内核态 | 用户态 |

本项目采用用户空间驱动方案，通过 `hidraw` 读取数据，通过 `uinput` 模拟输入。

## 数据解析过程

### 步骤 1: 识别设备

```bash
lsusb  # 查看 USB 设备
# Bus 001 Device 003: ID 05ac:0265 Apple, Inc. Magic Trackpad
```

### 步骤 2: 找到对应的 hidraw 设备

```bash
# 查看 hidraw 设备与 USB 设备的对应关系
for f in /dev/hidraw*; do
    echo "--- $f ---"
    udevadm info --query=all --name="$f" | grep DEVPATH
done
```

输出示例：
```
--- /dev/hidraw2 ---
E: DEVPATH=/devices/.../1-2.4:1.0/.../hidraw/hidraw2  # Interface 0 (鼠标)
--- /dev/hidraw3 ---
E: DEVPATH=/devices/.../1-2.4:1.1/.../hidraw/hidraw3  # Interface 1 (触摸)
```

### 步骤 3: 读取原始数据

使用 `hid_probe.c` 读取设备发送的原始 HID 报告：

```
收到 30 字节: 04 03 D3 03 6E 02 07 63 04 52 03 0B 90 02 86 02 ...
```

### 步骤 4: 解析数据格式

通过分析大量数据样本，结合 Apple 的 HID 报告描述符规范，推断出数据格式：

```
偏移  长度  说明
─────────────────────────────────────
[0]    1    报告 ID (0x04 = 触摸报告)
[1]    1    触点数 (低4位, 但实际是槽位数)
[2-3]  2    触点1 X坐标 (16位小端)
[4-5]  2    触点1 Y坐标 (16位小端)
[6-7]  2    触点2 X坐标
[8-9]  2    触点2 Y坐标
[10-11] 2   触点3 X坐标
[12-13] 2   触点3 Y坐标
[14-15] 2   触点4 X坐标
[16-17] 2   触点4 Y坐标
[18-19] 2   触点5 X坐标
[20-21] 2   触点5 Y坐标
[22-23] 2   触点6 X坐标
[24-25] 2   触点6 Y坐标
[26-27] 2   时间戳
[28]    1   状态标志1
[29]    1   状态标志2
```

**关键发现：**
- `buf[1]` 的低4位表示**槽位数**（slot count），不是实际手指数
- Apple 设备总是报告 3 个槽位，即使只有 1 根手指
- 有效触点通过坐标是否为 0 来判断
- 坐标是**绝对坐标**，范围约 0-1300

### 步骤 5: 数据样本分析

**单指触摸（实际 1 根手指，报告 3 个槽位）：**
```
04 03 D3 03 6E 02 07 63 04 52 03 0B 90 02 86 02 ...
   ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^
   触点1    触点2    触点3
   (979,622) (1123,850) (2960,646)
```

**手指抬起（所有坐标归零）：**
```
04 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ...
   ^^ ^^ ^^ ^^
   触点1=0
```

**手指计数变化：**
```
04 03 ...  # 3个槽位有数据 (实际1根手指)
04 01 ...  # 1个槽位有数据 (手指抬起过程中)
04 03 ...  # 再次触摸
```

### 步骤 6: 驱动实现

驱动的工作流程：

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  HID 设备    │────▶│  hidraw 读取  │────▶│  数据解析    │
│  (驱动板)    │     │  /dev/hidrawN │     │  解析坐标/   │
└─────────────┘     └──────────────┘     │  手指计数    │
                                          └──────┬──────┘
                                                 ▼
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  鼠标移动    │◀────│  uinput 发送  │◀────│  事件处理    │
│  屏幕显示    │     │  /dev/uinput │     │  移动/点击/  │
└─────────────┘     └──────────────┘     │  滚轮       │
                                          └─────────────┘
```

## 遇到的问题

### 1. magicmouse 驱动冲突

内核自带的 `magicmouse` 驱动会绑定设备，导致我们的驱动无法正常工作。

**解决方案：** 卸载冲突驱动
```bash
sudo modprobe -r hid_magicmouse
sudo modprobe -r hid_multitouch
```

### 2. hidraw 设备号变化

卸载/加载驱动后，hidraw 设备号会变化（如 `/dev/hidraw3` 变为 `/dev/hidraw11`）。

**解决方案：** 通过 `udevadm` 动态查找正确的设备。

### 3. 绝对坐标 vs 相对坐标

驱动板报告的是**绝对坐标**（类似触摸屏），但鼠标需要**相对坐标**（差值）。

**解决方案：** 计算相邻两次报告的坐标差值，作为相对移动发送。

### 4. 手指计数误判

`buf[1]` 的值不是实际手指数，而是槽位数。Apple 设备总是报告 3 个槽位。

**解决方案：** 直接使用 `buf[1] & 0x0F` 作为手指计数（虽然不准确，但能区分单指/双指）。

## 编译和运行

```bash
# 编译
cd test/hid_trackpad
gcc -o trackpad_driver trackpad_driver.c

# 运行 (需要 root 权限访问 hidraw 和 uinput)
sudo ./trackpad_driver

# 卸载冲突驱动 (如果必要)
sudo modprobe -r hid_magicmouse
sudo modprobe -r hid_multitouch
```

## 待改进

1. **自动查找设备**: 通过扫描 `/dev/hidraw*` 自动找到正确的设备
2. **动态加载/卸载**: 自动卸载冲突驱动
3. **多点触控手势**: 支持三指、四指手势
4. **配置界面**: 允许调整灵敏度、滚轮速度等参数
5. **绝对坐标映射**: 将绝对坐标映射到屏幕坐标，实现更精确的控制
