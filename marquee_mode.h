#ifndef MARQUEE_MODE_H
#define MARQUEE_MODE_H

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

#endif
