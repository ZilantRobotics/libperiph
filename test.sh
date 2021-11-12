#!/bin/bash
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
FLAGS="-c --specs=nosys.specs"
INC="-Idevices/ -Isensors/ -Icommon/"

$CXX $FLAGS common/soft_timer.cpp                   $INC -o build/soft_timer.o
$CXX $FLAGS devices/esp8266.cpp                     $INC -o build/esp8266.o
$CXX $FLAGS devices/seven_segments_indicators.cpp   $INC -o build/seven_segments_indicators.o
$CXX $FLAGS sensors/encoder.cpp                     $INC -o build/encoder.o

$CC $FLAGS devices/rangefinder.c                    $INC -o build/rangefinder.o
# $CC $FLAGS devices/ws2812.c                         $INC -o build/ws2812.o
$CC $FLAGS sensors/temperature_sensor.c             $INC -o build/temperature_sensor.o
$CC $FLAGS sensors/hmc5883l.c                       $INC -o build/hmc5883l.o
$CC $FLAGS sensors/ublox.c                          $INC -o build/ublox.o
$CC $FLAGS sensors/mpu9250.c                        $INC -o build/mpu9250.o
$CC $FLAGS sensors/rangesensor_sf1xx.c              $INC -o build/rangesensor_sf1xx.o
$CC $FLAGS sensors/bmp280.c                         $INC -o build/bmp280.o
$CC $FLAGS sensors/ms4525do.c                       $INC -o build/ms4525do.o
$CC $FLAGS sensors/rm3100.c                         $INC -o build/rm3100.o
$CC $FLAGS sensors/rangefinder_garmin_lite.c        $INC -o build/rangefinder_garmin_lite.o
$CC $FLAGS sensors/esc_flame.c                      $INC -o build/esc_flame.o
$CC $FLAGS sensors/rangesensor_tf_luna.c            $INC -o build/rangesensor_tf_luna.o

# depend on hal
# $CC -c devices/motors.cpp                       $INC -o build/motors.o
