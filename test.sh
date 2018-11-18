#!/bin/bash
CC=arm-none-eabi-g++
INC="-Iperiphery/ -Idevices/ -Isensors/ -Icommon/"

$CC -c devices/esp8266.cpp $INC -o build/esp8266.o
$CC -c devices/motor.cpp $INC -o build/motor.o
$CC -c devices/rangefinder.c $INC -o build/rangefinder.o
$CC -c devices/seven_segments_indicators.cpp $INC -o build/seven_segments_indicators.o

# $CC -c sensors/encoder.cpp $INC -o build/encoder.o
$CC -c sensors/temperature_sensor.c $INC -o build/temperature_sensor.o
$CC -c sensors/hmc5883l.c $INC -o build/hmc5883l.o
