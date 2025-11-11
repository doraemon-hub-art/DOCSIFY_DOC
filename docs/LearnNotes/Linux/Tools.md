> Ubuntu Linux下的一些工具&小技巧；

积累下来，方面后续快速配置开发环境。
有些基础的用法，也可以汇总成一期小视频？得看看是否有价值。因为这种东西晚上一搜一大把，AI一问全部知晓。

---

# terminator 超级终端

> 参考文章 

- [ubuntu安装超级终端（实现分屏功能）](https://blog.csdn.net/qq_45087381/article/details/132406395)

> 快捷键

- 按住Ctrl 点 Tab 切换窗口焦点；
- 

---

# 字体设置

## 安装字体

1. 下载相关字体文件.zip or .tff；
2. 安装；

```bash
sudo cp /path/to/your/font.ttf /usr/share/fonts/ # (或者安装到用户目录 ~/.fonts)
sudo fc-cache -fv # 更新字体缓存
```

3. 选择字体然后愉快地使用；

> 补充字体文件格式

- .dfont为MacOS字体格式；
- .ttf为Linux字体格式；

---

## 图形化工具

> font manager

- 安装

```bash
sudo apt install font-manager
```

---

# ZIP解压

## 常用命令

> 解压到指定目录

```bash
unzip file.zip -d /path
```

---

# Appimage

Linux下的一种便捷性软件，不用把应用程序安装在文件系统相应的目录中，而是在运行时挂载。

## 使用

```bash
# 权限
chmod +x xx.appimage  # 注意小写x，+ 跟chmod空格隔开
# 安装 fuse 允许非特权用户创建自己的文件系统，对于一些Appimage文件是必须的
sudo apt update
sudo apt install fuse
```

---

## 集成到系统快速搜索中

```bash
# 官方的PPA
sudo add-apt-repository ppa:appimagelauncher-team/stable
# 更新软件源
sudo apt update
# 安装
sudo apt install appimagelauncher
# 再次启动你的appimage文件
./xxx.appimage
# 此时AppimageLauncher会提示你是不是要集成到系统菜单，如下图所示
# ok 、integrate and run
# 此时再在系统菜单搜索就能看到了
```

![](https://oss.banshengua.top/blogimages/202510212032565.png)

![](https://oss.banshengua.top/blogimages/202510212033122.png)

---

# Redshift设置屏幕色温

> 安装

```bash
sudo apt install redshift
```

> 例如:

```bash
redshift -O 5000  # 例如设置色温为 5000K
```

---

# 带有回收站的删除操作 trash-cli

https://github.com/andreafrancia/trash-cli/blob/master/README_zh-CN.rst

```bash
#  安装
sudo apt install trash-cli
# 文件移动到回收站
transh-put 
```

---

# 多仓库管理工具 myrepos

https://myrepos.branchable.com/

> 将myrepos配置文件，添加到信任中，之后mr才能执行.mrconfig中的动作

```bash
echo "/home/xuan/workspace/code/test/.mrconfig" >> ~/.mrtrust
```

> 基本格式

```bash
# 当前所在文件夹就是一个目录
[.] 
checkout = git checkout 

# 指定路径，在此文件夹中执行对应的git操作
[common_tools]
checkout = git clone https://github.com/doraemon-hub-art/DOCSIFY_DOC.git
```

> 指定某个配置文件进行mr的操作 

> mr update

```bash
# 当执行，mr update时
# 路径存在，则执行 pull 操作；
# 路径不存在，则自动回退到 mr checkout 操作，即上面自己配置的拉取操作，如果也没有自定义checkout 配置，则报错；
mr -c .mrconfig_test update
```

> 


---

# cache 缓存编译工具

> 安装

```bash
sudo apt install ccache      # Debian/Ubuntu
```

> 缓存C/C++ 编译结果，显著加速反复编译的场景。

概述: 第一次编译后，将结果缓存起来，如果源码没变，直接把缓存拿出来用。


---