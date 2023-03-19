#!/bin/bash
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
FLAGS="-c --specs=nosys.specs"
INC="-Icommon/ \
    -Idevices/ \
    -Isensors/ \
    -Itests/ \
    -Iplatform_specific/hal_i2c"

# Part 1.
$CC $FLAGS sensors/barometer/bmp280.c               $INC -o build/baro_bmp280.o
$CC $FLAGS sensors/gps/ublox.c                      $INC -o build/ublox.o
$CC $FLAGS sensors/magnetometer/hmc5883l.c          $INC -o build/mag_hmc5883l.o
$CC $FLAGS sensors/magnetometer/rm3100.c            $INC -o build/mag_rm3100.o
$CC $FLAGS sensors/temperature_sensor/stm32_temperature.c $INC -o build/stm32_temperature.o
$CC $FLAGS devices/servos/servos.c                  $INC -o build/servo.o
$CC $FLAGS devices/servos/ttl.c                     $INC -o build/ttl.o

# Part 2.
$CC $FLAGS sensors/current_sensor/acs712.c          $INC -o build/acs712.o
$CC $FLAGS sensors/differential_pressure/ms4525do.c $INC -o build/ms4525do.o
# $CC $FLAGS sensors/rangefinder/sf1xx.c              $INC -o build/sf1xx.o
# $CC $FLAGS sensors/rangefinder/garmin_lite.c        $INC -o build/uarmin_lite.o
$CC $FLAGS sensors/rangefinder/tf_luna/tf_luna.c    $INC -o build/tf_luna.o

# Auxiliary
$CXX $FLAGS common/soft_timer.cpp                   $INC -o build/soft_timer.o
$CXX $FLAGS devices/esp8266.cpp                     $INC -o build/esp8266.o
# $CC -c devices/motors.cpp                       $INC -o build/motors.o
# $CC $FLAGS devices/ws2812.c                         $INC -o build/ws2812.o
$CXX $FLAGS devices/seven_segments_indicators.cpp   $INC -o build/seven_segments_indicators.o
$CXX $FLAGS sensors/encoder/encoder.cpp             $INC -o build/encoder.o
$CC $FLAGS sensors/esc/flame.c                      $INC -o build/esc_flame.o
$CC $FLAGS sensors/imu/mpu9250.c                    $INC -o build/mpu9250.o
$CC $FLAGS sensors/rangefinder/srf_05/srf_05.c      $INC -o build/srf_05.o
