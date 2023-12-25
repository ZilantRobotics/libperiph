# Rangefinders

Here we have a few rangefidners drivers.

| Sensor | [LW20/C](https://www.mouser.com/datasheet/2/321/28055-LW20-SF20-LiDAR-Manual-Rev-7-1371848.pdf) | [TF-Luna](https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf) | [Garmin Lite V3](https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf) | [vl53l0x](https://www.st.com/resource/en/datasheet/vl53l0x.pdf) |
| ------ | ------- | ------- | -------- | -------- |
| View | ![](https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/lw20.jpg?raw=true "lw20") | ![](https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/tf_mini.jpg?raw=true "Mini v1") | ![](https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/garmin_lite_v3.jpg?raw=true "Garmin Lite V3") | ![](https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/vl53l0x.jpg?raw=true "vl53l0x") |
| Interface | i2c | uart | i2c | i2c |

An i2c rangefinder has the following interface:

```c++
/**
  * @brief Initialize internal variables, perform calibration or something else
  */
int8_t <name>Init();

/**
  * Collect data from i2c and parse them
  * CollectData is a blocking function. It reads data from I2C and save it to the internal buffer.
  * It returns LIBPERIPH_OK on success and < 0 on failure.
  * ParseCollectedData parses the received buffer and returns the distance in meters.
  */
int8_t <name>CollectData();
float <name>ParseCollectedData();
```
