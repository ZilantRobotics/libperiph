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
$CC $FLAGS sensors/temperature_sensor.c             $INC -o build/temperature_sensor.o
$CC $FLAGS sensors/hmc5883l.c                       $INC -o build/hmc5883l.o
$CC $FLAGS sensors/ublox.c                          $INC -o build/ublox.o

# depend on hal
# $CC -c devices/motors.cpp                       $INC -o build/motors.o
