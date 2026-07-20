# 两种简单的生产者消费者模型

从消费者获取数据的方式这个维度来抽象，确实就两类：
    
| 方式         | 代表                  |
|--------------|-----------------------|
| 推送（push） | CV、回调、信号槽      |
| 拉取（pull） | Tick 轮询、定时器扫描 |

---

## 模型一：1:1 事件驱动（CV）

一个状态一个专属线程，状态变化时通过条件变量唤醒线程。

```
生产者（外部事件）                消费者（专属线程）
       │                              │
       │  setState(s)                  │
       │  cv.notify()                  │
       ├─────────────────────────────► │
       │                              │  cv.wait(lock)
       │                              │  唤醒 → 读 state → 执行动作
```

```cpp
class Controller {
    State state_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::thread worker_;
    bool alive_{true};

    void SetState(State s) {
        std::lock_guard lk(mtx_);
        state_ = s;
        cv_.notify_one();
    }

    void Worker() {
        while (alive_) {
            State snapshot;
            {
                std::unique_lock lk(mtx_);
                cv_.wait(lk, [&] {
                    return state_ != snapshot || !alive_;
                });
                snapshot = state_;
            }
            switch (snapshot) {
                case IDLE:   OnIdle();   break;
                case ERROR:  OnError();  break;
                case ACTIVE: OnWork();   break;
            }
        }
    }
};
```

| 角色 | 说明 |
|------|------|
| 生产者 | 外部调 SetState + notify |
| 消费者 | Worker 线程，wait 在 cv 上 |
| 通信 | condition_variable（推送） |
| 关系 | 1 状态机 : 1 线程 |

**适用**：连接状态监控、信令处理等毫秒级响应的单一状态。

**优缺点**：响应快；N 个状态需 N 个线程，扩展差。

---

## 模型二：N:1 轮询驱动（Tick）

多个任务共用一个工作循环，每个任务暴露 Tick() 返回需要执行的动作。

```
生产者                             消费者（共享循环）
   │                                  │
   │  Init() / OnResult()             │
   │                                  │  for each task:
   │                                  │    switch task.Tick():
   │                                  │      START  → do action
   │                                  │      RETRY  → do action
   │                                  │      DONE   → cleanup
   │                                  │      EXPIRED→ give up
   │                                  │      NONE   → skip
   └──────── 共享任务池 ──────────────┘
```

```cpp
enum class Action { NONE, START, RETRY, DONE, EXPIRED };

class Task {
public:
    Action Tick() {
        auto now = Clock::now();
        switch (phase_) {
            case IDLE:
                last_try_ = now;
                phase_ = BUSY;
                return Action::START;
            case BUSY:
                if (now - last_try_ <= interval_)
                    return Action::NONE;
                if (++retries_ > max_retries_) {
                    phase_ = EXPIRED;
                    return Action::EXPIRED;
                }
                last_try_ = now;
                return Action::RETRY;
            case SUCCESS:
            case FAILED:
                phase_ = UNINIT;
                return Action::DONE;
            case EXPIRED:
                return Action::EXPIRED;
            default:
                return Action::NONE;
        }
    }

    void OnResult(bool ok) {
        phase_ = ok ? SUCCESS : FAILED;
    }

    void Init(Args...) { phase_ = IDLE; }

private:
    Phase phase_{UNINIT};
    int retries_{0};
    int max_retries_{0};
    int interval_ms_{0};
    TimePoint last_try_{0};
};

// 外部共享循环
while (running) {
    sleep(30s);
    for (auto& task : tasks) {
        switch (task.Tick()) {
            case START:
            case RETRY:
                Execute(task.GetPath());
                break;
            case DONE:    Cleanup(task);  break;
            case EXPIRED: Remove(task);   break;
            case NONE:    break;
        }
    }
}
```

| 角色 | 说明 |
|------|------|
| 生产者 | Init 放入任务，OnResult 汇报结果 |
| 消费者 | 共享循环，固定间隔扫一次 |
| 通信 | Tick() 返回值（拉取） |
| 关系 | N 个 Task : 1 个 Loop |

**适用**：批量上传重试、定时任务等低频管理。

**优缺点**：一个循环管所有；实时性取决于轮询间隔（秒级）。

---

## 对比

| 维度 | 事件驱动（CV） | 轮询驱动（Tick） |
|------|---------------|-----------------|
| 关系 | 1 状态 : 1 线程 | N 任务 : 1 循环 |
| 消费者阻塞 | 是，wait 在 cv 上 | 否，sleep + 轮询 |
| 实时性 | 高（毫秒级） | 低（秒级） |
| 扩展 N 个 | 需要 N 个线程 | 一个循环管全部 |
| 复杂度 | 高（线程 + cv + 锁） | 低（纯函数调用） |
| 状态机持线程 | 是 | 否，纯状态寄存器 |

**一句话选型**：状态少且实时性敏感 → 事件驱动；任务多且低频 → 轮询驱动。

---

# 总结

在大AI时代，这种更高阶的思维，或者说意识，是更值得我们提升。

简单来说就是，根据公式实现问题的解已经不是问题，而更重要的是选择哪个公式，值得我们去思考，只有见过、了解过，才知道还有什么方法。

回看我这几天经历的折腾、别扭和反复，本质上就是因为一开始“选错了公式”，一开始就使用错误的公式，跟着AI反复讨论修改，迟迟得不到我想要的解，就是因为，我没接触过，没有在一开始就定准方向。

但，悲观的来讲，这种决策AI也能做。

---