Home Assistant 的初步上手。

TODO

当手里的米家设备越来越多......

接入DeepSeak......

# 创建容器

```bash
services:
  homeassistant:
    container_name: homeassistant
    image: "ghcr.io/home-assistant/home-assistant:stable"
    volumes:
      - ./config:/config
      - /etc/localtime:/etc/localtime:ro
    restart: unless-stopped
    privileged: true
    network_mode: host
```

```bash
docker compose up -d
```

8123端口，进入可视化页面。

```
http://192.168.1.10:8123
···

---

# 插件安装

```bash
docker exec -it homeassistant bash
wget -O - https://get.hacs.xyz | bash -
```

---