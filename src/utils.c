#include "utils.h"

#include <stdio.h>
#include <sys/epoll.h>

int epoll_ctl_add(int epoll, int fd, uint32_t events) {
    struct epoll_event event = {
        .data.fd = fd,
        .events  = events,
    };
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event)) {
        perror("failed to add device to epoll");
        return 1;
    }
    return 0;
}

int epoll_ctl_remove(int epoll, int fd) {
    if (epoll_ctl(epoll, EPOLL_CTL_DEL, fd, NULL)) {
        perror("failed to remove device from epoll");
        return 1;
    }
    return 0;
}
