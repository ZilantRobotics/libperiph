# Differential pressure sensor MS4525DO

The MS4525DO is a small, ceramic based, PCB mounted pressure transducer
from TE Connectivity.The transducer creates a low cost, high performance
digital output pressure (14bit) and temperature (11bit).

According to MS4525DO datasheet, the sensor has the following range of measured data:

- differential pressure is from -1 psi to +1 psi or from -6894.757 pa to +6894.757 pa.
- temperature is from -50 to +150 Celcius or from 223 to 423 Kelvin.

If we consider temperature ~288 Kelvin and pressure 101325 Pa according to the ISA model differential pressure interval above should be enough for up to 100 m/sec airspeed that is suitable for a wide area of small VTOL application.

|   |   |
| - | - |
| Reference | [Datasheet](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS4525DO%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS4525DO_B2.pdf%7FCAT-BLPS0002) |
| Requred periphery | I2C |
| Unit tests | [test_ms4525do.cpp](../../tests/sensors/dprs/test_ms4525do.cpp) |

## Usage

```c++
ms4525CollectData();
DifferentialPressureData readings = ms4525ParseCollectedData();
```
