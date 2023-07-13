/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <thread>
#include <chrono>

#include <string.h>
#include "ring_buffer.h"


// gnss.c
#define GNS_BUFFER_SIZE 256
uint8_t gns_buffer[GNS_BUFFER_SIZE];
static GnssFix2_t uavcan_fix2;


struct TestRingBuffer {
    static void check_result(bool result, bool correct_result) {
        if (result == correct_result) {
            std::cout << "test: ok" << std::endl;
        } else {
            std::cout << "test: failed (" << result << " instead of " << correct_result << ")" << std::endl;
        }
    }

    static void test_compare_full_equal() {
        uint8_t test_buf[GNS_BUFFER_SIZE];
        for (size_t idx = 0; idx < GNS_BUFFER_SIZE; idx++) {
            test_buf[idx] = idx;
        }
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 0), true);
    }
    static void test_compare_full_not_equal() {
        uint8_t test_buf[GNS_BUFFER_SIZE];
        for (size_t idx = 0; idx < GNS_BUFFER_SIZE; idx++) {
            test_buf[idx] = idx;
        }
        test_buf[42] = 0;
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 0), false);
    }
    static void test_compare_full_shifted_equal() {
        uint8_t test_buf[GNS_BUFFER_SIZE];
        for (size_t idx = 0; idx < GNS_BUFFER_SIZE; idx++) {
            test_buf[idx] = uint8_t(10 + idx);
        }
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 10), true);
    }
    static void test_compare_full_shifted_not_equal() {
        uint8_t test_buf[GNS_BUFFER_SIZE];
        for (size_t idx = 0; idx < GNS_BUFFER_SIZE; idx++) {
            test_buf[idx] = uint8_t(9 + idx);
        }
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 10), false);
    }

    static void test_copy_full() {
        uint8_t test_buf[GNS_BUFFER_SIZE] = {0};
        ringBufferCopy(test_buf, 0, GNS_BUFFER_SIZE);
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 0), true);
    }
    static void test_copy_full_shifted() {
        uint8_t test_buf[GNS_BUFFER_SIZE] = {0};
        ringBufferCopy(test_buf, 42, GNS_BUFFER_SIZE);
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE, 42), true);
    }
    static void test_copy_half() {
        uint8_t test_buf[GNS_BUFFER_SIZE / 2] = {0};
        ringBufferCopy(test_buf, 0, GNS_BUFFER_SIZE / 2);
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE / 2, 0), true);
    }
    static void test_copy_half_shifted() {
        uint8_t test_buf[GNS_BUFFER_SIZE / 2] = {0};
        ringBufferCopy(test_buf, 150, GNS_BUFFER_SIZE / 2);
        check_result(ringBufferCompare(test_buf, GNS_BUFFER_SIZE / 2, 150), true);
    }

    static void start() {
        for (size_t idx = 0; idx < GNS_BUFFER_SIZE; idx++) {
            gns_buffer[idx] = idx;
        }
        ringBufferInit(gns_buffer, GNS_BUFFER_SIZE);

        TestRingBuffer::test_compare_full_equal();
        TestRingBuffer::test_compare_full_not_equal();
        TestRingBuffer::test_compare_full_shifted_equal();
        TestRingBuffer::test_compare_full_shifted_not_equal();

        TestRingBuffer::test_copy_full();
        TestRingBuffer::test_copy_full_shifted();
        TestRingBuffer::test_copy_half();
        TestRingBuffer::test_copy_half_shifted();
    }
};

static const std::string GGA_EXAMPLE =
    "$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E\n\r";
struct TestNmeaParser {
    static void get_from_gps(std::string msg) {
        size_t counter = 0;
        while (1) {
            gns_buffer[counter % sizeof(gns_buffer)] = GGA_EXAMPLE[counter % GGA_EXAMPLE.size()];
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter++;
        }
    }
    static void pub_to_stream(std::string msg) {
        while (1) {
            nmeaProcess(gns_buffer, GNS_BUFFER_SIZE, &uavcan_fix2);
            std::cout << "uavcan_fix2.lat/lon/height = {"
                      << uavcan_fix2.longitude_deg_1e8 << ", "
                      << uavcan_fix2.latitude_deg_1e8 << ", "
                      << uavcan_fix2.height_msl_mm << ", "
                      << uavcan_fix2.height_ellipsoid_mm << "}" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }
    static void start() {
        memset(gns_buffer, 0x00, GNS_BUFFER_SIZE);
        std::thread thread1(TestNmeaParser::get_from_gps, "Hello");
        std::thread thread2(TestNmeaParser::pub_to_stream, "Hi");
        thread1.join();
        thread2.join();
    }
};


int main() {
    TestRingBuffer::start();
    TestNmeaParser::start();
}