
# M5Stack_Core2_ScreenBrightness
The Core2 screen brightness is capable of a much larger range of intensities than the standard functions use.

This Arduino .ino sketch file contains a useful function: **core2Brightness()**                   
Normally the brightness can be set from 8 through 20. (2.5 volts to 2.8 volts)                         
This function not enly extends the range of brightness values that can be used from 1 through 36, but it also turns the LED off at value 0.                      
While voltages between 2.8 and 3.3 are allowed, there's an "overdrive" flag if you want to avoid them, as the standard library sets the screen to 2.8 volts at startup. (Which is a "safe maximum" to avoid over heating and LED burnout).

**🔴WARNING!🔴**  
When running over 2.8v - which is over level 20 in my function...           
Direct quote from the M5Stack offices: **"No problem for short-time running in 3.3V. Let's say 1 or 2 hrs."**

 **void core2Brightness(uint8_t lvl, bool overdrive = false)**        
The "lvl" parameter:         
0: Backlight physically off.  
20: The standard brightness set by the Core2 library. (2.8 volts)         
All other values are clamped between 0 and 20.


The "overdrive" parameter:         
This gives an extra 16 brightness levels ABOVE the Core2's standard brightness setting.        
0: Backlight physically off.         
20: The standard brightness set by the Core2 library. (2.8 volts)      
36: Full brightness the screen LED can do. (3.3 volts)       
 

The .ino sketch shows these settings in action, use the touchscreen to control it:

![image](https://user-images.githubusercontent.com/1586332/128866190-4e3f69bd-8aa7-40ec-92f7-ed0894d540bc.png)
