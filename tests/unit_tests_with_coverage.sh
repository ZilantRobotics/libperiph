THIS_SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="$(dirname "$THIS_SCRIPT_DIR")"
BUILD_DIR=$REPO_DIR/build/tests

set -e

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DCOVERAGE=1 ../../tests
make

$BUILD_DIR/bmp280
$BUILD_DIR/esc_flame
$BUILD_DIR/ublox
$BUILD_DIR/ms4525do
$BUILD_DIR/stm32_temp
$BUILD_DIR/hmc5883l
$BUILD_DIR/rm3100
$BUILD_DIR/ttl
$BUILD_DIR/servo
$BUILD_DIR/servo_common
$BUILD_DIR/tf_luna
$BUILD_DIR/ws2812

echo "Part 1:--------------------------------------------------"
gcov $BUILD_DIR/CMakeFiles/bmp280.dir$REPO_DIR/sensors/barometer/*.gcda \
     $BUILD_DIR/CMakeFiles/ublox.dir$REPO_DIR/sensors/gps/*.gcda \
     $BUILD_DIR/CMakeFiles/esc_flame.dir$REPO_DIR/sensors/esc/*.gcda \
     $BUILD_DIR/CMakeFiles/hmc5883l.dir$REPO_DIR/sensors/magnetometer/hmc5883l*.gcda \
     $BUILD_DIR/CMakeFiles/rm3100.dir$REPO_DIR/sensors/magnetometer/rm3100*.gcda \
     $BUILD_DIR/CMakeFiles/stm32_temp.dir$REPO_DIR/sensors/temperature_sensor/*.gcda \
     $BUILD_DIR/CMakeFiles/ttl.dir$REPO_DIR/devices/servos/ttl*.gcda \
     $BUILD_DIR/CMakeFiles/servo.dir$REPO_DIR/devices/servos/servo*.gcda \
     $BUILD_DIR/CMakeFiles/servo_common.dir$REPO_DIR/devices/servos/common*.gcda

echo ""
echo "Part 2:--------------------------------------------------"
gcov $BUILD_DIR/CMakeFiles/ms4525do.dir$REPO_DIR/sensors/differential_pressure/ms4525do*.gcda \
     $BUILD_DIR/CMakeFiles/ws2812.dir$REPO_DIR/devices/rgb_leds/ws2812*.gcda \
     $BUILD_DIR/CMakeFiles/tf_luna.dir$REPO_DIR/sensors/rangefinder/tf_luna/*.gcda
