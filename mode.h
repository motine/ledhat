#ifndef MODE_H
#define MODE_H

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

#endif
