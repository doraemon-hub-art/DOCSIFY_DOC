> 笔记来源 —— 铁头山羊stm32 入门教程【新版】 

https://www.bilibili.com/video/BV11X4y1j7si/?spm_id_from=333.788.videopod.episodes&vd_source=f560f33f0a1290eb4abbd2cfb5ca7d21&p=2

---

# 前言

> 开发思路

1. 梳理思路；
2. 搭建电路；
3. 编码；
4. 烧录；
5. 运行；

---

# Stm32基本信息

> STM32 是什么？

STM32 —— 由ST公司生产的32位单片机统称

> 什么角色？干嘛的？

元器件中的管理者。

> STM32 型号含义

| 品牌 | 产品类型 | 产品系列 | 规格型号 |
| --- | --- | --- | --- |
| ST | M32 | F103 | C8T6 |

> 芯片结构

![](https://oss.banshengua.top/blogimages/20251129205202.png)

> 芯片引脚编号规则

- 一般规则: 圆点附近是1角，逆时针递增；

> 特殊引脚


> 普通引脚命名规则


---

# GPIO 

## 四种输出模式

- 通用输出推挽
- 通用输出开漏
- 复用输出推挽
- 复用输出开漏

---

> 推挽 —— Push-Pull

- 推 Push 向外推电流
- 挽 Pull 从外边向里拉电流

![](https://oss.banshengua.top/blogimages/20251129203604.png)

上下MOS管交替打通。
推挽模式下，IO引脚写0 输出低电压，写1输出高电压。

> 开漏

![](https://oss.banshengua.top/blogimages/20251129204350.png)

开漏模式下，上边的MOS管，始终断开，写 0 低电压，写 1 高阻抗。

> 通用模式

CPU直接控制IO引脚。

> 复用模式

控制权交由其他片外设。 

![](https://oss.banshengua.top/blogimages/20251129205829.png)

> 四种输入模式

- 输入上拉
- 输入下拉
- 输入浮空
- 模拟模式

