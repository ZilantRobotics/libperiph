#include <thread>
#include <iostream>
#include <cstring>
#include <chrono>
#include "gps/ublox.h"
#include "gps/ublox_emulation.h"

using namespace std::chrono_literals;

uint8_t ubuntu_uart_last_recv[2] = {199, 199};
extern uint8_t gnss_buffer[200];

void create_ubx_nav_pvt_package(UbxNavPvtRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_PVT;
    buffer.payload_length = sizeof(UbxNavPvt_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

void GnssSitlStartTask() {
    std::cout << "GNSS SITL mode has been activated!" << std::endl;
    size_t counter = 199;

    // Alaska
    UbxNavPvtRaw_t ubx_nav_pvt_raw = {
        .payload = {.lon = int32_t(-1511453160), .lat = int32_t(596176930), .hMSL = 48000}
    };
    create_ubx_nav_pvt_package(ubx_nav_pvt_raw);

    memcpy(gnss_buffer, (uint8_t*)&ubx_nav_pvt_raw, 100);
    memcpy(gnss_buffer + 100, (uint8_t*)&ubx_nav_pvt_raw, 100);

    auto next_awake = std::chrono::steady_clock::now() + 1ms;
    while (true) {
        ubuntu_uart_last_recv[0] = counter % 200;
        counter++;
        std::this_thread::sleep_until(next_awake);
        next_awake += 1ms;
    }
}
