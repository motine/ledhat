#include <Adafruit_NeoPixel.h>
#define STRIP_WIDTH 300
#define HAT_WIDTH 36
#define HAT_HEIGHT 8
#define STRIP_PIN 6

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(STRIP_WIDTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);


const uint8_t LETTERS[][8] = {
  {
    B11111111,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
  },
  {
    B00000000,
    B01111110,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01111110,
    B00000000,
  },
  {
    B00000000,
    B1100011,
    B1010101,
    B1001001,
    B1000001,
    B1000001,
    B1000001,
    B1000001,
  }
};

void setup() {
  strip.begin();
  strip.clear();
  strip.setBrightness(4); // be careful to not overload the arduino
  strip.show();
}

int i = 0;


// sets the color for the given coordinates.
// negative/out of bounds values are allowed and ignored.
void setPixel(int8_t x, int8_t y, uint32_t color) {
  if ((x < 0) || (x >= HAT_WIDTH)) // ignore out of bounds
    return;
  if ((y < 0) || (y >= HAT_HEIGHT))
    return;
  strip.setPixelColor((HAT_HEIGHT-1-y) * HAT_WIDTH + x, color);
}

// looks up the letter with the given index and draws it with the given color.
// the x_offset determines how much the letter should be moved to the left or right.
// negative offsets are valid (means shift to the left leading to a partial letter).
// partial letters are also drawn and out of bounds coordinates are skipped.
void drawLetter(int8_t x_offset, uint8_t letter_index, const uint32_t color) {
  if ((x_offset < -7) || (x_offset >= HAT_WIDTH)) // skip out of bounds
    return;
  for (uint8_t j = 0; j < 8; j++) { // y
    for (uint8_t i = 0; i < 8; i++) { // x
      uint8_t mask = 1 << i;
      if (LETTERS[letter_index][j] & mask) {
        setPixel(x_offset + i, j, color);
      }
    }
  }
}

void loop() {
  strip.clear();
  uint32_t color = strip.Color(255, 0, 0);
  drawLetter(0, 0, color);
  drawLetter(8, 1, color);
  drawLetter(16, 2, color);
  // static uint8_t cur = 0;
  // for (int i = 0; i < HAT_HEIGHT; i++) {
  //   setPixel(cur, i, color);
  // }
  // cur = (cur+1) % HAT_WIDTH;
  strip.show();
  delay(200);
}