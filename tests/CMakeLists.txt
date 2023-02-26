# Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.15.3)
project(tests)

if(COVERAGE)
	message(STATUS "Code coverage enabled!")
	add_compile_options(-g -O0 --coverage)
	add_link_options(--coverage)
endif()

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

include_directories(.)
include_directories(../common/)
include_directories(../devices/)
include_directories(../sensors/)
include_directories(../platform_specific/hal_adc)
include_directories(../platform_specific/hal_i2c)
include_directories(../platform_specific/hal_spi)
include_directories(../platform_specific/hal_timers)
include_directories(../platform_specific/hal_uart)

function(gen_test app_name src_files)
	add_executable(${app_name} ${src_files})
	target_link_libraries(${app_name} gtest gtest_main)
endfunction()

gen_test(bmp280				"test_bmp280.cpp;../sensors/barometer/bmp280.c;../platform_specific/hal_i2c/dummy/hal_i2c.c")
gen_test(esc_flame			"test_esc_flame.cpp;../sensors/esc/flame.c")
gen_test(ublox				"test_ublox.cpp;../sensors/gps/ublox.c;../sensors/gps/ublox_emulation.c")
gen_test(ms4525do			"test_ms4525do.cpp;../sensors/differential_pressure/ms4525do.c;../platform_specific/hal_i2c/dummy/hal_i2c.c")
gen_test(stm32_temperature 	"test_stm32_temperature.cpp;../sensors/temperature_sensor/stm32_temperature.c")
gen_test(hmc5883l 			"test_hmc5883l.cpp;../sensors/magnetometer/hmc5883l.c;../platform_specific/hal_i2c/dummy/hal_i2c.c")
gen_test(rm3100 			"test_rm3100.cpp;../sensors/magnetometer/rm3100.c;../platform_specific/hal_spi/dummy/hal_spi.c")
gen_test(ttl 				"test_ttl.cpp;../common/ttl.c")
gen_test(servo 				"test_servo.cpp;../devices/servos.c;../common/ttl.c;../platform_specific/hal_timers/dummy/hal_timers.c")
gen_test(tf_luna			"rangefinder/tf_luna.cpp;../sensors/rangefinder/tf_luna/tf_luna.c")
gen_test(ws2812				"devices/rgb_leds/test_ws2812.cpp;../devices/rgb_leds/ws2812.c")
