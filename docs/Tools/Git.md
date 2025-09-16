> 记录一些常用的Git命令作为补充 

---

> 存储本地登录凭证 token

```bash
git config --global credential.helper store
```

---

> 将对应仓库内容，拉到指定本地文件夹内

- 不指定本地文件夹直接git clone xx，会用远程仓库的名字创建一个文件夹，然后将远程内容拉进去；

```bash
# 例如: 我要将我远程的lvim配置，拉到另外一个电脑的lvim配置目录下
# git clone url.git target_path
git clone https://github.com/doraemon-hub-art/MY_LUNARVIM_CONFIG.git ~/.config/lvim
```



