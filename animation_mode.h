#ifndef ANIMATION_MODE_H
#define ANIMATION_MODE_H

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

#endif
