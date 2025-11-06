> CMake 相关

---

TODO: 好好梳理下

---

TODO: 系统学习


---

# 随笔

> add_custom_target(xx   ALL)

只要加了all,每次make都会执行这个，无论是否显式make xx。

---

> 引入.cmake文件，将配置项、等抽出来

```bash
# 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(MyProject)

include(${CMAKE_CURRENT_SOURCE_DIR}/find_submodule.cmake)
message(STATUS "TOP: Submodule path is ${SUBMODULE_PATH}")
```

---

> 