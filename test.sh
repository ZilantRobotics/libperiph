#!/bin/bash
CC=arm-none-eabi-g++
INC="-Iperiphery/ -Idevices/ -Isensors/ -Icommon/"

$CC -c devices/esp8266.cpp $INC -o build/esp8266.o
$CC -c devices/leds_rotation.cpp $INC -o build/leds_rotation.o
$CC -c devices/motor.cpp $INC -o build/motor.o
$CC -c devices/rangefinder.c $INC -o build/rangefinder.o
$CC -c devices/seven_segments_indicators.cpp $INC -o build/seven_segments_indicators.o

# $CC -c sensors/encoder.cpp $INC -o build/encoder.o
$CC -c sensors/temperature_sensor.cpp $INC -o build/temperature_sensor.o
