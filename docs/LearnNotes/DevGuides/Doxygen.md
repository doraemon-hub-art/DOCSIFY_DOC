> Doxygen注释风格，可以直接导出API文档。

> 嗯，这个不错，可以学习一下，做一个简单的分享。

从配置到基本的使用，进阶的需要继续查看文档学习。

简单介绍(是什么) -> 演示效果 -> 进入正题 -> 安装、配置 、用法 -> 结尾

---

> 参考资料

官方文档: 
—— https://www.doxygen.nl/
—— https://doxygen.cn/manual/index.html (中文)

程序员的自我修养Doxygen (这个网站结构不错) 
—— https://www.midlane.top/wiki/pages/viewpage.action?pageId=10060993

---

# 前言

> 例如: pulaseAudio的官方API文档就是用Doxygen生成的 ?



---

# 环境配置

## Doxygen 安装

> Ubuntu APT 直接安装

```bash
sudo apt install doxygen
doxygen --version
```

---

## VSCode插件

> Doxygen Documentation Generator

—— https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen


TODO: 回去试一下，不装Doxygen，只装这个插件看有没有效果

Doxygen生成文档的关键在于提取工程中的注释。

> 快速生成函数、变量、文件等描述注释的格式；

```bash
/** 
```

打完**后，回车，它会根据下方内容生成对应的默认格式，如下图所示:

![](https://oss.banshengua.top/blogimages/202510291953463.png)

---

# Doxygen用法

## 基本操作

顺着官方文档简单走下。

1. 创建配置文件 —— 配置文件来规定Doxygen的所有设置；

例如: 输入到哪里，解析什么文件，忽略什么目录......等。

```bash
# 生成配置模板
# 文件名可有可无，默认名字为Doxyfile
doxygen -g <config-file> 

# 或是自己自定义名字，在其中填入内容即可
```

TODO: 说明下配置文件语法

> 一些关键配置项说明

| 配置项             | 用法                      | 说明                             |
| ---               | ---                      | ---                              |
| FILE_PATTERNS     | 设置Doxygen要解析的文件类型 | 例如 *.hpp *.cpp 多个中间用空格分割  |
| OUTPUT_DIRECTORY  | 设置生成文件的输出目录       | 默认为当前文件所在目录              |
| 

---

2. 生成文件

```bash
doxygen <config_file>
```

3. 注释说明

这里面不用太关心，直接使用我们上面解释的VSCode插件。

TODO: 好好配置下VSCode的这个插件即可。

## 实例演示

TODO: 拉一个小的开源项目



