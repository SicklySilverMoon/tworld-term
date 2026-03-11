#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/timerfd.h>
#include <linux/serial.h>

uint8_t* read_file(const char* path, size_t* out_size) {
    if (!path || !out_size) {
        errno = EINVAL;
        return NULL;
    }

    *out_size = 0;

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        close(fd);
        return NULL;
    }

    // Optional: ensure it's a regular file
    if (!S_ISREG(st.st_mode)) {
        close(fd);
        errno = EINVAL;
        return NULL;
    }

    size_t size = st.st_size;
    if (size == 0) {
        // Valid empty file
        close(fd);
        return NULL;
    }

    uint8_t* buffer = malloc(size);
    if (!buffer) {
        close(fd);
        return NULL;
    }

    ssize_t total_read = read_data(fd, buffer, size);
    close(fd);

    if (total_read != size) {
        free(buffer);
        errno = EIO;
        return NULL;
    }

    *out_size = size;
    return buffer;
}

ssize_t read_data(int fd, unsigned char* data, ssize_t data_size) {
    ssize_t readIn = 0;
    while (readIn < data_size) {
        ssize_t ret = read(fd, data + readIn, data_size - readIn);
        if (ret == -1 && errno != EAGAIN) {
            perror("Failed to read from input");
            return -1;
        } else if (ret > 0) {
            readIn += ret;
        }
    }
    return readIn;
}

int create_timerfd(bool fireOnce, long seconds, long nanoseconds) {
    //! create timer
    int timer_fd;
    if ((timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)) == -1) {
        perror("Failed to create timer");
        return -1;
    }
    //eprintf("created timerfd %d\n", timer_fd);
    return arm_timerfd(timer_fd, fireOnce, seconds, nanoseconds);
}

int arm_timerfd(int timer_fd, bool fireOnce, long seconds, long nanoseconds) {
    struct itimerspec ts;
    ts.it_value.tv_sec = seconds;
    ts.it_value.tv_nsec = nanoseconds;
    if (!fireOnce) {
        ts.it_interval.tv_sec = seconds;
        ts.it_interval.tv_nsec = nanoseconds;
    } else {
        ts.it_interval.tv_sec = ts.it_interval.tv_nsec = 0;
    }

    //! set the timer's itimerspec
    if (timerfd_settime(timer_fd, 0, &ts, nullptr) < 0) {
        perror("Failed to arm timer");
        close(timer_fd);
        return -1;
    }

    return timer_fd;
}

WINDOW* create_window(int x, int y, int width, int height, bool box) {
    WINDOW* window = newwin(height, width, y, x);
    if (window == NULL) {
        return NULL;
    }
    keypad(window, TRUE);
    if (box) {
        box(window, 0, 0);
    }
    wrefresh(window);
    return window;
}

void c32_to_mb(char buf[MB_CUR_MAX * 2], char32_t c) { //MB_CUR_MAX * 2 can hold at minimum 2 chars
    mbstate_t state = {0};
    size_t written = c32rtomb(buf, c, &state);
    c32rtomb(buf + written, U'\0', &state); //NUL term, proper style
}
