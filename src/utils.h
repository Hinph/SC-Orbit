#ifndef SCWRAPPER_UTILS_H
#define SCWRAPPER_UTILS_H

#include <stdint.h>

int epoll_ctl_add(int epoll, int fd, uint32_t events);
int epoll_ctl_remove(int epoll, int fd);

#endif
