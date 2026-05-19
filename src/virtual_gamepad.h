#pragma once

#include <linux/uinput.h>

int virtual_gamepad_setup(int* const uinput_fd);
int virtual_gamepad_destroy(int uinput_fd);
