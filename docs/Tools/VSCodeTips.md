> 简单记录一些VSCode的小技巧

---

> 代码运行插件 CodeRunner

- 作者知乎文章: https://zhuanlan.zhihu.com/p/54861567

---

> VSCode中编写知乎文章  (已废弃)

配置评论系统的时候刷到的，不过貌似现在作者停止维护了。

![](https://oss.banshengua.top//blogimages/20251019160555275.png) 

---

> 光标移动效果

来源——[VSCode深度配置 - settings.json](https://juejin.cn/post/7369029201579343887#heading-3)

确实用起来不错，不知道结合VIM插件用起来怎么样？
不过NEOVIM插件配置本身好像就有光标移动的配置。

```bash
"editor.smoothScrolling": true,
"editor.cursorBlinking": "expand",
"editor.cursorSmoothCaretAnimation": "on",
"workbench.list.smoothScrolling": true,
```

---

> 打开setting.json

```bash
ctrl + p 
# 输入 > user setting
```

![](https://oss.banshengua.top//blogimages/20251102233110433.png)

---

> 文件搜索设置，搜索 use ignore 取消勾选

否则只能搜到打开过的文件

![](https://oss.banshengua.top/blogimages/202511061748923.png)

---

> 选中高亮

```bash
"workbench.colorCustomizations": {
    "editor.wordHighlightBackground": "#FFD70033",   // 选中词语的高亮背景
    "editor.wordHighlightStrongBackground": "#FFA50055", // 更强烈的高亮
    "editor.wordHighlightBorder": "#FF8C0033"        // 可选：边框增强对比
}
```

---

# NeoVim插件

一直断断续续地想讲开发环境从VSCode迁移至NeoVim，但是上手陡峭，总是无法坚持，索性将VSCode也试着改造下，兴许转变路径能够平缓些。

> 插件安装


> 环境要求

- nvim安装

```BASH
# 安装 

# 查看
nvim --version
# 查看安装位置
whitch nvim
```

> vscode-nvim 配置

- 为了避免不必要的冲突，有些nvim配置并不完全适配vscode-nvim，所以还是建议从一个空的init.lua (~/.config/nvim/init.lua)文件开始；
- 
---

