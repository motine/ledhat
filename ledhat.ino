#include <Adafruit_NeoPixel.h>

#include "letters.h"

#define STRIP_WIDTH 300
#define HAT_WIDTH 36
#define HAT_HEIGHT 8
#define STRIP_PIN 6

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(STRIP_WIDTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

const uint8_t MARQUEE_LETTERS[] = {0, 1, 2, 2, 2, 1};
const uint32_t MARQUEE_COLOR = strip.Color(255, 255, 0);
#define MARQUEE_DELAY 100 // milliseconds between frames
const uint8_t MARQUEE_LETTER_COUNT =
    sizeof(MARQUEE_LETTERS) / sizeof(MARQUEE_LETTERS[0]);

#define COLORS_SPEED_DIVISOR 20 // higher value means slower
#define COLORS_DURATION 5000 // milliseconds until we change to the next mode

void setup() {
  strip.begin();
  strip.clear();
  strip.setBrightness(4); // be careful to not overload the arduino
  strip.show();
}

int i = 0;

class Mode {
public:
  Mode(){};
  // if loop returns a true, it means that this mode is done
  virtual bool loop() { return NULL; };
  virtual void enter(){}; // called when the state is entered
protected:
  // sets the color for the given coordinates.
  // negative/out of bounds values are allowed and ignored.
  void setPixel(int8_t x, int8_t y, uint32_t color) {
    if ((x < 0) || (x >= HAT_WIDTH)) // ignore out of bounds
      return;
    if ((y < 0) || (y >= HAT_HEIGHT))
      return;
    strip.setPixelColor((HAT_HEIGHT - 1 - y) * HAT_WIDTH + x, color);
  }
};

class MarqueeMode : public Mode {
public:
  void enter() {
    // we start with the letters beyond the screen on the right
    position = HAT_WIDTH;
  }
  bool loop() {
    strip.clear();
    for (int i = 0; i < MARQUEE_LETTER_COUNT; i++) {
      drawLetter(position + i * 8, MARQUEE_LETTERS[i], MARQUEE_COLOR);
    }
    strip.show();
    position--;
    if (position < (-(int)MARQUEE_LETTER_COUNT * 8))
      return true;
    delay(MARQUEE_DELAY);
    return false;
  }

  // looks up the letter with the given index and draws it with the given color.
  // the x_offset determines how much the letter should be moved to the left or
  // right.
  // negative offsets are valid (means shift to the left leading to a partial
  // letter).
  // partial letters are also drawn and out of bounds coordinates are skipped.
  void drawLetter(int8_t x_offset, uint8_t letter_index, const uint32_t color) {
    if ((x_offset < -7) || (x_offset >= HAT_WIDTH)) // skip out of bounds
      return;
    for (uint8_t j = 0; j < 8; j++) {   // y
      for (uint8_t i = 0; i < 8; i++) { // x
        uint8_t mask = 1 << i;
        if (LETTER_IMAGES[letter_index][j] & mask) {
          setPixel(x_offset + i, j, color);
        }
      }
    }
  }

private:
  int position;
};

class AnimationMode : public Mode {
public:
  void enter() {
    // we start with the letters beyond the screen on the right
    position = HAT_WIDTH;
  }

  bool loop() {
    strip.clear();
    uint32_t color = strip.Color(255, 0, 0);
    // TODO
    return true;
    delay(100);
    return false;
  }

private:
  int position;
};

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

MarqueeMode marquee_mode_instance = MarqueeMode();
AnimationMode animation_mode_instance = AnimationMode();
ColorsMode colors_mode_instance = ColorsMode();
Mode *mode_sequence[] = {&colors_mode_instance, &marquee_mode_instance,
                         &animation_mode_instance};

bool mode_was_done = true;
short current_mode_index = -1;
void loop() {
  if (mode_was_done) {
    current_mode_index = (current_mode_index + 1) %
                         (sizeof(mode_sequence) / sizeof(mode_sequence[0]));
    mode_sequence[current_mode_index]->enter();
  }
  mode_was_done = mode_sequence[current_mode_index]->loop();
}