#ifndef TWORLD_TERM_MISC_H
#define TWORLD_TERM_MISC_H

#include <stdint.h>
#include <stdlib.h>
#include <uchar.h>
#include <curses.h>

uint8_t* read_file(const char* path, size_t* out_size);
ssize_t read_data(int fd, unsigned char* data, ssize_t data_size);
int create_timerfd(bool fireOnce, long seconds, long nanoseconds);
int arm_timerfd(int timer_fd, bool fireOnce, long seconds, long nanoseconds);
WINDOW* create_window(int x, int y, int width, int height, bool box);
void c32_to_mb(char buf[MB_CUR_MAX * 2], char32_t c);

#endif //TWORLD_TERM_MISC_H
