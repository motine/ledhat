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
const uint8_t MARQUEE_LETTER_COUNT = sizeof(MARQUEE_LETTERS) / sizeof(MARQUEE_LETTERS[0]);

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
  virtual void leave(){}; // called when the state is left
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
    // initialize the position
    reset_position();
  }
  void reset_position() {
    // we start with the letters beyond the screen on the right
    position = HAT_WIDTH;
  }

  bool loop() {
    strip.clear();
    uint32_t color = strip.Color(255, 0, 0);
    for (int i = 0; i < MARQUEE_LETTER_COUNT; i++) {
      drawLetter(position + i * 8, MARQUEE_LETTERS[i], MARQUEE_COLOR);
    }
    strip.show();
    position--;
    if (position < (-(int)MARQUEE_LETTER_COUNT * 8))
      reset_position();
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

MarqueeMode marquee_mode_instance = MarqueeMode();
Mode *current_mode = NULL;
// Mode* mode_sequence
void loop() {
  if (current_mode == NULL) {
    current_mode = &marquee_mode_instance;
    current_mode->enter();
  }

  // bool mode_done = current_mode->loop();
  // if
}