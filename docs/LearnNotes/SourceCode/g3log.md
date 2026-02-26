不光是做分享，其实也是我学习的过程。

> 一个开源的异步日志框架

对于这样代码行数相对较少，功能相对明确的开源代码，我们可以先运行起来，对照运行时LOG，做一个快速上手。

> 先跑起来

- 编译运行 main_contract；
- 对照文件查看基本的api调用；

```bash
./g3log/example/main_contract.cpp
```


# 问题记录

记录学习的过程，之后需要进行梳理，讲的时候直接对着源码跳转讲解。
这里的记录内容可以做文章输出。

- 一个宏的调用，底层发生了什么？
  - 这个宏替换过去，这里是对哪个对象做 << 操作？

```C++
LOG(INFO) << "Test SLOG INFO";
|
|
|
v
#define LOG(level) if (!g3::logLevel(level)) {} else INTERNAL_LOG_MESSAGE(level).stream()
|
|
|
v
#define INTERNAL_LOG_MESSAGE(level) LogCapture(__FILE__, __LINE__, static_cast<const char*>(G3LOG_PRETTY_FUNCTION), level)
|
|
|
v
std::ostringstream _stream;
```

实际上是对 ostringstream 这个流对象做 << 操作。

<< 为 流插入运算符。

- 如何写入的？
  - 光将数据写入流对象，之后就没别的操作了？

```C++
LogCapture::~LogCapture() noexcept(false) {
   using namespace g3::internal;
   SIGNAL_HANDLER_VERIFY();
   saveMessage(_stream.str().c_str(), _file, _line, _function, _level, _expression, _fatal_signal, _stack_trace.c_str());
}
```

在析构函数中实现，这种自杀式的逻辑，有点意思。

- g3::internal::saveMessage实现

```C++
void saveMessage(const char* entry, const char* file, int line, const char* function, const LEVELS& level,
                  const char* boolean_expression, int fatal_signal, const char* stack_trace) {
    LEVELS msgLevel{level};
    LogMessagePtr message{std::make_unique<LogMessage>(file, line, function, msgLevel)};
    message.get()->write().append(entry);
    message.get()->setExpression(boolean_expression);

    if (internal::wasFatal(level)) {
      saveFatalMessage(stack_trace, message, fatal_signal);
    } else {
      pushMessageToLogger(message);
    }
}
```

- 可以发现，我们已经慢慢讲源码串起来了。

```C++


```

这里会构造一个 LogMessage 对象。

- 同时我也想知道，日志中打印时间戳的时间来源是哪里？

```C++
   LogMessage::LogMessage(std::string file, const int line,
                          std::string function, const LEVELS level) :
       _logDetailsToStringFunc(LogMessage::DefaultLogDetailsToString),
       _timestamp(std::chrono::high_resolution_clock::now()),
```

C++高精度时间戳，时间来源为系统软件时间。



