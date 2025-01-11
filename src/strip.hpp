
#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B_1 15
#define PIN_WS2812B_2 22
#define NUM_PIXELS_1 25
#define NUM_PIXELS_2 27


Adafruit_NeoPixel ws2812b1(NUM_PIXELS_1, PIN_WS2812B_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ws2812b2(NUM_PIXELS_2, PIN_WS2812B_2, NEO_GRB + NEO_KHZ800);

// Helper function to convert HSV to RGB (values in range 0..1).
// Adapted to store results in floating-point pointers r, g, b.
static void hsvToRgb(float h, float s, float v, float *r, float *g, float *b) {
    // Ensure hue is in [0..360)
    h = fmodf(h, 360.0f);
    if (h < 0.0f) {
        h += 360.0f;
    }

    float c = v * s;                   // Chroma
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float rf, gf, bf;
    if      (h < 60)  { rf = c;  gf = x;  bf = 0;  }
    else if (h < 120) { rf = x;  gf = c;  bf = 0;  }
    else if (h < 180) { rf = 0;  gf = c;  bf = x;  }
    else if (h < 240) { rf = 0;  gf = x;  bf = c;  }
    else if (h < 300) { rf = x;  gf = 0;  bf = c;  }
    else              { rf = c;  gf = 0;  bf = x;  }

    *r = rf + m;
    *g = gf + m;
    *b = bf + m;
}

/**
 * @brief Get a color (in R, G, B) that rotates through the full hue range
 *        over a 10-second period, with maximum brightness = 128 (half of 256).
 *
 * @param ms   Current time in milliseconds
 * @param rOut Pointer to store the resulting red value (0..128)
 * @param gOut Pointer to store the resulting green value (0..128)
 * @param bOut Pointer to store the resulting blue value (0..128)
 */
void getColorForTime(uint32_t ms, uint8_t *rOut, uint8_t *gOut, uint8_t *bOut)
{
    // 10 seconds = 10,000 ms for a full hue rotation (0..360).
    // Hue will cycle from 0..360 every 10 seconds, then wrap.
    float hue = fmodf(((float)ms / 10000.0f) * 360.0f, 360.0f);

    // We want maximum brightness to be 128 instead of 255,
    // so effectively we can use HSV with v=1.0, then scale the RGB by 0.5.
    // Alternatively, we can just set v=1.0 and multiply final R,G,B by 0.5.
    
    float s = 1.0f;     // full saturation
    float v = 1.0f;     // full "value" (brightness)
    
    float rF, gF, bF;
    hsvToRgb(hue, s, v, &rF, &gF, &bF);   // rF, gF, bF in [0..1]

    // Scale down brightness to half (0..0.5 range),
    // so final max is around 128 instead of 255.
    rF *= 0.1f;
    gF *= 0.1f;
    bF *= 0.1f;

    // Convert floats [0..0.5] to integers [0..128].
    *rOut = (uint8_t)(rF * 255.0f + 0.5f); // Round
    *gOut = (uint8_t)(gF * 255.0f + 0.5f);
    *bOut = (uint8_t)(bF * 255.0f + 0.5f);
}

void setupLedStrip() {
  ws2812b1.begin(); 
  ws2812b2.begin(); 
}

void updateLedStrip() {
  for (int pixel = 0; pixel < NUM_PIXELS_1; pixel++) {       
    int time = millis();
    uint8_t r, g, b;
    getColorForTime(time, &r, &g, &b);
    ws2812b1.setPixelColor(pixel, ws2812b1.Color(r, g, b)); 
  }
  ws2812b1.show();                                         
  for (int pixel = 0; pixel < NUM_PIXELS_2; pixel++) {       
    int time = millis();
    uint8_t r, g, b;
    getColorForTime(time, &r, &g, &b);
    ws2812b2.setPixelColor(pixel, ws2812b2.Color(r, g, b)); 
  }
  ws2812b2.show();                                         
}
