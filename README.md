
# M5Stack_Core2_ScreenBrightness
The Core2 screen brightness is capable of a much larger range of intensities than the standard functions use.

This Arduino .ino sketch file contains a useful function: **void core2Brightness(uint8_t lvl, bool overdrive = false)**

The "lvl" parameter:
0: Backlight physically off.
20: The standard brightness set by the Core2 library. (2.8 volts)

All other values are clamped between 0 and 20.


The "overdrive" parameter:
This gives an extra 16 brightness levels ABOVE the Core2's standard brightness setting.
0:(backlight physically off)
20: The standard brightness set by the Core2 library. (2.8 volts)
36: Full brightness the screen LED can do. (3.2 volts)

The .ino sketch shows these settings in action:
![screenBrightness](https://user-images.githubusercontent.com/1586332/128865400-9b9cfd38-35c9-4c84-b473-0021d8c5de32.png)

