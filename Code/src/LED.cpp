#include <LED.h>

void LED::init()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

LED &LED::setRGB(int r, int g, int b)
{
    leds[0] = CRGB(r, g, b);
    FastLED.show();

    return *this;
}

LED &LED::setBrightness(float duty)
{
    duty = constrain(duty, 0, 1);
    FastLED.setBrightness((__uint8_t)(255 * duty));
    FastLED.show();

    return *this;
}

LED &LED::colorBreath()
{
    for (int i = 0; i < 3; i++)
    {
        if (((rgb[i] + step[i]) < 0) || ((rgb[i] + step[i]) > 255))
            step[i] = -step[i];
        rgb[i] += step[i];
    }

    setRGB(rgb[0], rgb[1], rgb[2]);

    return *this;
}

LED &LED::setColorBreath(__uint8_t r, __uint8_t g,__uint8_t b, int r_step, int g_step, int b_step)
{
    this->rgb[0] = r;
    this->rgb[1] = g;
    this->rgb[2] = b;
    this->step[0] = r_step;
    this->step[1] = g_step;
    this->step[2] = b_step;

    return *this;
}
