# Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

COMMON_INCLUDES=-I ../sensors/
COMMON_LIBS=-lpthread -pthread -lgtest
FLAGS=-Wall -std=c++11
GXX_COMPILE=g++ $(COMMON_INCLUDES) $(FLAGS)  -o shell 

SRC_UBLOX_TEST=test_ublox.cpp                   		../sensors/ublox.c
SRC_ESC_FLAME_TEST=test_esc_flame.cpp           		../sensors/esc_flame.c
SRC_MS4525DO_TEST=test_ms4525do.cpp             		../sensors/ms4525do.c
SRC_STM32_TEMPERATURE_TEST=test_stm32_temperature.cpp 	../sensors/temperature_sensor.c
SRC_BMP280_TEST=test_bmp280.cpp             			../sensors/bmp280.c hal_i2c_mock.cpp

all: esc_flame_test ublox_test ms4525do_test stm32_temperature_test

esc_flame_test:
	$(GXX_COMPILE) $(SRC_ESC_FLAME_TEST) $(COMMON_LIBS)
	./shell

ublox_test:
	$(GXX_COMPILE) $(SRC_UBLOX_TEST) $(COMMON_LIBS)
	./shell

ms4525do_test:
	$(GXX_COMPILE) $(SRC_MS4525DO_TEST) $(COMMON_LIBS)
	./shell

stm32_temperature_test:
	$(GXX_COMPILE) $(SRC_STM32_TEMPERATURE_TEST) $(COMMON_LIBS)
	./shell

bmp280_test:
	$(GXX_COMPILE) $(SRC_BMP280_TEST) $(COMMON_LIBS)
	./shell