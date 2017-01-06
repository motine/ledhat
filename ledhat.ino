#include <Adafruit_NeoPixel.h>

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
#define COLORS_DURATION 5000    // milliseconds until we change to the next mode

#include "letters.h"
#include "mode.h"
#include "marquee_mode.h"
#include "animation_mode.h"
#include "colors_mode.h"

void setup() {
  strip.begin();
  strip.clear();
  strip.setBrightness(4); // be careful to not overload the arduino
  strip.show();
}

int i = 0;

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