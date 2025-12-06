> 一些网络的概念知识

---

# DHCP

---

# mDNS

> 干嘛的？

在本地网络里，用名字找人，不暴露真实 IP。

> 怎么用？

在同一张局域网，用 mDNS 去解析，获取真实的局域网IP地址。

> WebRTC 中为啥会出现mDNS(.local)

这是出于 WebRTC 的隐私保护机制（IP Leakage Prevention）。
现代浏览器（Chrome, Edge, Safari）为了防止网站获取用户的局域网真实 IP（例如 192.168.1.5），会把 Local Host Candidate 隐藏在一个随机的 mDNS 域名后面。例: 

candidate:1 1 UDP ... abcdef.local 54321 typ host ...


---


