/**
 * hid_monitor.c - HID 数据监控工具
 * 
 * 实时打印 HID 设备收到的原始数据，方便观察操作时的数据变化
 * 
 * 编译: gcc -o hid_monitor hid_monitor.c
 * 运行: sudo ./hid_monitor [hidraw设备号，默认3]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <poll.h>
#include <signal.h>
#include <time.h>

static volatile int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

void print_timestamp(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("[%6ld.%03ld]", ts.tv_sec % 1000, ts.tv_nsec / 1000000);
}

void print_report(const uint8_t *buf, int len)
{
    print_timestamp();
    
    /* 报告 ID */
    printf(" ID=0x%02X", buf[0]);
    
    /* 触点数量 (如果适用) */
    if (buf[0] == 0x04 && len >= 2) {
        printf(" 触点数=%d", buf[1] & 0x0F);
    }
    
    /* 按键状态 (如果适用) */
    if ((buf[0] == 0x01 || buf[0] == 0x02) && len >= 2) {
        printf(" 按键=%s%s%s",
               (buf[1] & 0x01) ? "左" : "-",
               (buf[1] & 0x02) ? "右" : "-",
               (buf[1] & 0x04) ? "中" : "-");
    }
    
    /* 解析触点坐标 */
    if (buf[0] == 0x04 && len >= 26) {
        printf(" 触点:[");
        int first = 1;
        for (int i = 0; i < 6; i++) {
            int off = 2 + i * 4;
            if (off + 4 > len) break;
            int x = buf[off] | (buf[off+1] << 8);
            int y = buf[off+2] | (buf[off+3] << 8);
            if (x != 0 || y != 0) {
                if (!first) printf(" ");
                printf("(%d,%d)", x, y);
                first = 0;
            }
        }
        printf("]");
        
        /* 时间戳 */
        uint16_t ts = buf[26] | (buf[27] << 8);
        printf(" TS=%d", ts);
        
        /* 状态 */
        printf(" ST=%02X%02X", buf[29], buf[28]);
    }
    
    /* 标准触摸报告 */
    if (buf[0] == 0x01 && len >= 7) {
        int x = buf[2] | ((buf[3] & 0x0F) << 8);
        int y = ((buf[3] >> 4) & 0x0F) | (buf[4] << 4);
        int8_t wheel = (int8_t)buf[5];
        printf(" X=%d Y=%d 滚轮=%d", x, y, wheel);
    }
    
    /* 打印原始数据 (紧凑格式) */
    printf("  [");
    for (int i = 0; i < len && i < 30; i++) {
        printf("%02X", buf[i]);
        if (i < len - 1 && i < 29) printf(" ");
    }
    if (len > 30) printf(" ...");
    printf("]\n");
}

int main(int argc, char *argv[])
{
    int fd;
    struct pollfd fds;
    uint8_t buf[1024];
    int ret;
    int dev_num = 3;  /* 默认 hidraw3 */
    char path[64];
    
    if (argc > 1) {
        dev_num = atoi(argv[1]);
    }
    
    signal(SIGINT, int_handler);
    
    snprintf(path, sizeof(path), "/dev/hidraw%d", dev_num);
    
    printf("========================================\n");
    printf("  HID 数据监控工具\n");
    printf("  监控: %s\n", path);
    printf("========================================\n\n");
    printf("请在设备上操作, 观察数据变化\n");
    printf("Ctrl+C 退出\n\n");
    
    fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "无法打开 %s: %s\n", path, strerror(errno));
        fprintf(stderr, "请使用 sudo 运行\n");
        return 1;
    }
    
    fds.fd = fd;
    fds.events = POLLIN;
    
    while (keep_running) {
        ret = poll(&fds, 1, 100);
        
        if (ret > 0 && (fds.revents & POLLIN)) {
            memset(buf, 0, sizeof(buf));
            ret = read(fd, buf, sizeof(buf));
            
            if (ret > 0) {
                print_report(buf, ret);
            }
        } else if (ret < 0 && errno != EINTR) {
            perror("poll 错误");
            break;
        }
    }
    
    close(fd);
    printf("\n监控结束\n");
    return 0;
}
