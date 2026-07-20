# SC-Orbit

**Turn the Steam Controller into a trackball, spinner, mouse, gyro controller, number pad, and more on MiSTer FPGA.**

Built off of the excellent work the SC-Wrapper team did getting the Steam Controller to play nice with MiSTer, I wanted to go further by making use of all of the other weird inputs that make the controller so weird and lovely… and I also didn’t want to spend $200 on some physical controller with a trackball and spinner because I’m cheap and figured simulating them would be good enough!

SC-Orbit features smooth trackball controls, multiple spinner styles, gyro aiming, controller-based settings, haptic feedback, and a few ideas that probably sounded unreasonable when I first thought of them.

No keyboard required. No settings app to open. Hold the … button and adjust everything directly from the controller.

<img width="1024" height="1536" alt="HotkeyGuide" src="https://github.com/user-attachments/assets/c7e3c483-c07d-4e0d-a420-24616252a3db" />


## What SC-Orbit Does

### Trackball

The right or left trackpad can behave like a proper momentum-based trackball.

You can adjust:

- Speed
- Friction
- Momentum transfer
- Haptic strength
- Trackball or mouse behavior

Low momentum settings can feel tight and mouse-like, while higher settings allow large flicks and long rolling movement. Mess around with these settings and you should be able to tune things the way that feels best to you!

### Spinner

The assigned spinner pad supports two main control styles:

- iPod Wheel – Move your thumb in a circle around the pad.
- Horizontal Drag – Slide left and right across the pad.

These can be used for games such as Arkanoid, Tempest, Championship Sprint, paddle games, driving games, and anything else that benefits from spinner-style input.

Spinner speed can be adjusted directly – Slide left and right across the pad.


### Gyro

SC-Orbit includes touch-activated gyro controls.

The gyro can be turned on or off at any time. When it is off, all gyro functions are completely disabled.

Gyro can be used for:

- Mouse and trackball aiming
- Gyro Paddle
- Left analog-stick assistance
- Right analog-stick assistance

The regular mouse-style gyro is designed for precise aiming and immediately stops when the controller stops moving.

Analog-stick gyro assistance is currently experimental. It is included because analog-only games such as GoldenEye 007 are an interesting challenge, but converting modern gyro movement into old analog-stick camera controls will always depend somewhat on the game.

### Virtual Number Pad

The spinner pad can also become a simulated nine-button number pad

Two layouts are included:

- Radial: 1 at the top, with 2–9 continuing clockwise
- Telephone: Standard 123 / 456 / 789 layout

Move your thumb across the pad to feel each imaginary button through the controller’s haptics, then physically click the pad to type the selected number.

This may be useful for computer cores, keypad-heavy games, passwords, menu shortcuts, or for all of those weird systems with stupid controllers that included them… just kidding, I love Jaguar. Roar.

### Controller-Based Settings

Almost every important setting can be changed without leaving the game.

Hold the … button and use controller shortcuts to:

- Adjust trackball speed
- Adjust spinner speed
- Change friction and momentum
- Toggle gyro
- Change gyro sensitivity and direction
- Swap the trackpads
- Toggle mouse mode
- Open the number pad
- Save or load a profile
- Restore defaults

Each adjustment includes haptic feedback, and mode changes play sounda through the Steam Controller’s haptic actuators.

The controller sings a little... chirps and purrs. Maybe we can get prettier sounds in future.

I really didn’t want users to have to open up boring 80’s terminals to change settings, so while I know it’s a lot of shortcuts I believe in you...

## Installation

1. Download SC-Orbit.sh from the latest release.
2. Copy it to the Scripts folder on your MiSTer SD card.
3. Run it from MiSTer’s Scripts menu.
4. Connect your Steam Controller.
5. Map your buttons (ignore mouse controls)
6. Keep in mind that every core handles inputs like mouse and spinner differently, so always check the core settings… Centipede, for example, seems to work best with the trackball scaling turned to 200%

The installer includes everything SC-Orbit needs. It does not depend on a separate SC-Wrapper installation.

SC-Orbit will also remove the old SC-Wrapper startup entry when appropriate so that both drivers do not fight over the same controller.

Your settings are stored inside:

```text
/media/fat/linux/sc-orbit/
```

## Profiles

SC-Orbit automatically remembers your current settings.

You can also save one dedicated user profile with:

```text
… + Start
```

Load it again with:

```text
… + Select
```

The profile includes your trackball, spinner, gyro, haptic, pad-layout, and number-pad preferences.

## Project Status

SC-Orbit is ready for general use, but I am absolutely open to improving it as people try it with more games and MiSTer cores.

The main feature still considered experimental is:

- Gyro assistance converted into analog-stick output (this is tough)

Regular mouse-style gyro, trackball, spinner, keypad, profiles, and controller hotkeys are part of the main feature set.

## Credits

SC-Orbit grew out of the Steam Controller support originally developed for SC-Wrapper.

Huge credit goes to the SC-Wrapper project and its contributors for doing the difficult early work of communicating with the Steam Controller and making it usable on MiSTer.

SC-Orbit became its own project after substantially expanding and reworking the controller behavior, input routing, trackball and spinner physics, gyro systems, haptics, profiles, installation process, and controller-first configuration.

Also, thank you to Valve for making one of the strangest and most interesting controllers ever released. Combining this thing with MiSTer has been a lot of fun.

And thanks to everybody who tests this, breaks it, reports something useful, or discovers a game that suddenly feels great with these controls!
