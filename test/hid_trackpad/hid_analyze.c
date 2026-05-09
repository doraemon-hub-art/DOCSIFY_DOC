/**
 * hid_analyze.c - 详细分析 HID 数据格式
 * 
 * 读取原始数据并详细解析每个字节的含义
 * 
 * 编译: gcc -o hid_analyze hid_analyze.c
 * 运行: sudo ./hid_analyze
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

static volatile int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

void analyze_report_04(const uint8_t *buf, int len)
{
    printf("\n========== 多点触摸报告 (ID=0x04) 详细分析 ==========\n");
    printf("总长度: %d 字节\n", len);
    
    /* 打印所有字节 */
    printf("\n原始数据:\n");
    for (int i = 0; i < len; i++) {
        printf("  [%2d] 0x%02X (%3d) ", i, buf[i], buf[i]);
        
        /* 添加注释 */
        if (i == 0) printf("<- 报告ID");
        else if (i == 1) {
            int fingers = buf[i] & 0x0F;
            int bit4 = (buf[i] >> 4) & 1;
            int bit5 = (buf[i] >> 5) & 1;
            int bit6 = (buf[i] >> 6) & 1;
            int bit7 = (buf[i] >> 7) & 1;
            printf("<- 状态: 触点数=%d, b4=%d, b5=%d, b6=%d, b7=%d", 
                   fingers, bit4, bit5, bit6, bit7);
        }
        else if (i >= 2 && i <= 25) {
            int touch_idx = (i - 2) / 4;
            int byte_in_touch = (i - 2) % 4;
            if (byte_in_touch == 0) printf("<- 触点%d X低字节", touch_idx + 1);
            else if (byte_in_touch == 1) printf("<- 触点%d X高字节", touch_idx + 1);
            else if (byte_in_touch == 2) printf("<- 触点%d Y低字节", touch_idx + 1);
            else if (byte_in_touch == 3) printf("<- 触点%d Y高字节", touch_idx + 1);
        }
        else if (i == 26) printf("<- 时间戳/序列号 低字节");
        else if (i == 27) printf("<- 时间戳/序列号 高字节");
        else if (i == 28) printf("<- 状态标志1");
        else if (i == 29) printf("<- 状态标志2");
        
        printf("\n");
    }
    
    /* 解析触点数据 */
    printf("\n触点解析:\n");
    int finger_count = buf[1] & 0x0F;
    printf("报告触点数: %d\n", finger_count);
    
    for (int i = 0; i < 6; i++) {
        int offset = 2 + i * 4;
        if (offset + 4 > len) break;
        
        int x = buf[offset] | (buf[offset + 1] << 8);
        int y = buf[offset + 2] | (buf[offset + 3] << 8);
        
        printf("  触点%d: X=%d (0x%04X), Y=%d (0x%04X)%s\n", 
               i + 1, x, x, y, y,
               (x == 0 && y == 0) ? " [无效]" : " [有效]");
    }
    
    /* 时间戳 */
    uint16_t timestamp = buf[26] | (buf[27] << 8);
    printf("\n时间戳/序列号: %d (0x%04X)\n", timestamp, timestamp);
    
    /* 状态标志 */
    printf("状态标志1: 0x%02X\n", buf[28]);
    printf("  bit0=%d, bit1=%d, bit2=%d, bit3=%d, bit4=%d, bit5=%d, bit6=%d, bit7=%d\n",
           buf[28] & 1, (buf[28]>>1)&1, (buf[28]>>2)&1, (buf[28]>>3)&1,
           (buf[28]>>4)&1, (buf[28]>>5)&1, (buf[28]>>6)&1, (buf[28]>>7)&1);
    
    printf("状态标志2: 0x%02X\n", buf[29]);
    printf("  bit0=%d, bit1=%d, bit2=%d, bit3=%d, bit4=%d, bit5=%d, bit6=%d, bit7=%d\n",
           buf[29] & 1, (buf[29]>>1)&1, (buf[29]>>2)&1, (buf[29]>>3)&1,
           (buf[29]>>4)&1, (buf[29]>>5)&1, (buf[29]>>6)&1, (buf[29]>>7)&1);
    
    /* 计算触点间差异 */
    printf("\n触点间差异:\n");
    for (int i = 0; i < 5; i++) {
        int off1 = 2 + i * 4;
        int off2 = 2 + (i+1) * 4;
        if (off2 + 4 > len) break;
        
        int x1 = buf[off1] | (buf[off1 + 1] << 8);
        int y1 = buf[off1 + 2] | (buf[off1 + 3] << 8);
        int x2 = buf[off2] | (buf[off2 + 1] << 8);
        int y2 = buf[off2 + 2] | (buf[off2 + 3] << 8);
        
        if (x1 != 0 || y1 != 0 || x2 != 0 || y2 != 0) {
            printf("  触点%d -> 触点%d: dX=%d, dY=%d\n", 
                   i+1, i+2, x2-x1, y2-y1);
        }
    }
}

void analyze_report_01(const uint8_t *buf, int len)
{
    printf("\n========== 标准触摸报告 (ID=0x01) 详细分析 ==========\n");
    printf("总长度: %d 字节\n", len);
    
    for (int i = 0; i < len; i++) {
        printf("  [%2d] 0x%02X (%3d)", i, buf[i], buf[i]);
        if (i == 0) printf(" <- 报告ID");
        else if (i == 1) {
            printf(" <- 按键: %s%s%s", 
                   (buf[i] & 0x01) ? "左" : "",
                   (buf[i] & 0x02) ? "右" : "",
                   (buf[i] & 0x04) ? "中" : "");
        }
        else if (i == 2) printf(" <- X低4位");
        else if (i == 3) printf(" <- X高4位 + Y低4位");
        else if (i == 4) printf(" <- Y高8位");
        else if (i == 5) printf(" <- 滚轮");
        else if (i == 6) printf(" <- 功能键");
        printf("\n");
    }
    
    if (len >= 7) {
        int x = buf[2] | ((buf[3] & 0x0F) << 8);
        int y = ((buf[3] >> 4) & 0x0F) | (buf[4] << 4);
        int8_t wheel = (int8_t)buf[5];
        printf("\n解析结果: X=%d, Y=%d, 滚轮=%d\n", x, y, wheel);
    }
}

int main(int argc, char *argv[])
{
    int fd;
    struct pollfd fds;
    uint8_t buf[1024];
    int ret;
    int count = 0;
    
    signal(SIGINT, int_handler);
    
    printf("========================================\n");
    printf("  HID 数据格式详细分析工具\n");
    printf("========================================\n\n");
    
    fd = open("/dev/hidraw3", O_RDWR);
    if (fd < 0) {
        perror("无法打开 /dev/hidraw3");
        return 1;
    }
    
    printf("等待数据... (请在设备上操作, Ctrl+C退出)\n");
    printf("将显示前20个数据包的详细分析\n\n");
    
    fds.fd = fd;
    fds.events = POLLIN;
    
    while (keep_running && count < 20) {
        ret = poll(&fds, 1, 5000);
        
        if (ret > 0 && (fds.revents & POLLIN)) {
            memset(buf, 0, sizeof(buf));
            ret = read(fd, buf, sizeof(buf));
            
            if (ret > 0) {
                count++;
                printf("\n------------------- 数据包 #%d -------------------\n", count);
                
                if (buf[0] == 0x04) {
                    analyze_report_04(buf, ret);
                } else if (buf[0] == 0x01) {
                    analyze_report_01(buf, ret);
                } else {
                    printf("未知报告 ID=0x%02X, %d 字节\n", buf[0], ret);
                    for (int i = 0; i < ret; i++) {
                        printf("  [%2d] 0x%02X\n", i, buf[i]);
                    }
                }
            }
        } else if (ret == 0) {
            printf("超时, 无数据\n");
            break;
        }
    }
    
    close(fd);
    printf("\n分析完成\n");
    return 0;
}
