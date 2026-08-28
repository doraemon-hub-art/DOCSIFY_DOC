> FRP 反向代理

---

# Reference

- https://gofrp.org/zh-cn/

---

# 跳板机配置

1. 下载解压；

```bash
wget https://github.com/fatedier/frp/releases/download/v0.54.0/frp_0.54.0_linux_amd64.tar.gz
tar -zxvf frp_0.54.0_linux_amd64.tar.gz
cd frp_0.54.0_linux_amd64
```

2. 编辑frps.toml；

```bash
# frps.toml
bindPort = 7000           # frp 通讯端口，不要和博客端口冲突
auth.token = "你的安全密码"
```

3. 端口开放；

如果是云服务器，配置入栈规则，放开两个端口:

- 转发端口，也就是上面配置的bindPort;
- 访问端口，通过跳板机访问目标机器的端口,即下方frpc.toml中的remotePort；

4. 子域名配置；

- 添加解析，一条A记录；

5. systemd后台运行；

```bash
# 编辑
sudo nano /etc/systemd/system/frps.service

# 修改
[Unit]
Description=frp server
After=network.target

[Service]
Type=simple
# 改成你解压 frp 的实际绝对路径
ExecStart=/root/frp_0.54.0_linux_amd64/frps -c /root/frp_0.54.0_linux_amd64/frps.toml
Restart=on-failure

[Install]
WantedBy=multi-user.target

# 加载配置
sudo systemctl daemon-reload

# 启动并设置开机自启
sudo systemctl enable frps
sudo systemctl start frps

# 检查运行状态
sudo systemctl status frps

# 实时日志
journalctl -u frps -f
```

---

# 目标机配置

1. 下载解压；

```bash
wget https://github.com/fatedier/frp/releases/download/v0.54.0/frp_0.54.0_linux_amd64.tar.gz
tar -zxvf frp_0.54.0_linux_amd64.tar.gz
cd frp_0.54.0_linux_amd64
```

2. 编辑frpc.toml；

```bash
# frpc.toml
serverAddr = "ssh.yourdomain.com" # 或者直接写主域名/IP
serverPort = 7000                 # 对应 frps 的 bindPort
auth.token = "你的安全密码"
# 添加下面这一行，防止第一次连接失败就退出
loginFailExit = false

[[proxies]]
name = "ssh-proxy"
type = "tcp"
localIP = "127.0.0.1"
localPort = 22
remotePort = 6000                 # 这是你在外网连接时要用的端口
```

3. systemd后台运行；

```bash
# 编辑
sudo nano /etc/systemd/system/frpc.service

# 修改
[Unit]
Description=frp client
# 尽量等待网络就绪
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
# 改成你解压 frp 的实际绝对路径
ExecStart=/home/yourname/frp_0.54.0_linux_amd64/frpc -c /home/yourname/frp_0.54.0_linux_amd64/frpc.toml
Restart=on-failure
# 每次失败后等待 15 秒再重启，给网络留出充足时间
RestartSec=15s
# 取消启动频率限制
StartLimitIntervalSec=0

[Install]
WantedBy=multi-user.target



# 加载配置
sudo systemctl daemon-reload

# 启动并设置开机自启
sudo systemctl enable frpc
sudo systemctl start frpc

# 检查运行状态
sudo systemctl status frpc

# 实时日志
journalctl -u frpc -f
```

---

# 愉快的使用

Bios开启通电启动，可以配合米家智能开关，进行远程开机，再通过反向代理连接。

```bash
# 注意p小写
ssh -p 6000 内网用户名@ssh.yourdomain.com
```

注意: 跳板机、目标机，都需要开启并运行SSH服务。

---

# 问题参考

## ssh访问卡顿

> 排查所处网络环境问题

根因链路：
    
```bash
SSH 请求 → DNS 解析域名
          ↓
resolvectl → 192.168.1.250（旁路由）
          ↓
旁路由 Clash 拦截 → 返回 fake-ip（198.18.x.x）
          ↓
SSH 连 fake-ip → 流量走 tun0 代理 → 多跳转发 → 延迟高+不稳定
```

两条 DNS 路径都断了：
- /etc/resolv.conf 是普通文件写死 114.114.114.114 → glibc 拿 fake-ip
- cscotun0 (Cisco VPN) 挂着 DNS Domain: ~. → 所有域名走 VPN DNS → 也是 fake-ip

修复（三件事）：
1. sudo ln -sf /run/systemd/resolve/stub-resolv.conf /etc/resolv.conf → glibc 走 systemd-resolved
2. sudo resolvectl domain cscotun0 "" → 去掉 VPN DNS 劫持（VPN 断了所以这步没实际跑成）
3. /etc/hosts 写死 → 暴力绕过所有 DNS 层面

最终效果：SSH 直接直连真实 IP，不走代理不绕路，不卡了。

---

Thanks.