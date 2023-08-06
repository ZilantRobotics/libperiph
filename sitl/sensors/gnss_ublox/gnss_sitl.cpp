#include <thread>
#include <iostream>
#include <cstring>
#include <chrono>
#include "gps/ublox.h"
#include "gps/ublox_emulation.h"

using namespace std::chrono_literals;

#define GNSS_BUFFER_SIZE 200
uint8_t ubuntu_uart_last_recv[2] = {199, 199};
extern uint8_t gnss_buffer[GNSS_BUFFER_SIZE];

void create_ubx_nav_pvt_package(UbxNavPvtRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_PVT;
    buffer.payload_length = sizeof(UbxNavPvt_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

void create_ubx_nav_cov_package(UbxNavCovRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_COV;
    buffer.payload_length = sizeof(UbxNavCov_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

void create_ubx_nav_status_package(UbxNavStatusRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_STATUS;
    buffer.payload_length = sizeof(UbxNavStatus_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

struct UbloxBuffer {
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    UbxNavCovRaw_t ubx_nav_cov_raw;
    UbxNavStatusRaw_t ubx_nav_status_raw;

    static constexpr const size_t SIZE = sizeof(UbxNavPvtRaw_t) + sizeof(UbxNavCovRaw_t) + sizeof(UbxNavStatusRaw_t);

    void construct() {
        create_ubx_nav_pvt_package(ubx_nav_pvt_raw);
        create_ubx_nav_cov_package(ubx_nav_cov_raw);
        create_ubx_nav_status_package(ubx_nav_status_raw);
    }
};

void GnssSitlStartTask() {
    std::cout << "GNSS SITL mode has been activated!" << std::endl;
    size_t counter = 199;

    UbloxBuffer buffer;

    // Alaska coordinates
    buffer.ubx_nav_pvt_raw.payload.lon = -1511453160;
    buffer.ubx_nav_pvt_raw.payload.lat = 596176930;
    buffer.ubx_nav_pvt_raw.payload.hMSL = 48000;

    buffer.ubx_nav_cov_raw.payload.posCovNN = 1.0;
    buffer.ubx_nav_cov_raw.payload.posCovEE = 0.99;
    buffer.ubx_nav_cov_raw.payload.posCovDD = 1.01;
    buffer.ubx_nav_cov_raw.payload.velCovNN = 0.50;
    buffer.ubx_nav_cov_raw.payload.velCovEE = 0.49;
    buffer.ubx_nav_cov_raw.payload.velCovDD = 0.51;

    buffer.ubx_nav_status_raw.payload.flags2.spoofDetState = NO_SPOOFING_INDICATED;

    buffer.construct();

    auto next_awake = std::chrono::steady_clock::now() + 1ms;
    while (true) {
        size_t buffer_idx = counter % GNSS_BUFFER_SIZE;
        gnss_buffer[buffer_idx] = ((uint8_t*)&buffer)[counter % UbloxBuffer::SIZE];
        ubuntu_uart_last_recv[0] = buffer_idx;
        counter++;
        std::this_thread::sleep_until(next_awake);
        next_awake += 1ms;
    }
}
