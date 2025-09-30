> C++内存检测工具Valgrind

# 交叉编译源码在aarch64系统上运行

以3.32版本为例。

> 下载源码

```bash
wget https://sourceware.org/pub/valgrind/valgrind-3.23.0.tar.bz2
tar -xjf valgrind-3.23.0.tar.bz2
cd valgrind-3.23.0
```

> 运行构建脚本configure

- 会尝试找到交叉编译工具链（aarch64-poky-linux-gcc、aarch64-poky-linux-ld 等）。

- 会检测系统特性（类型大小、字节序、函数可用性）针对目标架构。

- 生成适合目标架构的 Makefile，而不是本机的 Makefile。

```bash
# 注意在valgrind-3.23.0目录下

# source交叉工具栏环境脚本
source xx
# 示例
# --host 目标系统 --build 当前构建系统
# 我在 x86_64 Linux 上编译 Valgrind，但是目标是 aarch64 的车机系统。
./configure --host=aarch64-poky-linux --build=x86_64-linux-gnu
```

> 编译&安装

```bash
make -j14

# 安装到当前工作目录指定文件夹
make DESTDIR=$PWD/_install install
```

由于编译安装的时候默认前缀是 /usr/local，所以生成的 valgrind 在：


```bash
_install/usr/local/bin/valgrind
_install/usr/local/lib/valgrind/
```
> 直接推送到车机目标目录(以ADB示例)

```bash
# 可执行文件
adb push _install/usr/local/bin/valgrind /usr/bin/
# 运行时依赖的库和模块
adb push  _install/usr/local/lib/valgrind /usr/lib/
adb push  _install/usr/local/libexec/valgrind/* /usr/lib/valgrind/
```

> 运行测试

```bash
valgrind
```

如下提示即可:

```bash
valgrind: no program specified
valgrind: Use --help for more information.
```

# 示例

