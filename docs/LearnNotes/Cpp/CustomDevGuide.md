> 记录常用的一套C++开发工具和规范

> 代码风格 

- Google C++ Code Style；

> 格式化工具

- Clang-Format；

> 注释&文档；

- Doxygen JavaDoc；

---

# 习惯和注意事项

> #ifdef 内部尽量不要包含 {} 块边界

- 防止块代码包裹错误，例如:

```C++
if (xx) {

#ifdef XX
  printf("xx");
} // 这里的包裹错误
#endif

```

---
