#include "constants.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>

int epoll_ctl_add(int epoll, int fd, uint32_t events) {
    struct epoll_event event = {
        .data.fd = fd,
        .events  = events,
    };
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event)) {
        perror("failed to add device to epoll");
        return RET_ERROR;
    }
    return RET_OKAY;
}

int epoll_ctl_remove(int epoll, int fd) {
    if (epoll_ctl(epoll, EPOLL_CTL_DEL, fd, NULL)) {
        perror("failed to remove device from epoll");
        return RET_ERROR;
    }
    return RET_OKAY;
}

int safe_strcpy(char* const dst, char const* const src, size_t count) {
    if ((count == 0) || (count <= strlen(src))) {
        return RET_ERROR;
    }
    strcpy(dst, src);
    return RET_OKAY;
}
