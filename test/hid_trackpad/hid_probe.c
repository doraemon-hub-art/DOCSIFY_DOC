/**
 * hid_probe.c - 探测 HID 设备的原始数据
 * 
 * 通过 hidraw 读取 Apple Magic Trackpad (05ac:0265) 的原始 HID 报告数据
 * 并解析显示坐标、按键、触点数量等信息
 * 
 * 编译: gcc -o hid_probe hid_probe.c
 * 运行: sudo ./hid_probe
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

/* HID 报告 ID 定义 (基于报告描述符分析) */
#define REPORT_ID_MOUSE       0x02  /* 基础鼠标: 按键 + 相对位移 */
#define REPORT_ID_MTOUCH      0x01  /* 标准触摸: 按键 + 绝对坐标 */
#define REPORT_ID_MULTITOUCH  0x04  /* 多点触摸: 触点数据 */
#define REPORT_ID_TOUCH_STATE 0x02  /* 触摸状态 (Interface 1) */
#define REPORT_ID_VENDOR_3F   0x3F  /* 厂商自定义 16字节 */
#define REPORT_ID_VENDOR_44   0x44  /* 厂商自定义 大块数据 */
#define REPORT_ID_BUTTON      0x03  /* 按钮功能 */
#define REPORT_ID_FEATURE     0x05  /* 多功能 */
#define REPORT_ID_VENDOR_06   0x06  /* 厂商自定义 */

/* 报告长度 */
#define MAX_REPORT_SIZE 1024

static const char *hidraw_paths[] = {
    "/dev/hidraw2",  /* Interface 0 - 基础鼠标 */
    "/dev/hidraw3",  /* Interface 1 - 多点触摸 */
    NULL
};

/**
 * 解析报告 ID 0x02 (基础鼠标 - Interface 0)
 * 3按键 + X/Y相对位移
 */
void parse_report_mouse(const uint8_t *buf, int len)
{
    if (len < 2) return;
    
    uint8_t buttons = buf[0] & 0x07;  /* 低3位: 左(bit0), 右(bit1), 中(bit2) */
    int8_t dx = (int8_t)buf[1];
    int8_t dy = (int8_t)buf[2];
    
    printf("  [鼠标] 按键:%s%s%s  X:%+4d  Y:%+4d\n",
           (buttons & 0x01) ? "左" : "  ",
           (buttons & 0x02) ? "右" : "  ",
           (buttons & 0x04) ? "中" : "  ",
           dx, dy);
}

/**
 * 解析报告 ID 0x01 (标准触摸 - Interface 1)
 * 3按键 + X/Y绝对坐标 + 滚轮
 */
void parse_report_touch(const uint8_t *buf, int len)
{
    if (len < 7) return;
    
    /* 跳过报告ID(1字节) */
    uint8_t buttons = buf[1] & 0x07;
    
    /* X坐标: 12位, 小端序, 范围 -2048~2047 */
    int16_t x = (int16_t)(buf[2] | ((buf[3] & 0x0F) << 8));
    if (x & 0x0800) x |= 0xF000;  /* 符号扩展 */
    
    /* Y坐标: 12位 */
    int16_t y = (int16_t)(((buf[3] >> 4) & 0x0F) | (buf[4] << 4));
    if (y & 0x0800) y |= 0xF000;
    
    /* 滚轮 */
    int8_t wheel = (int8_t)buf[5];
    
    /* 触摸功能键 */
    uint8_t fn_key = (buf[6] >> 4) & 0x0F;
    
    printf("  [触摸] 按键:%s%s%s  X:%+6d  Y:%+6d  滚轮:%+4d  FN:%d\n",
           (buttons & 0x01) ? "左" : "  ",
           (buttons & 0x02) ? "右" : "  ",
           (buttons & 0x04) ? "中" : "  ",
           x, y, wheel, fn_key);
}

/**
 * 解析多点触摸报告 (报告 ID 0x04)
 * 最多4指触摸数据
 */
void parse_report_multitouch(const uint8_t *buf, int len)
{
    if (len < 4) return;
    
    int offset = 1;  /* 跳过报告ID */
    int finger_count = 0;
    
    printf("  [多点触摸]\n");
    
    /* 每个触点9字节 */
    while (offset + 9 <= len && finger_count < 4) {
        uint8_t touch_id = buf[offset] & 0x0F;
        uint8_t touch_active = (buf[offset] >> 4) & 0x01;
        uint8_t touch_type = (buf[offset] >> 5) & 0x07;
        
        /* X坐标: 16位 */
        int16_t x = (int16_t)(buf[offset+1] | (buf[offset+2] << 8));
        /* Y坐标: 16位 */
        int16_t y = (int16_t)(buf[offset+3] | (buf[offset+4] << 8));
        
        if (touch_active) {
            finger_count++;
            printf("    触点%d: ID=%d  X=%+6d  Y=%+6d  Type=%d\n",
                   finger_count, touch_id, x, y, touch_type);
        }
        
        offset += 9;
    }
    
    printf("    活动触点数: %d\n", finger_count);
}

/**
 * 解析触摸状态报告 (报告 ID 0x02 - Interface 1)
 */
void parse_report_touch_state(const uint8_t *buf, int len)
{
    if (len < 3) return;
    
    uint8_t finger_count = buf[1] & 0x0F;
    uint8_t touch_size = buf[2];
    
    printf("  [触摸状态] 手指数:%d  触摸面积:%d\n", finger_count, touch_size);
}

/**
 * 打印原始数据
 */
void print_raw_data(const uint8_t *buf, int len, const char *prefix)
{
    printf("%s [原始 %d字节]: ", prefix, len);
    for (int i = 0; i < len && i < 32; i++) {
        printf("%02X ", buf[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

/**
 * 根据报告ID解析数据
 */
void parse_report(const uint8_t *buf, int len, int interface)
{
    if (len < 1) return;
    
    uint8_t report_id = buf[0];
    
    if (interface == 0) {
        /* Interface 0 - 基础鼠标功能 */
        switch (report_id) {
            case REPORT_ID_MOUSE:
                parse_report_mouse(buf + 1, len - 1);
                break;
            case REPORT_ID_VENDOR_3F:
                printf("  [厂商数据3F] 16字节自定义数据\n");
                break;
            case REPORT_ID_VENDOR_44:
                printf("  [厂商数据44] %d字节自定义数据\n", len - 1);
                break;
            default:
                printf("  [未知报告] ID=0x%02X\n", report_id);
                break;
        }
    } else {
        /* Interface 1 - 多点触摸功能 */
        switch (report_id) {
            case REPORT_ID_MTOUCH:
                parse_report_touch(buf, len);
                break;
            case REPORT_ID_MULTITOUCH:
                parse_report_multitouch(buf, len);
                break;
            case REPORT_ID_TOUCH_STATE:
                parse_report_touch_state(buf, len);
                break;
            case REPORT_ID_BUTTON:
                printf("  [按钮] 数据: ");
                for (int i = 1; i < len && i < 8; i++)
                    printf("%02X ", buf[i]);
                printf("\n");
                break;
            case REPORT_ID_FEATURE:
                printf("  [多功能] 数据: ");
                for (int i = 1; i < len && i < 8; i++)
                    printf("%02X ", buf[i]);
                printf("\n");
                break;
            case REPORT_ID_VENDOR_06:
                printf("  [厂商数据06] 数据: ");
                for (int i = 1; i < len && i < 16; i++)
                    printf("%02X ", buf[i]);
                printf("\n");
                break;
            default:
                printf("  [未知报告] ID=0x%02X\n", report_id);
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    int fd;
    struct hidraw_devinfo info;
    uint8_t buf[MAX_REPORT_SIZE];
    int ret;
    
    printf("=== HID 设备数据探测工具 ===\n");
    printf("设备: Apple Magic Trackpad (05ac:0265)\n\n");
    
    /* 遍历两个接口 */
    for (int i = 0; hidraw_paths[i] != NULL; i++) {
        fd = open(hidraw_paths[i], O_RDONLY);
        if (fd < 0) {
            printf("无法打开 %s: %s\n", hidraw_paths[i], strerror(errno));
            printf("提示: 请使用 sudo 运行\n");
            continue;
        }
        
        /* 获取设备信息 */
        memset(&info, 0, sizeof(info));
        ret = ioctl(fd, HIDIOCGRAWINFO, &info);
        if (ret < 0) {
            printf("获取设备信息失败: %s\n", strerror(errno));
            close(fd);
            continue;
        }
        
        printf("打开 %s (Interface %d)\n", hidraw_paths[i], i);
        printf("  Vendor=0x%04X Product=0x%04X\n", info.vendor, info.product);
        
        /* 设置非阻塞模式 */
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        
        printf("\n等待数据... (请在设备上操作, Ctrl+C退出)\n\n");
        
        /* 读取数据循环 */
        for (int count = 0; count < 50; count++) {
            memset(buf, 0, sizeof(buf));
            ret = read(fd, buf, sizeof(buf));
            
            if (ret > 0) {
                print_raw_data(buf, ret, i == 0 ? "[IF0]" : "[IF1]");
                parse_report(buf, ret, i);
                printf("\n");
            } else if (ret < 0 && errno != EAGAIN) {
                printf("读取错误: %s\n", strerror(errno));
                break;
            }
            
            usleep(10000);  /* 10ms */
        }
        
        close(fd);
        printf("--- Interface %d 探测完成 ---\n\n", i);
    }
    
    printf("探测结束\n");
    return 0;
}
