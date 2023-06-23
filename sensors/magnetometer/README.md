# Magnetometer driver

Here are drivers for 2 magnetometers:
- hmc588l ([datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Magneto/HMC5883L-FDS.pdf)),
- rm3100 ([datasheet](https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FR%252FRM3100.pdf)).

## hmc5883l usage example

hmc5883l magnetometer is based on I2C driver.

```c++
// application initialization
float x, y, z;
i2cInit();
hmc5883Init();

// process periodically
hmc5883lMeasure();
hmc5883GetMeasurement(&x, &y, &z);
```

## rm3100 usage example

rm3100 magnetometer is based on SPI driver.

```c++
// application initialization
float x, y, z;
spiInit();
rm3100Init();

// process periodically
rm3100Measure();
rm3100GetMeasurement(&x, &y, &z);
```

## Tests

- [tests/sensors/test_hmc5883l.cpp](../../tests/test_hmc5883l.cpp)
- [tests/sensors/test_rm3100.cpp](../../tests/test_rm3100.cpp)
