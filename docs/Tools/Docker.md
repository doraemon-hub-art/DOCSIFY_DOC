> docker相关

---

> docker 是什么？

Docker 是一个开源的容器化平台，旨在简化应用程序的开发、交付和部署。

它允许开发人员将应用程序及其所有依赖项打包到一个称为 容器 的标准化单元中，这些容器可以在任何环境中运行，无论是开发环境、测试环境还是生产环境。

---

#
# Ubuntu Docker安装

> 详见 —— [【Docker】Ubuntu Docker安装](https://banshengua.top/archives/zuXP85JB)

主站这篇文章要不要搬过来？

---

# 常用命令

> 删除本地所有容器

```bash
docker rm $(docker ps -aq)
```

---

> 创建本地docker网络

容器名称只对 同一网络中的容器有效。

不同网络中的容器无法直接使用容器名称互相访问。

```bash
docker network create my_custom_network
```

---

> 指定容器网络

- 容器创建时

```bash
docker run --network my_custom_network my_image
```

- 容器创建后

```bash
docker network connect <network_name> <container_name_or_id>
```

---

> 给容器命名

- 容器创建时

```bash
docker run --name my-container -d my-image
```

- 容器创建后

```bash
docker rename my-old-container my-new-container
```

---

> 查看某一网络下的容器

```bash
docker network inspect lambda-network
```

