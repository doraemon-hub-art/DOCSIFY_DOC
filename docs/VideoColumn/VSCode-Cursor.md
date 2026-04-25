> 使用插件Cline插件，接入DeepSeep，实现类似Cursor的效果

> Title 《VSCode AI Agent接入手顺》

在还没部署过的电脑上，可以完整的部署一下。

在尝试AWS Kiro、字节Trace、Cursor这些Ai Agent，用完免费额度后，我考虑要不要充值Cursor，因为它效果还可以，而且充值方便，可以直接支付宝冲到自己号上，不过最低档位20$一个月的额度，总感觉似乎不是很够用，而且也比较贵。

所以我就开始寻找一些便宜的模型token来代替，发现deepseek的api还比较便宜，最低档位可以冲10块，觉得先简单用下。

开始想接入Cursor，但是似乎免费版的Cursor也不支持接入DeepSeek模型，官方写的模型列表中，都搜索不到了，自定义添加，也是提示需要升级到pro，放弃了。

![](https://oss.banshengua.top/blogimages/202604251349234.png)

于是，我在VSCode中下载Cline插件，可以很方便的将其接入。实现很接近于Cursor的效果。

---

astronvim 接入 neovim对应的插件；


---

> 其他用法

我经常用VSCode作为Git可视化，可以让他帮忙在提交前，review下大致的逻辑。

---