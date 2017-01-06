#include <Adafruit_NeoPixel.h>

#include "strip.h"
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
Mode *mode_sequence[] = {&animation_mode_instance, &colors_mode_instance,
                         &marquee_mode_instance};

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