SC-ORBIT
========

SC-Orbit turns the Steam Controller into a controller-first input system for
MiSTer, with fluid trackball and spinner controls, gyro aiming, a virtual
number pad, profiles, and haptic feedback.

INSTALLATION
------------
1. Copy SC-Orbit.sh to /media/fat/Scripts/ on the MiSTer SD card.
2. Run SC-Orbit from MiSTer's Scripts menu.
3. The installer starts the driver immediately and adds it to normal startup.

No extra dependencies, Quartus core, recovery folder, or MiSTer.ini changes are
required. Running the installer again safely updates the existing installation.
If SCWrapper is present, SC-Orbit disables its conflicting startup/runtime entry.

QUICK CONTROLS
--------------
Hold the controller's ... button and press the listed control.

GYRO
  L5                 Gyro master on/off
  L4                 Cycle four direction profiles
  R4 / R5            Gyro sensitivity down/up
  L3 / R3            Left/right analog-stick gyro assist on/off

  Gyro defaults OFF. Profile 4 is the natural direction profile:
  tilt up/down/left/right moves up/down/left/right.
  Analog-stick gyro assist remains EXPERIMENTAL and may vary by game.

TRACKBALL / MOUSE
  Trackball-pad click Toggle trackball/direct-mouse mode
  L2 / R2             Trackball speed down/up
  D-pad left/right    Tighter/looser friction
  D-pad down/up       Momentum down/up

SPINNER
  Spinner-pad click   Toggle iPod wheel/horizontal-X mode while gyro is off
  L1 / R1             Spinner speed down/up

  When gyro is on, the spinner pad becomes a touch-clutched Gyro Paddle.

NUMBER PAD
  B                    Number pad on/off
  Number-pad click     Toggle radial/telephone layout

  Radial: 1 at top-center, then 2-9 clockwise.
  Telephone: 123 / 456 / 789.
  Move across regions to feel them; click a region to type its number.

PROFILES / SYSTEM
  Start                Save user profile
  Select               Load user profile
  Steam/Menu           Restore defaults

See SC-Orbit-Hotkey-Guide.png for the phone-friendly visual reference.

FILES WRITTEN TO THE SD CARD
----------------------------
SC-Orbit installs only the files required to run and start the driver. Runtime
settings are stored at:

  /media/fat/linux/sc-orbit/sc-orbit.ini
  /media/fat/linux/sc-orbit/sc-orbit-user-profile.ini  (only after manual save)

It creates no runtime diagnostic, telemetry, spinner-trace, or gyro-trace files.
The public source is included in this ZIP but is not copied to the SD card.

COMMAND-LINE MAINTENANCE
------------------------
From MiSTer Linux, the installed script also accepts:

  SC-Orbit.sh --status
  SC-Orbit.sh --restart
  SC-Orbit.sh --reset
  SC-Orbit.sh --stop
  SC-Orbit.sh --uninstall

CREDITS
-------
SC-Orbit grew from the Steam Controller support pioneered by SC-Wrapper and its
contributors. SC-Orbit substantially expands and rewrites the installation,
virtual-device architecture, controller-first tuning, trackball, spinner, gyro,
keypad, profile, and haptic systems. Please retain upstream credit when sharing
or modifying the project.
