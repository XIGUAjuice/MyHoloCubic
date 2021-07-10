#ifndef LED_H
#define LED_H
#include <FastLED.h>

#define NUM_LEDS 1
#define LED_PIN 27

class LED
{
private:
    CRGB leds[NUM_LEDS];
    __uint8_t rgb[3];
    int step[3];

public:
    void init();
    LED &setRGB(int r, int g, int b);
    LED &setBrightness(float duty);
    LED &setColorBreath(__uint8_t r, __uint8_t g,__uint8_t b, int r_step, int g_step, int b_step);
    LED &colorBreath();
};

#endif