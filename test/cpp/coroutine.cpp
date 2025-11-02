#include <coroutine>
#include <iostream>
#include <optional>

template<typename T>
struct Generator {
    struct promise_type {
        T current_value;
        auto get_return_object() { return Generator{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::exit(1); }
    };

    std::coroutine_handle<promise_type> handle;
    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }

    std::optional<T> next() {
        if (!handle.done()) {
            handle.resume();
            return handle.promise().current_value;
        }
        return std::nullopt;
    }
};

Generator<int> counter(int n) {
    for (int i = 1; i <= n; i++) {
        co_yield i;
    }
}

int main() {
    auto gen = counter(5);
    while (auto v = gen.next()) {
        std::cout << *v << "\n";  // 输出 1 2 3 4 5
    }
    std::cout << "test test" << std::endl;
    return 0;
}
