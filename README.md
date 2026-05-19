# Steam Controller Wrapper
(I'm currently looking for a better name.)

SCWrapper is a Linux application that translates inputs from the 2026 version of the Steam Controller into standard gamepad controls without depending on the Steam Client. This enables the Steam Controller to function in nontraditional environments such as on the MisterFPGA or Raspberry Pi where a Steam client may be unavailable or the default keyboard + mouse control scheme is unsuitable.

### Features:
- Basic Steam Controller support (buttons, triggers, touchpads, and joysticks.)
- Support for the Steam Controller Puck (i.e. handling multiple controllers) --- Note: This is untested, because I only have one controller.
- *Multiple* Puck Support: up to 16 controllers, but can be compiled to handle more. --- Note: Untested for the same reason.
- Supports:
  - x86_64 Linux
  - ARMv7L Linux (for the MisterFPGA)

### Planned Features:
- Rumble Support
- Gyro Support
- Alternative Touchpad Configurations
- Haptics Support (?)

## MisterFPGA / MisterPi Installation Guide
1. Download [scwrapper.sh](https://raw.githubusercontent.com/bnpfeife/scwrapper/refs/heads/main/Mister/scwrapper.sh) and place it in the MisterFPGA's Scripts directory.
2. Boot up the MisterFGPA with a keyboard attached, and run the scwrapper.sh script.
3. Follow the prompts.
4. Plug in your Steam Controller (or Puck) after the MisterFGPA reboots.
5. Enjoy! (Please report any issues or bugs you encounter!)

## Linux Installation Guide
1. Download the [udev rules](https://raw.githubusercontent.com/bnpfeife/scwrapper/refs/heads/main/Linux/files/99-disable-sc.rules) and place it in the `/etc/udev/rules.d` directory.
2. Reboot or run these commands:
```
# udevadm control --reload-rules
# udevadm trigger
```
3. Download the [latest](https://github.com/bnpfeife/scwrapper/releases/tag/latest) scwrapper binary from Releases.
4. Run `scwrapper`
5. Plug in your Steam Controller (or Puck).
6. Enjoy! (Please report any issues or bugs you encounter!)

## Build Guide
### For Desktop Linux:
Dependencies:
- gcc
- make
- glibc (development libraries)
- linux-headers (development libraries)

Run this command within the project directory:
```
$ make
```

### For MisterFPGA:
Dependencies:
- docker

Run this command within the project directory:
```
$ Mister/build
```

## Contributing
Please note that this project is in a very early state, so large architectural changes may occur.  Feel free to create an issue or submit a pull-request if you encounter a bug or wish to develop / suggest a feature!
