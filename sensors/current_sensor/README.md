# Allegro Current sensor ACS712 driver

Fully Integrated, Hall Effect-Based Linear Current Sensor with 2.1 kVRMS Voltage Isolation and a
Low-Resistance Current Conductor.

The decices provides economical and precise solutions for AC or DC current sensing in industrial,
commercial, and communications systems. Typical applications include motor control, load detection
and management, switched-mode power supplies, and overcurrent fault protection.

The device consists of a precise, low-offset, linear Hall sensor circuit with a copper conduction
path located near the surface of the die. Applied current flowing through this copper conduction
path generates a magnetic field which is sensed by the integrated Hall IC and converted into a
proportional voltage. Device accuracy is optimized through the close proximity of the magnetic
signal to the Hall transducer.

<img src="https://sigma.octopart.com/cdn-cgi/image/width=75,height=75,fit=contain/145603129/image/Allegro-MicroSystems-LLC-ACS712ELCTR-20A-T.jpg" alt="drawing" width="64">

|   |   |
| - | - |
| Reference | [Datasheet](https://www.tme.eu/Document/75af175f80c090e9b8f9078a0b0b2409/ACS712.PDF)|
| Requred periphery | ADC |
| Unit tests | [test_acs712.cpp](tests/sensors/current_sensor/test_acs712.cpp) |

## Usage

Installation:
1. Include the header file `tf_luna.h` and source file `tf_luna.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other
libraries specific to your STM32 development environment.

Usage:
1. Initialize ADC
2. Initialize the sensor calling `acs712SetCoefficient` with the coefficent corresponded to
sensitivity of your sensor
3. Either perform calibration of the sensor by obtaing ADC values and calling
`acs712PerformIterationOfCalibration` or set by manually setting the calibration value
`acs712SetCalibratedValue`.
4. Obrain ADC value and convert it to the current by calling `acs712ConvertToCurrent`

> Note: Ensure that you have connected the ACS712 sensor properly to the MCU, and provide
appropriate power and reference voltage according to the specifications of the ACS712 model being
used.

Here are some key specifications to consider:

1. Sensitivity: The sensitivity of the ACS712 sensor determines how much voltage change occurs
across the sense resistor for a given current. Common sensitivity values are 20mV/A, 30mV/A, and
5A/V. Choose a sensitivity value that matches your expected current range and accuracy
requirements.
2. Measuring Range: The measuring range specifies the maximum current that can be accurately
measured by the ACS712 sensor. The range typically varies from a few amperes up to 20 or 30
amperes, depending on the model. Ensure that the selected ACS712 model covers the expected current
range in your application.
3. Output: The ACS712 sensor provides an analog output voltage proportional to the sensed current.
The exact voltage range depends on the model and sensitivity chosen. It is important to note the
voltage range and ensure that it is compatible with the ADC input of your STM32 microcontroller.

Consider these specifications when choosing the appropriate ACS712 model for your project, ensuring
that it meets your required sensitivity, measuring range, and compatibility with your STM32
microcontroller's ADC capabilities.
