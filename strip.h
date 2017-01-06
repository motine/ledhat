#ifndef STRIP_H
#define STRIP_H

#define HAT_WIDTH 36
#define HAT_HEIGHT 8
#define STRIP_WIDTH (HAT_HEIGHT * HAT_WIDTH)
#define STRIP_PIN 6

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(STRIP_WIDTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

#endif