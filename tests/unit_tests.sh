#!/bin/bash
THIS_SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="$(dirname "$THIS_SCRIPT_DIR")"
BUILD_DIR=$REPO_DIR/build/tests

set -e
if [[ $1 == "--coverage" ]]; then
  RUN_COVERAGE="-DCOVERAGE=1"
else
  RUN_COVERAGE=""
fi

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake $RUN_COVERAGE ../../tests
make

sensor_executables=(acs712 bmp280 esc_flame esc_thunder ublox ublox_commands ms4525do stm32_temp hmc5883l rm3100 tf_luna)
for sensor_executable in ${sensor_executables[@]}; do
    $BUILD_DIR/$sensor_executable
done

driver_executables=(ttl servo servo_common ws2812 ring_buffer)
for driver_executable in ${driver_executables[@]}; do
    $BUILD_DIR/$driver_executable
done

hal_executables=(adc i2c spi timers uart)
for hal_executable in ${hal_executables[@]}; do
    $BUILD_DIR/$hal_executable
done

if [[ $1 == "--coverage" ]]; then
echo "Part 1:--------------------------------------------------"
gcov $BUILD_DIR/CMakeFiles/acs712.dir$REPO_DIR/sensors/current_sensor/*.gcda \
     $BUILD_DIR/CMakeFiles/bmp280.dir$REPO_DIR/sensors/barometer/*.gcda \
     $BUILD_DIR/CMakeFiles/ublox.dir$REPO_DIR/sensors/gps/ublox.*.gcda \
     $BUILD_DIR/CMakeFiles/ublox_commands.dir$REPO_DIR/sensors/gps/ublox_commands.*.gcda \
     $BUILD_DIR/CMakeFiles/hmc5883l.dir$REPO_DIR/sensors/magnetometer/hmc5883l*.gcda \
     $BUILD_DIR/CMakeFiles/rm3100.dir$REPO_DIR/sensors/magnetometer/rm3100*.gcda \
     $BUILD_DIR/CMakeFiles/stm32_temp.dir$REPO_DIR/sensors/temperature_sensor/*.gcda \
     $BUILD_DIR/CMakeFiles/ttl.dir$REPO_DIR/devices/servos/ttl*.gcda \
     $BUILD_DIR/CMakeFiles/servo.dir$REPO_DIR/devices/servos/servo*.gcda \
     $BUILD_DIR/CMakeFiles/servo_common.dir$REPO_DIR/devices/servos/common*.gcda

echo ""
echo "Part 2:--------------------------------------------------"
gcov $BUILD_DIR/CMakeFiles/ms4525do.dir$REPO_DIR/sensors/differential_pressure/ms4525do*.gcda \
     $BUILD_DIR/CMakeFiles/tf_luna.dir$REPO_DIR/sensors/rangefinder/tf_luna/*.gcda
fi

echo ""
echo "Auxilliary:--------------------------------------------------"
gcov $BUILD_DIR/CMakeFiles/ring_buffer.dir$REPO_DIR/common/*.gcda \
     $BUILD_DIR/CMakeFiles/esc_flame.dir$REPO_DIR/sensors/esc/flame*.gcda \
     $BUILD_DIR/CMakeFiles/esc_thunder.dir$REPO_DIR/sensors/esc/thunder*.gcda \
     $BUILD_DIR/CMakeFiles/ws2812.dir$REPO_DIR/devices/rgb_leds/ws2812*.gcda
