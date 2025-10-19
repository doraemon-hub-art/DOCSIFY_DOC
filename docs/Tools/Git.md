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

---

> win10大小写不敏感导致的冲突问题

```bash
# 告诉git区分大小写
git config core.ignorecase false  
```

- 但这样只是解决了git的冲突，但是win10文件系统，还是需要手动开启目录级的大小写敏感；

---

# 概念补充

## 常见的代码提交平台

> GitHub

全球最流行的代码托管平台，支持Git版本控制。广泛用于开源项目和团队合作。

> GitLab

提供类似GitHub的功能，但强调企业级的私有部署。GitLab还集成了CI/CD、监控和其他DevOps功能。

> Bitbucket

主要用于私有仓库，支持Git和Mercurial（虽然Mercurial已经逐步停止支持）。在团队开发中较为常用。

---

## PR和MR的区别

> 术语差异: 

PR 和 MR 只是不同平台的不同命名，功能上没有本质区别。

> 平台差异:

- Bitbucket 使用 MR（Merge Request）；
- GitLab 和 GitHub 使用 PR（Pull Request）。
