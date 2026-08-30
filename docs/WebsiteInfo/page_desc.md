> 页面描述

# 数据统计

> 数据统计脚本 collect_info.py

统计下方需要显示的所以信息，生成document_info.json

---

# 数据渲染页面

## 全局总览

> README.md

1. md文章总数 除去 README.md _navbar.md _sidebar.md ，以及对应的总字数； 以及去掉WebsiteInfo这里里面的所以md

---

## 文件信息

> over_view.md

根据最后一次的更新时间，排序，列出所以文章，用一个列表的方式显示所以文件，每个文件的信息包括:

- 文件名 —— 用路径的方式显示，例如: AI/Agent/Conctpt.md；
- 总字数；
- 最后一次更新时间 —— 年月日时分秒
- 创建时间 —— 年月日时分秒；

文件信息来自documen_info.json

---