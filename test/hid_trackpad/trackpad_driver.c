/**
 * trackpad_driver.c - 驱动板用户空间驱动 v3.1
 * 
 * 通过 hidraw 读取驱动板的 HID 数据，解析触摸/按键信息，
 * 通过 uinput 模拟鼠标/触摸板输入
 * 
 * 数据格式 (报告 ID=0x04, 30字节):
 *   [0]   报告ID (0x04)
 *   [1]   触点数 (低4位, 但实际是槽位数, 需根据有效触点判断)
 *   [2-3] 触点1 X坐标 (16位小端)
 *   [4-5] 触点1 Y坐标 (16位小端)
 *   [6-7] 触点2 X坐标
 *   [8-9] 触点2 Y坐标
 *   [10-11] 触点3 X坐标
 *   [12-13] 触点3 Y坐标
 *   [14-15] 触点4 X坐标
 *   [16-17] 触点4 Y坐标
 *   [18-19] 触点5 X坐标
 *   [20-21] 触点5 Y坐标
 *   [22-23] 触点6 X坐标
 *   [24-25] 触点6 Y坐标
 *   [26-27] 时间戳
 *   [28]   状态标志1
 *   [29]   状态标志2
 * 
 * 编译: gcc -o trackpad_driver trackpad_driver.c
 * 运行: sudo ./trackpad_driver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <poll.h>
#include <signal.h>
#include <time.h>

/* ============ 配置 ============ */
#define HIDRAW_PATH  "/dev/hidraw11"

/* 灵敏度参数 */
#define SCROLL_THRESHOLD     15
#define MOVE_THRESHOLD       2     /* 移动阈值 */
#define CLICK_MOVE_MAX       30    /* 点击允许的最大移动距离 */

/* ============ 全局状态 ============ */
static volatile int keep_running = 1;
static int uinput_fd = -1;
static int debug = 0;

typedef struct {
    int prev_x, prev_y;
    int current_x, current_y;
    int has_touch;
    int prev_has_touch;
    int finger_count;       /* buf[1] & 0x0F 报告的触点数 */
    int prev_finger_count;
    
    int scroll_accum_y;
    int scroll_accum_x;
    
    int touch_start_x, touch_start_y;
    int is_click;
} DriverState;

static DriverState state;

void int_handler(int dummy) {
    keep_running = 0;
}

/* ============ uinput 初始化 ============ */
int setup_uinput_device(void)
{
    struct uinput_setup usetup;
    int fd;
    
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0) {
            perror("无法打开 /dev/uinput");
            return -1;
        }
    }
    
    printf("  /dev/uinput 打开成功 (fd=%d)\n", fd);
    
    /* 启用事件类型 */
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    
    /* 设置按键 */
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_FINGER);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_DOUBLETAP);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_TRIPLETAP);
    
    /* 设置相对事件 */
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);
    ioctl(fd, UI_SET_RELBIT, REL_HWHEEL);
    
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x05AC;
    usetup.id.product = 0x0265;
    strcpy(usetup.name, "Custom Trackpad Driver");
    
    if (ioctl(fd, UI_DEV_SETUP, &usetup) < 0) {
        perror("  UI_DEV_SETUP 失败");
        close(fd);
        return -1;
    }
    
    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        perror("  UI_DEV_CREATE 失败");
        close(fd);
        return -1;
    }
    
    printf("  uinput 设备创建成功\n");
    return fd;
}

/* ============ uinput 事件发送 ============ */
void emit(int type, int code, int value)
{
    struct input_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    ev.code = code;
    ev.value = value;
    write(uinput_fd, &ev, sizeof(ev));
}

void syn(void) { emit(EV_SYN, SYN_REPORT, 0); }
void key(int code, int val) { emit(EV_KEY, code, val); }
void rel(int code, int val) { emit(EV_REL, code, val); }

/* ============ 数据处理 ============ */

void process_report(const uint8_t *buf, int len)
{
    if (len < 30 || buf[0] != 0x04) {
        return;
    }
    
    /* 解析触点数 (buf[1] 低4位) */
    state.prev_finger_count = state.finger_count;
    state.finger_count = buf[1] & 0x0F;
    
    /* 解析第一个触点坐标 */
    int x = buf[2] | (buf[3] << 8);
    int y = buf[4] | (buf[5] << 8);
    
    /* 判断是否有有效触摸 */
    state.prev_has_touch = state.has_touch;
    state.has_touch = (x != 0 || y != 0);
    
    if (state.has_touch) {
        state.prev_x = state.current_x;
        state.prev_y = state.current_y;
        state.current_x = x;
        state.current_y = y;
    }
    
    /* ====== 处理触摸事件 ====== */
    
    if (state.has_touch) {
        /* 触摸开始 */
        if (!state.prev_has_touch) {
            state.touch_start_x = x;
            state.touch_start_y = y;
            state.is_click = 1;
            state.scroll_accum_y = 0;
            state.scroll_accum_x = 0;
            if (debug) printf("  触摸开始: (%d,%d) 手指=%d\n", x, y, state.finger_count);
        }
        
        /* 检测移动 */
        if (state.prev_has_touch && state.prev_x != 0) {
            int dx = state.current_x - state.prev_x;
            int dy = state.current_y - state.prev_y;
            
            /* 检查是否移动超过点击阈值 */
            int total_dx = abs(state.current_x - state.touch_start_x);
            int total_dy = abs(state.current_y - state.touch_start_y);
            if (total_dx > CLICK_MOVE_MAX || total_dy > CLICK_MOVE_MAX) {
                state.is_click = 0;
            }
            
            /* 单指: 移动鼠标 */
            if (state.finger_count == 1) {
                if (abs(dx) > MOVE_THRESHOLD || abs(dy) > MOVE_THRESHOLD) {
                    if (debug) printf("  移动: dx=%d, dy=%d\n", dx, dy);
                    rel(REL_X, dx);
                    rel(REL_Y, dy);
                }
            }
            
            /* 双指: 移动 + 滚轮 */
            if (state.finger_count >= 2) {
                /* 也发送移动 */
                if (abs(dx) > MOVE_THRESHOLD || abs(dy) > MOVE_THRESHOLD) {
                    if (debug) printf("  双指移动: dx=%d, dy=%d\n", dx, dy);
                    rel(REL_X, dx);
                    rel(REL_Y, dy);
                }
                
                /* 滚轮 */
                state.scroll_accum_y += dy;
                state.scroll_accum_x += dx;
                
                while (state.scroll_accum_y >= SCROLL_THRESHOLD) {
                    rel(REL_WHEEL, -1);
                    state.scroll_accum_y -= SCROLL_THRESHOLD;
                }
                while (state.scroll_accum_y <= -SCROLL_THRESHOLD) {
                    rel(REL_WHEEL, 1);
                    state.scroll_accum_y += SCROLL_THRESHOLD;
                }
                while (state.scroll_accum_x >= SCROLL_THRESHOLD) {
                    rel(REL_HWHEEL, 1);
                    state.scroll_accum_x -= SCROLL_THRESHOLD;
                }
                while (state.scroll_accum_x <= -SCROLL_THRESHOLD) {
                    rel(REL_HWHEEL, -1);
                    state.scroll_accum_x += SCROLL_THRESHOLD;
                }
            }
        }
        
        /* 设置触摸状态 */
        key(BTN_TOUCH, 1);
        key(BTN_TOOL_FINGER, (state.finger_count == 1) ? 1 : 0);
        key(BTN_TOOL_DOUBLETAP, (state.finger_count == 2) ? 1 : 0);
        key(BTN_TOOL_TRIPLETAP, (state.finger_count >= 3) ? 1 : 0);
        
    } else {
        /* 触摸结束 - 检测点击 */
        if (state.prev_has_touch && state.is_click) {
            if (debug) printf("  点击! 位置=(%d,%d) 手指=%d\n", 
                state.touch_start_x, state.touch_start_y, state.prev_finger_count);
            
            if (state.prev_finger_count == 1) {
                key(BTN_LEFT, 1);
                syn();
                usleep(50000);
                key(BTN_LEFT, 0);
            } else if (state.prev_finger_count == 2) {
                key(BTN_RIGHT, 1);
                syn();
                usleep(50000);
                key(BTN_RIGHT, 0);
            }
        }
        
        /* 清除触摸状态 */
        key(BTN_TOUCH, 0);
        key(BTN_TOOL_FINGER, 0);
        key(BTN_TOOL_DOUBLETAP, 0);
        key(BTN_TOOL_TRIPLETAP, 0);
        
        state.scroll_accum_y = 0;
        state.scroll_accum_x = 0;
    }
    
    syn();
}

/* ============ 主循环 ============ */

int main(int argc, char *argv[])
{
    int fd;
    struct pollfd fds;
    uint8_t buf[1024];
    int ret;
    
    signal(SIGINT, int_handler);
    
    printf("========================================\n");
    printf("  驱动板用户空间驱动 v3.1\n");
    printf("========================================\n\n");
    
    memset(&state, 0, sizeof(state));
    
    /* 打开 HID 设备 */
    fd = open(HIDRAW_PATH, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "无法打开 %s: %s\n", HIDRAW_PATH, strerror(errno));
        return 1;
    }
    printf("  打开 %s 成功 (fd=%d)\n", HIDRAW_PATH, fd);
    
    /* 设置 uinput */
    uinput_fd = setup_uinput_device();
    if (uinput_fd < 0) {
        close(fd);
        return 1;
    }
    
    printf("\n驱动已启动!\n");
    printf("  单指: 移动鼠标\n");
    printf("  单指点击: 左键\n");
    printf("  双指: 移动 + 滚轮\n");
    printf("  双指点击: 右键\n");
    printf("  Ctrl+C 退出\n\n");
    
    /* 主循环 */
    fds.fd = fd;
    fds.events = POLLIN;
    
    while (keep_running) {
        ret = poll(&fds, 1, 50);
        
        if (ret > 0 && (fds.revents & POLLIN)) {
            memset(buf, 0, sizeof(buf));
            ret = read(fd, buf, sizeof(buf));
            
            if (ret > 0) {
                process_report(buf, ret);
            }
        } else if (ret < 0 && errno != EINTR) {
            perror("poll 错误");
            break;
        }
    }
    
    /* 清理 */
    printf("\n正在关闭驱动...\n");
    
    if (uinput_fd >= 0) {
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
    }
    
    close(fd);
    printf("驱动已关闭\n");
    return 0;
}
