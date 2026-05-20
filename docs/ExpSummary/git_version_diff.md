> 最近写了一个基于Git Tag的版本差异工具，一些感想。


在不做严格的发版分支限制下，接收branch，commit id打tag，会导致tag diff查分异常。

但是结合实际场景，也不得不如此。

在已经形成的发版流程下，限定分支不现实，只能优化commit merge流程。



TODO: git tag版本变更游离问题



> 当checkout 到一个 commit上，这个git一下的其他内容，是如何定位到的？

你有什么更好的想法吗？



