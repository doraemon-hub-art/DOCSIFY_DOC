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

> 将指定仓库clone到指定文件夹，不创建文件夹

```bash
git clone <your-repo-url> ~/.config/lvim
```

---

> 仓库主页下载的zip用于作为第三方引入

没有git相关内容，只有源码文件，很小。

---

> 合并多笔提交

```bash
git rebase -i dev
```

> 设置局部文件夹git config配置

```bash
git config user.name "你的名字"
git config user.email "你的邮箱"

# 写入当前仓库下
./git/config
```

执行这个命令后：

- Git 会把所有“属于你这个分支、但不属于 dev”的提交全部列出来;
- 你只需要保留第一行为 pick，剩下的全部改为 s;
- 这样最准确，永远不会出现“多一笔”或“少一笔”的问题;

---

> 丢弃本地提交

- 未暂存的

```bash
git restore .
```

- 删除未被跟踪的

```bash
# 必须在项目根目录
git clean -fd
```

- 去掉某笔提交

```bash
# 需要指定对应提交的上一笔提交
# 而且有时候还需要解决后面的冲突，不断git base --continue
# 删除较远的提交不建议这样
git rebase -i xx
git rebase --continue
# 终止
git base --abort

# =============================
# 推荐使用revert，但是会多一笔revert提交
# 不过用于测试分支，不影响
git revert xx
```

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

---

# Git Tag

可用于发布版本管理。

给重要时间点的代码打标签，方便定位、对比或部署。

(这个以前还真没咋用过，所以单独建立一个小标题来讲一下。)

> 特点

- 不跟随分支走，只是认定某个commit；
- 不指定commit的时候，默认以当前分支的HEAD，即当前分支最新的commit；

> 用法

```bash
# 不显式指定
git tag -a v2.3.1 -m "修复登录Bug，优化性能"
# 等价于
git tag -a v2.3.1 HEAD -m "修复登录Bug，优化性能"

# 指定commit
git tag -a v2.3.0 a1b2c3d -m "补打标签"

# 推送到远程
git push origin --tags 

# 显示差异
# 文件
git diff <tag_name_1> <tag_name_2>
# commit列表
git log <tag_name_1>..<tag_name_2>
```

> 补充

- Git 是基于“图（Graph）”而不是“分支（Branch）”;
- Git 的 git log A..B 命令实际上是在进行集合运算：
  - 计算逻辑：找到所有“能从 B 到达，但不能从 A 到达”的提交;
  - 物理前提：A 和 B 在 Git 的对象数据库中必须有祖先-后代关系;

---

# gitignore

忽略仓库下的某些文件，用于告诉 Git 哪些文件或目录不需要被纳入版本控制。比如构建目录、本地缓存等。

常见场景：

- 编译产物：build/、bin/
- 临时文件：*.tmp
- 日志文件：*.log
- IDE配置：.vscode/、.idea/
- 依赖目录：node_modules/
- 敏感信息：.env
  
> 用法

- 创建.gitingore文件；
- 填入相关类型、目录；

---