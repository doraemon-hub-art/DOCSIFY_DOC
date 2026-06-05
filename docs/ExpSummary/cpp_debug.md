> 一些调试方法，看具体情况是否整合到其他为止

> 看进程加载的具体SO路径

```bash
/proc/<pid>/maps
```

> 加载特定路径的so

```bash
LD_PRELOAD=xx.so ./test_bin
```