
#include <M5Core2.h>

uint8_t oldSliderValue = 0;

bool turbo = false;
bool turboOld = false;

struct Touches {
  bool touching;
  uint16_t x;
  uint16_t y;
} touch;

struct Sliders {
  uint16_t width;
  uint16_t height;
  uint16_t x;
  uint16_t y;
  uint16_t steps;
  uint16_t value;
  uint16_t inRed;
  float stepPixels;
} slider;

struct Buttons {
  uint16_t x;
  uint16_t y;
  uint16_t size;
  bool on;
  bool pressed;
  bool oldPressed;
} button;


//####################################

// lvl: 0:(backlight physically off), 20: full brightness (2.8 volts)

// Overdrive:
// This gives an extra 16 brightness levels ABOVE Core2's standard setting.
// lvl: 0:(backlight physically off), 36: full brightness (3.2 volts)

void core2Brightness(uint8_t lvl, bool overdrive = false) {
  // The backlight brightness is in steps of 25 in AXP192.cpp
  // calculation in SetDCVoltage: ( (voltage - 700) / 25 )
  // 2325 is the minimum "I can just about see a glow in a dark room" level of brightness.
  // 2800 is the value set by the AXP library as "standard" bright backlight.
  int v = lvl * 25 + 2300;

  // Clamp to safe values.
  if (v < 2300) v = 2300;
  if (overdrive) {
    if (v > 3200) v = 3200; // maximum of 3.2 volts, 3200 (uint8_t lvl  = 36) absolute max!
  } else {
    if (v > 2800) v = 2800; // maximum of 2.8 volts, 2800 (uint8_t lvl  = 20)
  }


  // Minimum brightness means turn off the LCD backlight.
  if (v == 2300) {
    // LED set to minimum brightness? Turn off.
    M5.Axp.SetDCDC3(false);
    return;
  } else {
    // Ensure backlight is on. (magic name = DCDC3)
    M5.Axp.SetDCDC3(true);
  }

  // Set the LCD backlight voltage. (magic number = 2)
  M5.Axp.SetDCVoltage(2, v);
}

//####################################

void setup() {
  M5.begin();
  
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextSize(1);
  core2Brightness(20);
  slider.width = 250;
  slider.height = 50;
  slider.x = (320 - slider.width) >> 1;
  slider.y = 180;
  slider.steps = 36;
  slider.inRed = 21;
  slider.value = 20;
  slider.stepPixels = slider.width / (slider.steps - 1);

  button.x = 25;
  button.y = 10;
  button.size = 70;
  button.on = false;
  button.pressed = false;
  button.oldPressed = false;

  M5.Lcd.setFreeFont(FSS12);
  M5.Lcd.setCursor(button.x + button.size + 30, button.y + (button.size >> 1) - 5);
  M5.Lcd.printf(":Press to enable");
  M5.Lcd.setCursor(button.x + button.size +40, button.y + (button.size >> 1) + 16);
  M5.Lcd.printf("turbo brightness.");

  drawSlider();
  drawTurboButton();

  //takeScreenshot565();
}

void loop() {
  M5.update();
  updateTouch();
  if (touch.touching) {
    updateTurboButton(touch.x, touch.y);
    turboOld = turbo;
    turbo = button.on;
    if (turbo != turboOld) {
      core2Brightness(slider.value, button.on);
      drawTurboButton();
      drawSlider();
    }
    updateSliderTouchValue(touch.x, touch.y);
    if (slider.value != oldSliderValue) {
      core2Brightness(slider.value, turbo);
      drawSlider();
      oldSliderValue = slider.value;
    }
  } else {
    button.pressed = false;
    button.oldPressed = false;
  }
}

void updateTurboButton(uint16_t x, uint16_t y) {
  // Offset the touch.
  x -= 50;
  y -= 20;
  if (!( (x >= button.x) && (x <= button.x + button.size) && (y >= button.y) && (y <= button.y + button.size) )) {
    button.pressed = false;
    button.oldPressed = false;
    return;
  }
  button.pressed = true;
  if (!button.oldPressed) {
    button.on = !button.on;

  }
  button.oldPressed = button.pressed;
}

void drawTurboButton() {
  uint16_t x = button.x;
  uint16_t y = button.y;
  uint16_t size = button.size;
  if (button.on)
    M5.Lcd.fillRoundRect(x, y, x + size, y + size, 5, RED);
  else {
    M5.Lcd.fillRoundRect(x, y, x + size, y + size, 5, WHITE);
    M5.Lcd.drawRoundRect(x, y, x + size, y + size, 5, RED);
  }
}

void drawSlider() {
  M5.Lcd.fillRect(slider.x, slider.y - slider.height, slider.width, slider.height, WHITE);
  M5.Lcd.drawLine(slider.x, slider.y, slider.x + (slider.inRed - 1) * slider.stepPixels, slider.y, BLACK);
  M5.Lcd.drawLine(slider.x + (slider.inRed - 1) * slider.stepPixels, slider.y, slider.x + slider.width, slider.y, RED);
  for (float i = 0; i < slider.steps; i++) {
    float sx = ((float)slider.x + (i * slider.stepPixels));
    float ex = ((float)slider.x + ((i + 1) * slider.stepPixels));
    ex += (i >= slider.value ? -(slider.stepPixels - 1) : -1); // Make blank sections at least 1 pixel
    uint16_t color = (i < slider.inRed) ? BLACK : RED;
    for (float x = sx; x < ex; x++)
      M5.Lcd.drawLine((uint16_t)x, slider.y, (uint16_t)x, slider.y - slider.height, color);
  }
  M5.Lcd.fillRect(145, 92, 40, 25, WHITE);
  M5.Lcd.setCursor(150, 110);
  if (!turbo && slider.value > 20) {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.printf("20");
  } else {
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.printf("%hu", slider.value);
  }
}

void updateSliderTouchValue(uint16_t x, uint16_t y) {
  if (!( (x >= slider.x) && (x <= slider.x + slider.width + slider.stepPixels) && (y >= slider.y - slider.height) && (y <= slider.y) )) return;
  uint16_t val = (x - slider.x) / slider.stepPixels;
  slider.value = val;
  return;
}

void updateTouch() {
  Event& e = M5.Buttons.event;
  if (e & (E_TOUCH | E_MOVE)) {
    touch.touching = true;
    touch.x = e.to.x;
    touch.y = e.to.y;
  } else {
    touch.touching = false;
  }
}

void takeScreenshot565(void) {
  uint16_t pixels[6];
  Serial.println(F("--------------- COPY CODE AFTER THIS LINE FOR RGB565 SCREEN SHOT - https://codepen.io/SarahC/pen/oNWqKQa ----------------"));
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x += 5) {
      M5.Lcd.readRect(x, y, 5, 1, pixels);
      for (int arrayPtr = 0; arrayPtr < 5; arrayPtr++) {
        Serial.printf("%04X", pixels[arrayPtr]);
      }
    }
    if (y != 240) Serial.println("");
  }
  Serial.println(F("--------------- COPY CODE UP TO THIS LINE FOR RGB565 SCREEN SHOT - https://codepen.io/SarahC/pen/oNWqKQa ----------------"));
}
