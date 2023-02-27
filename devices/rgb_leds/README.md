# ws2812 driver

## Prerequisites

Expected STM32CubeMX configuration:

1. Let's say you use timer 2, channel 3:

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/ws2812/tim_config.png?raw=true)

2. Select prescaler and counter period. For 72 MHz clock, this should be 0 and 90 respectively.

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/ws2812/tim_config_parameters.png?raw=true)

3. Enable DMA memory to peripheral:

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/ws2812/tim_config_dma.png?raw=true)

4. Enable global interrupt.

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/ws2812/tim_config_nvic.png?raw=true)

## Usage example:

```c++
...
#include "rgb_color.h"
static Leds_Color_t leds;
...

void application_init() {
    ...
    ws2812bInit(3, &htim2, TIM_CHANNEL_3);
    ...
}

void application_set_red_blue_purple_colors() {
    // Set predefined Red color for the first LED
    rgbLedSetValue(&leds.colors[0], RGB_LED_RED_COLOR);

    // Set predefined Blue color for the second LED
    rgbLedSetValue(&leds.colors[1], RGB_LED_BLUE_COLOR);

    // Set custom color for the third LED
    leds.colors[2].shades.blue = 255;
    leds.colors[2].shades.green = 0;
    leds.colors[2].shades.red = 255;

    // Serialize colors to the DMA buffer and start it
    ws2812bSetColors(&leds);
    ws2812bStartOnce();
}

```

## Implementation details

Reference: [WS2812B datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)

Required config for timer:
- timer freq = 72 000 000
- timer prescaller = 0
- pwm period or 1 bit = 90 timer ticks = 1.25 us:
-    transfer bit=1 => T1H = 0.8 us => 57.6 ticks
-    transfer bit=0 => T0H = 0.4 us => 28.8 ticks

Performance:

| Number of LEDs | Bits required | Duration |
| -------------- | ------------- | -------- |
| 1              | 24            | 30 us    |
| 3              | 72            | 90 us    |
| 10             | 230           | 0.3 ms   |
| 15             | 345           | 0.45 ms  |
| 32             | 768           | 0.96 ms  |
| 216            | 5184          | 6.480 ms |

RAM requirements:
- LED's color buffer requires 3 bytes for each LED.
- DMA serialization buffer requires 24 bytes for each LED.
