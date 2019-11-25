#!/bin/bash
CC=arm-none-eabi-g++
INC="-Idevices/ -Isensors/ -Icommon/"

$CC -c common/soft_timer.cpp                    $INC -o build/soft_timer.o
$CC -c devices/esp8266.cpp                      $INC -o build/esp8266.o
$CC -c devices/rangefinder.c                    $INC -o build/rangefinder.o
$CC -c devices/seven_segments_indicators.cpp    $INC -o build/seven_segments_indicators.o
$CC -c sensors/encoder.cpp                      $INC -o build/encoder.o
$CC -c sensors/temperature_sensor.c             $INC -o build/temperature_sensor.o
$CC -c sensors/hmc5883l.c                       $INC -o build/hmc5883l.o

# depend on hal
# $CC -c devices/motors.cpp                       $INC -o build/motors.o
