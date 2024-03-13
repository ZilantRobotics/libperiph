# Barometer bmp280 driver

Barometer driver is based on i2c driver.

## Usage example

```c++
#include "bmp280.h"

void application_init() {
    if (bmp280Init() < 0) {
        // Handle error. Possible error causes:
        // 1. The sensor doesn't respond on i2c bus
        // 2. DevID register returns wrong value
        // 3. Calibration registers cannot be read successfully
        return;
    }
}

void application_collect_and_parse() {
    BarometerMeasurements data;
    if (bmp280GetData(&data) < 0) {
        // Handle error. Possible error causes:
        // 1. The sensor doesn't respond on i2c bus
        // 2. Measurements doesn't fit a valid range (300 … 1100 hPa, -40 … +85 °C)
        // 3. Stale measurements (the same data 10 times in a row)
        return;
    }

    // Process measured data
}
```

## Tests

[tests/sensors/test_bmp280.cpp](../../tests/sensors/test_bmp280.cpp)
