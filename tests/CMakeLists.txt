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
set(LIBPERIPH_PLATFORM ubuntu)

function(gen_test app_name test_file cmake_path)
    include(${cmake_path})
    include_directories(
        ${LIBPERIPH_PERIPHERY_HEADERS}
        ${LIBPERIPH_DRIVERS_HEADERS}
        ${LIBPERIPH_SENSORS_HEADERS}
    )
    add_executable(${app_name}
        ${test_file}
        ${LIBPERIPH_DRIVERS_SOURCE}
        ${LIBPERIPH_PERIPHERY_SOURCE}
    )
    target_link_libraries(${app_name} gtest pthread gtest_main)
endfunction()

add_definitions( -DLIBPERIPH_UNIT_TESTS=1 )

gen_test(ring_buffer    common/test_ring_buffer.cpp             ../common/CMakeLists.txt)

gen_test(ttl            devices/servos/test_ttl.cpp             ../devices/servos/CMakeLists.txt)
gen_test(servo          devices/servos/test_servo.cpp           ../devices/servos/CMakeLists.txt)
gen_test(servo_common   devices/servos/test_common.cpp          ../devices/servos/CMakeLists.txt)
gen_test(ws2812         devices/rgb_leds/test_ws2812.cpp        ../devices/rgb_leds/CMakeLists.txt)

gen_test(acs712         sensors/current_sensor/test_acs712.cpp  ../sensors/current_sensor/CMakeLists.txt)
gen_test(esc_alpha      sensors/esc/test_esc_alpha.cpp          ../sensors/esc/CMakeLists.txt)
gen_test(esc_flame      sensors/esc/test_esc_flame.cpp          ../sensors/esc/CMakeLists.txt)
gen_test(esc_thunder    sensors/esc/test_esc_thunder.cpp        ../sensors/esc/CMakeLists.txt)
gen_test(ublox          sensors/gnss_ublox/test_ublox.cpp       ../sensors/gps/CMakeLists.txt)
gen_test(ublox_commands sensors/gnss_ublox/test_ublox_commands.cpp   ../sensors/gps/CMakeLists.txt)
gen_test(ms4525do       sensors/dprs/test_ms4525do.cpp          ../sensors/differential_pressure/CMakeLists.txt)
gen_test(stm32_temp     sensors/temperature_sensor/test.cpp     ../sensors/temperature_sensor/CMakeLists.txt)
gen_test(hmc5883l       sensors/mag/test_hmc5883l.cpp           ../sensors/magnetometer/CMakeLists.txt)
gen_test(rm3100         sensors/mag/test_rm3100.cpp             ../sensors/magnetometer/CMakeLists.txt)
gen_test(tf_mini        sensors/rangefinder/test_tf_mini.cpp    ../sensors/rangefinder/tf_mini/CMakeLists.txt)
gen_test(garmin_lite    sensors/rangefinder/test_garmin_lite.cpp ../sensors/rangefinder/garmin_lite/CMakeLists.txt)
gen_test(bmp280         sensors/test_bmp280.cpp                 ../sensors/barometer/CMakeLists.txt)

gen_test(adc            periphery/test_adc.cpp                  ../platform_specific/hal_adc/CMakeLists.txt)
gen_test(i2c            periphery/test_i2c.cpp                  ../platform_specific/hal_i2c/CMakeLists.txt)
gen_test(spi            periphery/test_spi.cpp                  ../platform_specific/hal_spi/CMakeLists.txt)
gen_test(timers         periphery/test_timers.cpp               ../platform_specific/hal_timers/CMakeLists.txt)
gen_test(uart           periphery/test_uart.cpp                 ../platform_specific/hal_uart/CMakeLists.txt)
