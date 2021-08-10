
#include <M5Core2.h>


void core2Brightness(uint8_t lvl, bool overdrive = false) {
  int v = lvl * 25 + 2300;
  if (v < 2300) v = 2300;
  if (overdrive) {
    if (v > 3200) v = 3200;
  } else {
    if (v > 2800) v = 2800;
  }
  if (v == 2300) {
    M5.Axp.SetDCDC3(false);
    return;
  } else {
    M5.Axp.SetDCDC3(true);
  }
  M5.Axp.SetDCVoltage(2, v);
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
}

void loop() {

  for (uint8_t b = 0; b <= 20; b++) {
    core2Brightness(b);
    delay(50);
  }
  
  for (uint8_t b = 19; b > 0; b--) {
    core2Brightness(b);
    delay(50);
  }
  
}
