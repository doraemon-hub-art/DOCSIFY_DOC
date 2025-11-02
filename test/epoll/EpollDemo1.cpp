#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <cstring>

int main() {
    // 1. 创建 epoll 实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return 1;
    }

    // 2. 添加 stdin (fd=0)
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl: stdin");
        return 1;
    }

    // 3. 创建 timerfd (每隔3秒触发)
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd == -1) {
        perror("timerfd_create");
        return 1;
    }

    itimerspec timerSpec{};
    timerSpec.it_interval.tv_sec = 3;  // 周期 3 秒
    timerSpec.it_value.tv_sec = 3;     // 首次 3 秒后触发
    if (timerfd_settime(tfd, 0, &timerSpec, nullptr) == -1) {
        perror("timerfd_settime");
        return 1;
    }

    ev.events = EPOLLIN; // 关心可读事件
    ev.data.fd = tfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, tfd, &ev) == -1) {
        perror("epoll_ctl: timerfd");
        return 1;
    }

    std::cout << "Epoll demo started. Type something or wait for timer..." << std::endl;

    // 4. epoll 循环
    while (true) {
        // 10 不是固定值，指定每次取出来的就绪事件个数
        epoll_event events[10];
        int nfds = epoll_wait(epfd, events, 10, -1); // 阻塞等待
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buf[256];
                int n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if (n > 0) {
                    buf[n] = '\0';
                    std::cout << "[stdin] You typed: " << buf;
                }
            } else if (events[i].data.fd == tfd) { // timerfd就绪
                uint64_t expirations;
                read(tfd, &expirations, sizeof(expirations)); // 必须读掉
                std::cout << "[timer] Timer ticked! (" << expirations << " times)" << std::endl;
            }
        }
    }

    close(tfd);
    close(epfd);
    return 0;
}
