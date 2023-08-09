#include <thread>
#include <iostream>
#include <cstring>
#include <chrono>
#include "hal_uart.h"
#include "gps/ublox.h"
#include "gps/ublox_emulation.h"

using namespace std::chrono_literals;

#define GNSS_BUFFER_SIZE 200
extern uint8_t gnss_buffer[GNSS_BUFFER_SIZE];

struct UbloxBuffer {
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    UbxNavCovRaw_t ubx_nav_cov_raw;
    UbxNavStatusRaw_t ubx_nav_status_raw;

    static constexpr const size_t SIZE = sizeof(UbxNavPvtRaw_t) + sizeof(UbxNavCovRaw_t) + sizeof(UbxNavStatusRaw_t);

    void construct();

private:
    void create_ubx_nav_pvt_package(UbxNavPvtRaw_t& buffer);
    void create_ubx_nav_cov_package(UbxNavCovRaw_t& buffer);
    void create_ubx_nav_status_package(UbxNavStatusRaw_t& buffer);
};

void UbloxBuffer::construct() {
    create_ubx_nav_pvt_package(ubx_nav_pvt_raw);
    create_ubx_nav_cov_package(ubx_nav_cov_raw);
    create_ubx_nav_status_package(ubx_nav_status_raw);
}

void UbloxBuffer::create_ubx_nav_pvt_package(UbxNavPvtRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_PVT;
    buffer.payload_length = sizeof(UbxNavPvt_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

void UbloxBuffer::create_ubx_nav_cov_package(UbxNavCovRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_COV;
    buffer.payload_length = sizeof(UbxNavCov_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}

void UbloxBuffer::create_ubx_nav_status_package(UbxNavStatusRaw_t& buffer) {
    buffer.sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer.sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer.class_nav = CLASS_NAV;
    buffer.id_nav_pvt = ID_NAV_STATUS;
    buffer.payload_length = sizeof(UbxNavStatus_t);
    buffer.crc = ubloxCrc16(&buffer.class_nav, sizeof(buffer) - 4);
}


void GnssSitlStartTask() {
    std::cout << "GNSS SITL mode has been activated!" << std::endl;
    const auto init_time = std::chrono::steady_clock::now();
    size_t ubx_byte_counter = GNSS_BUFFER_SIZE - 1;

    UbloxBuffer buffer;

    // Alaska coordinates
    buffer.ubx_nav_pvt_raw.payload.lon = -1511453160;
    buffer.ubx_nav_pvt_raw.payload.lat = 596176930;
    buffer.ubx_nav_pvt_raw.payload.hMSL = 48000;

    buffer.ubx_nav_cov_raw.payload.pos.cov.nn = 1.01;
    buffer.ubx_nav_cov_raw.payload.pos.cov.ne = 1.02;
    buffer.ubx_nav_cov_raw.payload.pos.cov.nd = 1.03;
    buffer.ubx_nav_cov_raw.payload.pos.cov.ee = 1.04;
    buffer.ubx_nav_cov_raw.payload.pos.cov.ed = 1.05;
    buffer.ubx_nav_cov_raw.payload.pos.cov.dd = 1.06;

    buffer.ubx_nav_cov_raw.payload.vel.cov.nn = 0.51;
    buffer.ubx_nav_cov_raw.payload.vel.cov.ne = 0.52;
    buffer.ubx_nav_cov_raw.payload.vel.cov.nd = 0.53;
    buffer.ubx_nav_cov_raw.payload.vel.cov.ee = 0.54;
    buffer.ubx_nav_cov_raw.payload.vel.cov.ed = 0.45;
    buffer.ubx_nav_cov_raw.payload.vel.cov.dd = 0.56;

    buffer.ubx_nav_status_raw.payload.flags2.spoofDetState = NO_SPOOFING_INDICATED;

    buffer.construct();

    auto next_awake = std::chrono::steady_clock::now() + 50ms;
    std::this_thread::sleep_until(next_awake);
    memcpy(gnss_buffer, &buffer, UbloxBuffer::SIZE);

    while (true) {
        const auto crnt_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(crnt_time - init_time).count();
        if (ubx_byte_counter % UbloxBuffer::SIZE == 0) {
            std::cout << elapsed_time << std::endl;
            buffer.ubx_nav_pvt_raw.payload.time_of_week_ms = elapsed_time;
            buffer.construct();
        }

        for (uint_fast8_t idx = 0; idx < 1; idx++) {
            ubx_byte_counter++;
            size_t buffer_idx = ubx_byte_counter % GNSS_BUFFER_SIZE;
            size_t ubx_tx_buffer_idx = ubx_byte_counter % UbloxBuffer::SIZE;
            gnss_buffer[buffer_idx] = ((uint8_t*)&buffer)[ubx_tx_buffer_idx];
            uartSetLastReceivedIndex(UART_FIRST, buffer_idx);
        }

        std::this_thread::sleep_until(next_awake);
        next_awake += 1ms;
    }
}
