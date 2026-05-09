/**
 * hid_probe2.c - 高级 HID 设备探测工具
 * 
 * 尝试通过多种方式与设备通信，获取原始数据
 * 
 * 编译: gcc -o hid_probe2 hid_probe2.c
 * 运行: sudo ./hid_probe2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <poll.h>
#include <signal.h>

#define MAX_REPORT_SIZE 1024
#define TIMEOUT_MS 5000

static volatile int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

/**
 * 尝试通过 HID 功能报告 (Feature Report) 获取设备信息
 */
void get_feature_reports(int fd, const char *label)
{
    uint8_t buf[MAX_REPORT_SIZE];
    int ret;
    
    printf("\n--- 尝试获取 Feature Report ---\n");
    
    /* 尝试获取报告 ID 0x02 的 Feature Report */
    for (int report_id = 0x01; report_id <= 0x06; report_id++) {
        memset(buf, 0, sizeof(buf));
        buf[0] = report_id;
        ret = ioctl(fd, HIDIOCGFEATURE(64), buf);
        if (ret >= 0) {
            printf("  Feature Report ID=0x%02X (%d bytes): ", report_id, ret);
            for (int i = 0; i < ret && i < 16; i++)
                printf("%02X ", buf[i]);
            printf("\n");
        } else {
            // printf("  Feature Report ID=0x%02X: %s\n", report_id, strerror(errno));
        }
    }
    
    /* 尝试获取报告 ID 0x3F, 0x44 的 Feature Report */
    uint8_t vendor_ids[] = {0x3F, 0x44, 0xBC, 0xBD, 0xBE, 0xBF};
    for (int i = 0; i < sizeof(vendor_ids); i++) {
        memset(buf, 0, sizeof(buf));
        buf[0] = vendor_ids[i];
        ret = ioctl(fd, HIDIOCGFEATURE(256), buf);
        if (ret >= 0) {
            printf("  Feature Report ID=0x%02X (%d bytes): ", vendor_ids[i], ret);
            for (int j = 0; j < ret && j < 32; j++)
                printf("%02X ", buf[j]);
            printf("\n");
        }
    }
}

/**
 * 尝试发送 Output Report 来初始化设备
 */
void send_output_reports(int fd, const char *label)
{
    uint8_t buf[MAX_REPORT_SIZE];
    int ret;
    
    printf("\n--- 尝试发送 Output Report ---\n");
    
    /* 尝试发送一些常见的初始化命令 */
    struct {
        uint8_t id;
        uint8_t data[16];
        int len;
    } init_cmds[] = {
        {0x02, {0x00}, 1},           /* 报告 ID 0x02 */
        {0x01, {0x00}, 1},           /* 报告 ID 0x01 */
        {0xBC, {0x00}, 1},           /* 厂商自定义 */
        {0xBD, {0x00}, 1},
        {0xBE, {0x00}, 1},
        {0xBF, {0x00}, 1},
    };
    
    for (int i = 0; i < sizeof(init_cmds)/sizeof(init_cmds[0]); i++) {
        memset(buf, 0, sizeof(buf));
        buf[0] = init_cmds[i].id;
        memcpy(buf + 1, init_cmds[i].data, init_cmds[i].len);
        
        ret = write(fd, buf, init_cmds[i].len + 1);
        if (ret >= 0) {
            printf("  Output Report ID=0x%02X 发送成功 (%d bytes)\n", 
                   init_cmds[i].id, ret);
        } else {
            // printf("  Output Report ID=0x%02X: %s\n", 
            //        init_cmds[i].id, strerror(errno));
        }
    }
}

/**
 * 读取原始 HID 数据 (阻塞模式)
 */
void read_hid_data(int fd, const char *label, int timeout_ms)
{
    uint8_t buf[MAX_REPORT_SIZE];
    struct pollfd fds;
    int ret;
    int count = 0;
    
    printf("\n--- 读取数据 (%s) ---\n", label);
    printf("等待数据... (请在设备上操作)\n");
    
    fds.fd = fd;
    fds.events = POLLIN;
    
    while (keep_running && count < 20) {
        ret = poll(&fds, 1, timeout_ms);
        
        if (ret > 0 && (fds.revents & POLLIN)) {
            memset(buf, 0, sizeof(buf));
            ret = read(fd, buf, sizeof(buf));
            
            if (ret > 0) {
                count++;
                printf("[%s] 收到 %d 字节: ", label, ret);
                for (int i = 0; i < ret && i < 64; i++)
                    printf("%02X ", buf[i]);
                if (ret > 64) printf("...");
                printf("\n");
                
                /* 解析报告 ID */
                if (ret >= 1) {
                    printf("  报告 ID: 0x%02X\n", buf[0]);
                }
            }
        } else if (ret == 0) {
            printf("[%s] 超时, 无数据\n", label);
            break;
        } else if (ret < 0) {
            if (errno != EINTR)
                printf("[%s] poll 错误: %s\n", label, strerror(errno));
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int fd;
    struct hidraw_devinfo info;
    
    signal(SIGINT, int_handler);
    
    printf("========================================\n");
    printf("  HID 设备高级探测工具\n");
    printf("========================================\n\n");
    
    /* 探测两个接口 */
    const char *paths[] = {"/dev/hidraw2", "/dev/hidraw3", NULL};
    const char *labels[] = {"Interface 0 (鼠标)", "Interface 1 (触摸)"};
    
    for (int i = 0; paths[i] != NULL; i++) {
        printf("\n========================================\n");
        printf("  打开 %s - %s\n", paths[i], labels[i]);
        printf("========================================\n");
        
        fd = open(paths[i], O_RDWR);
        if (fd < 0) {
            printf("无法打开 %s: %s\n", paths[i], strerror(errno));
            printf("提示: 请使用 sudo 运行\n");
            continue;
        }
        
        /* 获取设备信息 */
        memset(&info, 0, sizeof(info));
        if (ioctl(fd, HIDIOCGRAWINFO, &info) >= 0) {
            printf("  Vendor=0x%04X Product=0x%04X\n", info.vendor, info.product);
        }
        
        /* 1. 尝试获取 Feature Report */
        get_feature_reports(fd, labels[i]);
        
        /* 2. 尝试发送 Output Report 初始化设备 */
        send_output_reports(fd, labels[i]);
        
        /* 3. 等待一下，让设备响应 */
        usleep(100000);
        
        /* 4. 读取数据 */
        read_hid_data(fd, labels[i], TIMEOUT_MS);
        
        close(fd);
    }
    
    printf("\n========================================\n");
    printf("  探测完成\n");
    printf("========================================\n");
    
    return 0;
}
