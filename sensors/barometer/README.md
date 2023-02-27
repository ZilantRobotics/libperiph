# Barometer bmp280 driver

## Usage example:

```c++
...
#include "bmp280.h"
...

void application_init() {
    ...
    bmp280Init();

    if (bmp280IsInitialized()) {
        bmp280Calibrate();
    }
    ...
}

void application_collect_and_parse() {
    bmp280CollectData();
    bmp280ParseData();

    float pressure = bmp280GetStaticPressure();
    float temperature = bmp280GetStaticTemperature();
}

```
