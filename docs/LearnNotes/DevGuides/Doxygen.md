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


~~TODO: 回去试一下，不装Doxygen，只装这个插件看有没有效果。(有的兄弟，有的。)~~

Doxygen生成文档的关键在于提取工程中的注释。

> 快速生成函数、变量、文件等描述注释的（默认）格式；

```bash
/** 
```

打完**后，回车，它会根据下方内容生成对应的默认格式，如下图所示:

![](https://oss.banshengua.top/blogimages/202510291953463.png)

当然，你也可以把触发生成的注释的关键字改为其他的，比如: /// ,感觉这样更方便一些，而且不会对原本的多行注释造成影响。

下面会对相关配置参数做出讲解。

---

### 插件参数配置

插件配置说明: https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen

可以看到，在VSCode界面，有一些常用的参数设置，而更多更高级的设置，需要在setting.json中进行手动配置。

setting.json配置文件中，没有指定的参数，插件是有默认值的。






---

# Doxygen用法

## 生成接口文档

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
| INPUT             | 设置目标解析代码目录         | 多个目录用空格分割                 | 

---

2. 生成文件

```bash
doxygen <config_file>
```

3. 注释格式说明

这里面不用太关心，直接使用我们上面解释的VSCode插件即可。

TODO: 好好配置下VSCode的这个插件即可。

---

### 注释参数

> 文档风格

Javadoc:

Qt:

---

#### Javadoc 注释说明

TODO: 筛选，将常用的归到一起。

| 标签 | 说明 | 示例 |
|------|------|------|
| `@param` | 描述方法参数，参数名必须与方法定义一致 | `@param count 要生成的元素数量` |
| `@return` | 描述方法的返回值 | `@return 平均值` |
| `@throws / @exception` | 描述方法可能抛出的异常及触发条件 | `@throws IOException 文件读取失败` |
| `@author` | 标明作者信息 | `@author Zhao` |
| `@version` | 指定版本号 | `@version 1.2` |
| `@since` | 指示该类或方法从哪个版本开始存在 | `@since 1.8` |
| `@deprecated` | 标记该元素已弃用，可说明替代方案 | `@deprecated 使用 newApi() 代替` |
| `@see` | 引用相关类、方法或文档 | `@see java.util.List` |
| `@link` | 在文档中插入超链接 | `{@link Math#sqrt(double)}` |
| `@linkplain` | 类似 `@link`，但显示为普通文本 | `{@linkplain StringBuilder}` |
| `@code` | 插入行内代码，固定宽度字体 | `@code for(int i=0;i<10;i++)` |
| `@literal` | 显示原始文本，不解析 HTML 标签 | `@literal <html>` |
| `@docRoot` | 生成链接时指代文档根路径 | `{@docRoot}/overview-summary.html` |
| `@implNote` | 实现细节说明（Java 8+） | `@implNote 本方法非线程安全` |
| `@implSpec` | 描述方法的实现规范（Java 8+） | `@implSpec 子类必须调用 super` |
| `@apiNote` | 面向 API 使用者的额外说明（Java 9+） | `@apiNote 仅在调试模式下可用` |
| `@serial` | 描述可序列化字段 | `@serial 存储版本号` |
| `@serialField` | 描述 `ObjectStreamField` | `@serialField name String 字段名` |
| `@serialData` | 描述序列化数据格式 | `@serialData 以 UTF-8 输出` |
| `@value` | 显示常量字段值 | `@value` |


---

## 生成代码调用图



---

## 实例演示

TODO: 拉一个小的开源项目

恰好这个zmqpp就是
https://github.com/zeromq/zmqpp

---

## 概念补充


