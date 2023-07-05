# Current sensor ACS712

Fully Integrated, Hall Effect-Based Linear Current Sensor
with 2.1 kVRMS Voltage Isolation and a Low-Resistance Current Conductor.

Datasheet: https://www.tme.eu/Document/75af175f80c090e9b8f9078a0b0b2409/ACS712.PDF

Current sensor driver expected to have ADC.

## Usage example

```c++
uint16_t raw_current_adc; // get raw adc value
float current_ampers = acs712ConvertToCurrent(raw_current_adc);
```
