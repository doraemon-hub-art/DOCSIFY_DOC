>  一种类似于Json/XML的配置文件格式。

---

> 示例

```bash
# 设备身份
[device]
thing_name = "tempSensor001"
client_id  = "tempSensor001"

# 证书路径（相对或绝对）
[certs]
ca_file   = "certs/AmazonRootCA1.pem"
cert_file = "certs/device.crt"
```

---

> 相比Json，XML，Yaml有什么好处 ？

简洁，没有过多的缩进和复杂的结构。