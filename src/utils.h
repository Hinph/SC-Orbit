#ifndef SCWRAPPER_UTILS_H
#define SCWRAPPER_UTILS_H

#include <stddef.h>
#include <stdint.h>

int epoll_ctl_add(int epoll, int fd, uint32_t events);
int epoll_ctl_remove(int epoll, int fd);
int safe_strcpy(char* const dst, char const* const src, size_t count);

#endif
