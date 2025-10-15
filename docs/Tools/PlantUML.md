> 使用drow io等UI拖动即可画图的工具固然方便上手，但是我觉得为了长久可维护，还是要使用代码来维护。

> [PlantUML在线预览](www.plantuml.com/plantuml)

> [PlantUML中文文档](https://plantuml.com/zh/)

> [Ubuntu PlantUML VsCode配置]

- https://blog.csdn.net/yinhezhanshen/article/details/147940372
- https://blog.csdn.net/weixin_41111116/article/details/129582436

`快速配置`

```bash
sudo apt-get install openjdk-8-jdk
sudo apt-get install graphviz

# VSCode 安装插件 PlantUML
``` 

> 补充: MarkDown增强渲染PlantUML 

- "Ctrl + ," 打开设置 搜索 markdown-preview-enhanced.plantumlJarPath， 配置路径

- 例如:

```bash
# download rom github.com/plantuml/plantuml/releases/

/home/xuan/workspace/package/plantuml/plantuml-1.2025.8.jar
```

> tips

- 避免大小写切换，可以只用英文小写；
- 自动标记每一步的序号 autonumber；
- 注释，在交互行下 note left(riht): some info；
- 消息注释/换行，消息中添加 \n ；
- 
> test case

文件扩展名 .puml

```plantuml
@startuml
Alice -> Bob: Authentication Request
Bob --> Alice: Authentication Response

Alice -> Bob: Another authentication Request
Alice <-- Bob: another authentication Response
@enduml
```