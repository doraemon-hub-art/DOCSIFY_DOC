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

# CORS 跨域问题

> 浏览器规定，网页中的 JavaScript 只能访问与当前页面同源的资源。

- “同源”指 **协议、域名、端口**三者完全一致。

当网页试图通过 fetch / XMLHttpRequest 等方式,访问 不同源的 HTTP 接口 时，
浏览器会阻止 JavaScript 读取响应结果，这就被称为“跨域问题”。

> 为了解决这个限制，服务器可以通过返回CORS响应头，来显式声明允许哪些来源、方法和头部跨域访问。

---


