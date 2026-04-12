---

marp: true

---

# 目录解析

以Ubuntu为例，NVim的配置路径在 ~/.config/nvim/，目录结构如下所示:

```bash
├── init.lua # bootstrap the lazy plugin manager
├── lazy-lock.json
├── lua
│   ├── community.lua # 【AstroNvim】 import AstroCommunity plugin specs
│   ├── lazy_setup.lua # configure and setup lazy.nvim
│   ├── plugins # configure plugins
│   │   ├── astrocore.lua # 【AstroNvim】
│   │   ├── astrolsp.lua # 【AstroNvim】
│   │   ├── astroui.lua # 【AstroNvim】
│   │   ├── format_config.lua
│   │   ├── git_plugins.lua
│   │   ├── mason.lua
│   │   ├── neogen.lua
│   │   ├── none-ls.lua
│   │   ├── themes.lua
│   │   ├── treesitter.lua
│   │   └── user.lua
│   └── polish.lua # 【AstroNvim】file executed last with arbitrary Lua
├── neovim.yml
├── README.md
└── selene.toml
```

---

## 核心文件

> community.lua

从AstroNvim丰富的社区中引入插件，这个在下方插件引入我们再仔细介绍。

> astrocore.lua

- 用作核心功能的主要配置，包括: 
  - Vim Option（Vim的一些内置配置，比如行号显示、缩进等）;
  - User Commands（： 命令模式的自定义的指令）等；
  - Global Mappings (全局映射快捷键);
  - ......

> astroui.lua


---

> astrolsp.lua

- 用于配置语言服务器，包括:
  - 格式化；
  - Lsp行为；
  - ......

什么是LSP? 
LSP即 Language Server Protocol，翻译过来叫做“语言服务器协议”，简单来说就是一套通用的代码解析协议，例如pyright为python提供语言支持，clangd为C/C++提供语言支持，让Vim从单纯的文本编辑器，彻底成为了IDE（集成变编译环境），为你提供代码补全、跳转、报错等编码工具。

> themes.lua
