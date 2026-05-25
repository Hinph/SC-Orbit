#ifndef SCWRAPPER_VIRTUAL_GAMEPAD_H
#define SCWRAPPER_VIRTUAL_GAMEPAD_H

int virtual_gamepad_setup(int* const uinput_fd);
int virtual_gamepad_destroy(int uinput_fd);

#endif // SCWRAPPER_VIRTUAL_GAMEPAD_H
