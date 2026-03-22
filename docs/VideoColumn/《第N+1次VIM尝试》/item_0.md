# 前言

经过反复的放弃，然后尝试，我终于下定决心要学习使用vim，以替换掉我平时开发使用的VSCode，更多的使用键盘，避免鼠标键盘之间的切换。

从NeoVim，到LunarVim，再到AstroNVim，我想，这次我一定可以坚持下来。

这个系列视频，我会不断分享自己在Vim上的一些使用技巧与心得。希望可以对你有一些帮助。

本次我们将选择AstroNVim作为环境与基准工具，不过实际上他也只是在NVim上的一层套壳，所以下面的内容，我们均称其为NVim。

NeoVim是Vim的现代进化版，它有着很多现代的插件，而AstroNVim，是NeoVim精装修版，直接在NeoVim的基础上，预装了很多插件和设置，真正意义上的可以开箱即用。

作为本系列的第一期视频，我会简单介绍其安装与环境配置，同时分享一些我在初步接触后，学习上的一些技巧。

---

# 环境配置

> 参考官方Requirement

https://docs.astronvim.com/#-requirements

1. 安装终端字体Mono；
2. 安装更好用的终端软件 Tabby 并配置；
3. 安装Neovim 0.10 +;
  
```bash
# 下载.appimage ；
https://github.com/neovim/neovim/releases/tag/stable 
# 重命名
mv nvim-linux-x86_64.appimage nvim；
# 权限&全局安装
chmod +x ./nvim；
mv ./nvim /usr/local/bin
nvim --version
```

4. 导入astronvim

```bash
git clone --depth 1 https://github.com/AstroNvim/template ~/.config/nvim
rm -rf ~/.config/nvim/.git
# 首次进入，会安装默认携带的一些插件
nvim 
```

至此，完成AstroNvim的安装&初始化，你已经可以开始使用了。

---

# 心得分享

到此，官网的内容也就暂时没有看过了，我也没有先查询相关的快捷键，而是以先入为主，在使用中，动态的查询自己所需要的快捷键，同时不断记录对应的快捷键笔记，你也可以钉一个文本窗口在屏幕上，用于记录&查询。

而一些对应的快捷键，往往需要相关嗯NVim插件和配置文件支持，为了维护自己自定义的功能，可以针对NVim配置创建一个Git仓库，添加一个功能，就做一笔提交。这样可以追溯自己的修改与自定义内容。