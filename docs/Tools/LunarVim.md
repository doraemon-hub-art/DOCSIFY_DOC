> 本文，我们将遵循着上来就用的原则，NeoVim还需要自己熟悉配置，不如先使用现成配置好的IDE,然后熟悉使用，之后摸索NeoVim完善自己的配置。

# 常用命令

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

# 打开文件树

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
跳转到实现	gi
查找引用	gr
```

---

