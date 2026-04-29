---

marp: true

---

# 引用

本文的最终参考 —— 
- https://docs.astronvim.com/

---

# 前言

(Hello 大家好，今天我准备开一个新的系列，从视频中你可以看到 —— AstroNeoVim，一个有关Vim的使用分享。)

经过反复的放弃，然后尝试，我终于下定决心要学习并使用Vim，这指的并不是学习简单的Vim指令，而是将终端打造成一个IDE，以替换掉平时开发工作中使用的VSCode，更多的使用键盘，减少鼠标键盘之间的切换，感觉这样用起来会比较爽。

从NeoVim，到LunarVim，再到AstroNVim，我想，这次我一定可以坚持下来。

这个系列视频，将作为我在Vim使用上的成长记录，同时，我也会不断分享自己在Vim上的一些使用技巧与心得。

希望可以对你有一些帮助。

---

本次我们将选择AstroNVim作为Vim的基础环境，不过实际上他也只在NeoVim上的一层配置框架，实际底层上还是NeoVim。

AstroNvim 即，Astro Neo Vim译为如星辰璀璨的Vim。

AstroNvim 是一套“开箱即用”但“高度可定制”的 Neovim 框架。

NeoVim是Vim的现代进化版，它有着很多现代的插件，AstroNVim，是NeoVim精装修版，直接在NeoVim的基础上，预装了很多插件和设置，真正意义上的可以开箱即用。

作为本系列的第一期视频，我会简单介绍其安装与环境配置，同时分享一些我在初步接触后，使用上的一些小技巧。

我们以先用为主，下一期视频呢，我会进一步分享它的核心配置与相关内容解读。
（其实是这期懒了，发现扩展的越来越多：）

---

# 环境配置

> 参考官方Requirement

https://docs.astronvim.com/#-requirements

1. 安装终端字体[Mono](https://www.nerdfonts.com/font-downloads)；
2. 安装Neovim 0.10 +;

```bash
# 下载.appimage ；
https://github.com/neovim/neovim/releases/tag/stable 
# 重命名awd
mv nvim-linux-x86_64.appimage nvim
# 权限&全局安装
chmod +x ./nvim
mv ./nvim /usr/local/bin
nvim --version
```

---

3. 安装 astronvim

(实际上就是在Neovim基础上，导入AstroNVimd的自定义配置)

```bash
git clone --depth 1 https://github.com/AstroNvim/template ~/.config/nvim
rm -rf ~/.config/nvim/.git
# 首次进入，会安装默认携带的一些插件
nvim 
```

至此，完成AstroNvim的安装&初始化，你已经可以开始使用了。

---




---

# 插件与配置

社区为我们提供了很多插件，来让NVim支持更多功能。

AstroNVim是在NeoVim上的封装，对于相关插件的引入也有着两种方式。

---


---

## 以原生NeoVim的方式引入

TODO: 起到一个对比的作用，所有我们这里先讲一下NeoVim原本的引入方式。

我们以一些复杂的插件为例，即需要很多配置。

使用 NeoVim的方式引入，所有的配置都需要你自己填写，如果少些某个配置，插件可能起不来，相反，所有的配置都由你掌握，自由度最高。

而配置简单的插件，区别就不是很大啦。

---

##  从AstroNVim社区引入（优先）

AstroNVim社区中提供了很多直接插件，我们可以直接拿过来用，里面为我们提供了一些默认配置。

如果你想知道 astrocommunity 默认帮你写了哪些配置，可以去 GitHub 上看它源码里的 init.lua

点击标题即可跳转至github仓库。

![](https://oss.banshengua.top/blogimages/202604271941059.png)

虽然类似于原生NeoVim的引入方式，都需要在lua/plugins下新建一个lua文件，但是作用是不同的。

---

使用 AstroNvim的方式引用，你不需要所有内容都配置，它会采用“合并” 的方式，将你自定义的内容，覆盖到原本的默认配置中。如果你不需要或者有的插件不需要怎么配置，直接在 community.lua  中一行 import就搞定了。

https://github.com/AstroNvim/astrocommunity

https://astronvim.github.io/astrocommunity/

注意: 如果提示找不到某个社区的插件，记得更新一下本地的astrocommunity插件库。

![](https://oss.banshengua.top/blogimages/202604271806124.png)

输入 Lazy，之后U，等待更新完成，重新打开nvim。

或者直接输入 Lazy update。

---

## 快捷键

astrocore.lua 是用来配置 Neovim 本身和 AstroNvim 框架的，而 lua/plugins/ 下的其他文件是用来配置具体功能的。

即，全局快捷键，在astrocore.lua中修改，针对某个插件的快捷键，放到对应的lua中。

当然，你也可以集中到一起，放到astrocore.lua的mappings中统一管理。

或者随便写一个lua，放到plugin目录下，让AstroNvim检测到，集中到里面管理，不过这样比较麻烦：）

当习惯一些键位之后，引入新的插件，使用Leader后，根据提示一步一步引导，会更加熟练。

---

# 辅助工具

简单的工具分享。

> Termal

- Kitty —— https://sw.kovidgoyal.net/kitty/
- Tabby —— https://tabby.sh/

---

# 简单演示

(这里做一下简单的示范。)

---

# 心得分享

到此，官网的内容也就暂时没有看过了，我也没有先查询相关的快捷键，而是以先入为主，在使用中，借助AI，动态的查询自己所需要的快捷键，同时不断记录对应的快捷键笔记，你也可以钉一个文本窗口在屏幕上，用于记录&查询。

而一些对应的快捷键，往往需要相关Neovim插件和配置文件支持，为了维护自己自定义的功能，可以针对NeoVim配置创建一个Git仓库，添加一个功能，就做一笔提交。这样可以追溯自己的修改与自定义内容。

如果发现一个组合键不可用了，可以更换终端验证下，有可能是跟终端快捷键冲突了。

很重要的一点，为了更好的切换模式和取消某些操作，把没啥用的大写锁定键改成ESC，会更加舒服。不过开始还是需要适应一下。

以及，当引入AI插件之后，正式由于我的Kitty终端无法输入中文，我才意识到，尽量避免切换中文输入法，进一步提高效率，用蹩脚的英文，AI 也能听懂。这个我们放到下期细说。

至此我已上手并使用快四周了，一些基础的快捷键也算是得心应手，可以用作完成一些编码工作，同时我也在进一步地研究它。

---

# 更进一步

在后面的内容中，我会进一步研究AstroNVim的配置与使用新的，并与你分享。

原本的第一期内容，被扩充了很多。

我们，下期再见吧。

---