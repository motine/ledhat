#ifndef COLORS_MODE_H
#define COLORS_MODE_H

#include "strip.h"

#define COLORS_SPEED_DIVISOR 20 // higher value means slower
#define COLORS_DURATION 5000    // milliseconds until we change to the next mode

class ColorsMode : public Mode {
public:
  void enter() {
    // remember when we started
    start_millis = millis();
  }

  bool loop() {
    fill_all(hue_to_rgb((millis() / COLORS_SPEED_DIVISOR % 360)));
    strip.show();
    delay(50);
    if ((millis() - start_millis) > COLORS_DURATION)
      return true;
    return false;
  }

protected:
  // converts a HSV to RGB values. For optimization we assume full S and full V.
  // hue is between 0.0 and 1.0.
  // for conversion for:
  // - http://www.rapidtables.com/convert/color/hsv-to-rgb.htm
  // - http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
  uint32_t hue_to_rgb(uint16_t hue) {
    // uint8_t h = hue / 60;
    // float x = 1 - abs(fmod(hue / 60.0, 2.0) - 1);
    // uint8_t xi = x * 255;

    uint8_t h = hue / 60;
    uint16_t x = 255 * (1 - abs(fmod(hue / 60.0, 2.0) - 1));

    switch (h) {
    case 0:
      return strip.Color(255, x, 0);
    case 1:
      return strip.Color(x, 255, 0);
    case 2:
      return strip.Color(0, 255, x);
    case 3:
      return strip.Color(0, x, 255);
    case 4:
      return strip.Color(x, 0, 255);
    default:
      return strip.Color(255, 0, x);
    }
  }

  void fill_all(uint32_t color) {
    for (int i = 0; i < STRIP_WIDTH; i++) {
      strip.setPixelColor(i, color);
    }
  }

private:
  unsigned long start_millis;
};

#endif