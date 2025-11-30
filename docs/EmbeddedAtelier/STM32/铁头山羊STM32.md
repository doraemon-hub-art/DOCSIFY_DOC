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

> 什么是IO引脚的最大输出速度？

向IO引脚交替写0 1 且输出不失帧的最快速度。

> 上升时间 下降时间 保持时间

![](https://oss.banshengua.top/blogimages/20251130161118.png)

保持时间: 输出有效电压的时间。

> 什么限制了IO的最大输出速度？

上升时间，下降时间的长段，影响IO引脚输出速度。

> 最大输出速度应如何选择？

选取满足要求的最小值，过于陡峭得边沿，会增加耗电，并引入EMI问题。

---

### LED 闪灯实验

> 接法

![](https://oss.banshengua.top/blogimages/20251130163559.png)

> 测试代码

```C
#include "stm32f10x.h"
#include "delay.h"

int main(void)
{
	// open gpioc clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// 初始化引脚，通用输出开漏模式 2MHz
	GPIO_InitTypeDef gpio_init_struct = {0};
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_13;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &gpio_init_struct);

	while(1){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		Delay(100);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		Delay(100);
	}
}
```

---

## 四种输入模式

- 输入上拉
- 输入下拉
- 输入浮空
- 模拟模式

电流从高电压流向低电压。

施密特触发器，将IO输入进来的高低电压，转换成数组 0 or 1，保存在输入输出寄存器中。

![](https://oss.banshengua.top/blogimages/20251130195042.png)

### 按钮亮灯实验

```C
#include "stm32f10x.h"
#include "delay.h"

int main(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct = {0};
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_1;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
	// 最大输出速度只对输出模式有效
	// gpio_init_struct.GPIO_Speed = GPIO_Speed_2MHz;
	 
	GPIO_Init(GPIOA, &gpio_init_struct);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	while (1) {
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET) {
				GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_SET);
			} else {
				GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET);
			}
	}
}
```
