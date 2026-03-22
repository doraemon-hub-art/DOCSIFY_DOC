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

