# Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.15.3)
project(tests)

if(COVERAGE)
    message(STATUS "Code coverage enabled!")
    add_compile_options(-g -O0 --coverage )
    add_link_options(--coverage)
endif()

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(.)
set(LIBPERIPH_PLATFORM dummy)

function(gen_test app_name test_file cmake_path)
    include(${cmake_path})
    add_executable(${app_name} ${test_file} ${DRIVERS_SOURCE} ${PERIPHERY_SOURCE})
    target_link_libraries(${app_name} gtest pthread gtest_main)
endfunction()

gen_test(bmp280         sensors/test_bmp280.cpp             ../sensors/barometer/CMakeLists.txt)
gen_test(esc_flame      test_esc_flame                      ../sensors/esc/CMakeLists.txt)
gen_test(ublox          test_ublox.cpp                      ../sensors/gps/CMakeLists.txt)
gen_test(ms4525do       test_ms4525do.cpp                   ../sensors/differential_pressure/CMakeLists.txt)
gen_test(stm32_temp     test_stm32_temperature.cpp          ../sensors/temperature_sensor/CMakeLists.txt)
gen_test(hmc5883l       test_hmc5883l.cpp                   ../sensors/magnetometer/CMakeLists.txt)
gen_test(rm3100         test_rm3100.cpp                     ../sensors/magnetometer/CMakeLists.txt)
gen_test(ttl            devices/servos/test_ttl.cpp         ../devices/servos/CMakeLists.txt)
gen_test(servo          devices/servos/test_servo.cpp       ../devices/servos/CMakeLists.txt)
gen_test(servo_common   devices/servos/test_common.cpp      ../devices/servos/CMakeLists.txt)
gen_test(tf_luna        rangefinder/tf_luna.cpp             ../sensors/rangefinder/tf_luna/CMakeLists.txt)
gen_test(ws2812         devices/rgb_leds/test_ws2812.cpp    ../devices/rgb_leds/CMakeLists.txt)
gen_test(ring_buffer    test_ring_buffer.cpp                ../common/CMakeLists.txt)
