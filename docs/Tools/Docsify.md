> 本站由Docsify + Github Page搭建，在此记录一些搭建过程中的内容。

---

# 添加Gittalk评论系统

> 官方文档

- [GITTALK-一个基于 Github Issue 和 Preact 开发的评论插件](https://gitalk.github.io/)

---

> 基本配置

- [官方文档](https://github.com/gitalk/gitalk/blob/master/readme-cn.md)
- [【网站搭建】Docsify+Gittalk的配置过程记录分享。原创！](https://www.cnblogs.com/zwj/p/docsify_gittalk.html)
- [Docsify+GithubPages建立个人知识Wiki及Gitalk踩坑指南](https://juejin.cn/post/7210386831451176997)
- [Docsify的评论系统gitalk配置过程 - 林明生一的文章 - 知乎](https://zhuanlan.zhihu.com/p/652938905)

注意，加载gittalk配置(gitalk.render)需要在Docsify完成渲染之后。如若不然，评论框会丢失或渲染在错误位置。

可以通过Docsify官方提供的钩子函数，来在页面渲染完成之后，加载评论组件。

```html
    plugins: [
        function (hook, vm) {
            // Docsfiy官方提供的钩子函数，代表页面渲染完毕。
            // 开始加载评论组件
            hook.doneEach(function() {
            // 清理旧的评论容器
            const old = document.getElementById('gitalk-container')
            if (old) old.remove()

            // 新建容器并插入页面底部
            const container = document.createElement('div')
            container.id = 'gitalk-container'
            container.style = 'margin-top: 40px;'
            const main = document.querySelector('.content')
            if (main) main.appendChild(container)

            // 渲染 Gitalk
            const gitalk = new Gitalk({
                clientID: 'xx',
                clientSecret: 'xx',
                repo: 'xx',  // 存放评论的仓库
                owner: 'xx',  // 仓库的创建者，
                admin: ['xx'], // 如果仓库有多个人可以操作，那么在这里以数组形式写出
                title: `Document comment ${location.hash.match(/#(.*?)([?]|$)/)[1]}`,
                id: location.hash.match(/#(.*?)([?]|$)/)[1],  // 可以取到正常的pathname，也可以作为页面的唯一标识
                distractionFreeMode: false  // Facebook-like distraction free mode
            })
            gitalk.render('gitalk-container')
            })
        }
    ]
```

---

> 安全问题？

直接暴露clientSecret看起来也不会有啥安全问题。 —— [Bug？暴露 Client_id和Client_secret](https://github.com/gitalk/gitalk/issues/150)

![](https://oss.banshengua.top//blogimages/20251019162206430.png)

---

> 看到了一句很好的话

原文——[Docsify+GithubPages建立个人知识Wiki及Gitalk踩坑指南](https://juejin.cn/post/7210386831451176997)

![](https://oss.banshengua.top//blogimages/20251019160817129.png)

---

# 内嵌抖音视频

TODO:  https://dujun.io/embed-a-douyin-video-in-an-iframe-player.html