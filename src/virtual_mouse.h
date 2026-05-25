#ifndef SCWRAPPER_VIRTUAL_MOUSE_H
#define SCWRAPPER_VIRTUAL_MOUSE_H

int virtual_mouse_setup(int* const uinput_fd);
int virtual_mouse_destroy(int uinput_fd);

#endif // SCWRAPPER_VIRTUAL_MOUSE_H
