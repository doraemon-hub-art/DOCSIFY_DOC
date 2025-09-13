> 本文，我们将遵循着上来就用的原则，NeoVim还需要自己熟悉配置，不如先使用现成配置好的IDE,然后熟悉使用，之后摸索NeoVim完善自己的配置。

# 安装

- 详见:  [安装 | LunarVim](https://www.lunarvim.org/zh-Hans/docs/installation)

> 我的环境

- 配置存储仓库:  [doraemon-hub-art/MY_LUNARVIM_CONFIG: LunarVim setting.](https://github.com/doraemon-hub-art/MY_LUNARVIM_CONFIG)

```bash
# WSL
Ubuntu 24.04.2 LTS
```

> 根据官方文档的一件安装命令  (注意科学上网更顺利)

```bash
LV_BRANCH='release-1.4/neovim-0.9' bash <(curl -s https://raw.githubusercontent.com/LunarVim/LunarVim/release-1.4/neovim-0.9/utils/installer/install.sh)
```

## 报错解决

> 安装pynvim 解决668安装报错

```bash
xuanxuan@xuan:~/.local/share/fonts$ python3 -m pip install --user --upgrade pynvim
error: externally-managed-environment

× This environment is externally managed
╰─> To install Python packages system-wide, try apt install
    python3-xyz, where xyz is the package you are trying to
    install.

    If you wish to install a non-Debian-packaged Python package,
    create a virtual environment using python3 -m venv path/to/venv.
    Then use path/to/venv/bin/python and path/to/venv/bin/pip. Make
    sure you have python3-full installed.

    If you wish to install a non-Debian packaged Python application,
    it may be easiest to use pipx install xyz, which will manage a
    virtual environment for you. Make sure you have pipx installed.

    See /usr/share/doc/python3.12/README.venv for more information.

note: If you believe this is a mistake, please contact your Python installation or OS distribution provider. You can override this, at the risk of breaking your Python installation or OS, by passing --break-system-packages.
hint: See PEP 668 for the detailed specification.
```

- 解决

```bash
sudo apt update
sudo apt install python3-pynvim
```

之后在安装过程中跳过这步

```bash
Would you like to install LunarVim's Python dependencies: pynvim?
[y]es or [n]o (default: no) : n
```

- 补充

`pynvim` 是 **Neovim 的 Python 插件接口**（Python client for Neovim）。

它让 Neovim 能够调用 **Python 脚本写的插件**，也让 Python 脚本能够直接与 Neovim 交互。

换句话说：
 👉 没有 `pynvim`，Neovim 只能跑 VimScript / Lua 插件。
 👉 装了 `pynvim`，Neovim 才能加载 **基于 Python 的插件**。

> 没有设置默认Rust版本

- 报错

```bash
Would you like to install LunarVim's Rust dependencies: fd::fd-find, rg::ripgrep?
[y]es or [n]o (default: no) : y
Installing missing Rust dependency with cargo
error: rustup could not choose a version of cargo to run, because one wasn't specified explicitly, and no default is configured.
help: run 'rustup default stable' to download the latest stable release of Rust and set it as your default toolchain.
```

- 解决

```bash
# 这一步会下载并激活 stable 版本的 Rust + Cargo。
rustup default stable

# 确认Cargo可用
cargo --version
rustc --version
```

- 补充

LunarVim 的安装脚本里会尝试用 **Cargo** 安装两个常用的搜索工具：

- **`ripgrep (rg)`** → 用来在文件里做快速内容搜索（比 `grep` 快很多）。
   在 Lvim 里，像 Telescope、`live_grep`、LSP references 等都依赖 `rg`。
- **`fd (fd-find)`** → 快速文件搜索工具（比 `find` 简洁很多）。
   在 Lvim 里，文件切换、模糊搜索通常会调用 `fd`。

如果系统里没有现成的 `rg` / `fd`，Lvim 就会尝试通过 `cargo install ripgrep fd-find` 来装。

反正就是一些相关的依赖。

> 完成

至此，终端输入lvim，成功进入到lunarVim界面，开启使用。

 ![image-20250913150518959](https://oss.banshengua.top//blogimages/image-20250913150518959.png)

---

# 常用命令

- 参考键位: [键位总览](https://www.lunarvim.org/zh-Hans/docs/master/beginners-guide/keybinds-overview)

## 全局搜索文件

```bash
learder +  ff
```

- 不区分大小写配置

```bash
-- 忽略大小写搜索
vim.opt.ignorecase = true
vim.opt.smartcase = true

-- 让 telescope 也遵守 smart-case
local telescope = require("telescope")

telescope.setup {
  defaults = {
    vimgrep_arguments = {
      "rg",
      "--color=never",
      "--no-heading",
      "--with-filename",
      "--line-number",
      "--column",
      "--smart-case",
    },
  },
}
```

---

## 撤销

```bash
u
```

---

## 打开文件树

```bash
leader + e
```

---

## 上一个/下一个光标位置

```bash
Ctrl + o / i
```

---

## 定义/声明/实现/信用 跳转

```bash
跳转到定义	gd	
跳转到声明	gD
跳转到实现	gi  有点问题，貌似是LSP认为定义和实现是一个东西？gd可以在定义和实现间切换
查找引用	gr
```

---

## 上下左右窗口焦点切换

```bash
# 左 下 上 右
# h j k l
Ctrl + h / l
```

---

## 窗口大小控制

```bash
# 在当前窗口中
Ctrl + 左右上下
```

---

## 打开一个终端窗口

```bash
# 打开和隐藏
Ctrl + 4
```

---

## 关闭所有窗口并退出

```bash
!qa
```



---

# LSP

> 激活对应语言的服务器

```bash
:Mason 
```

> C++ compile_commands.json

```bash
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

> 用 Bear（适用于非 CMake 项目）

```
sudo apt install bear    # Debian/Ubuntu
```

- 编译项目时用 Bear 记录：

```
bear -- make
```

- 会生成 `compile_commands.json`

> 重新加载

```bash
:LspRestart
```



---

# Git



---

# 添加NeoVim插件

> 配置文件目录

```bash
/home/xuanxuan/.config/lvim
```

> 添加插件

在 `config.lua` 中，用 `lvim.plugins` 配置。例如：

```bash
lvim.plugins = {
  -- 显示CPU、内存的插件（例如 btop.vim）
  { "voldikss/vim-floaterm" },
}
```

---

