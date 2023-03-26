# Ublox gnss driver

This driver is dedicated for parsing data from ublox [L1 NEO-M8](https://www.u-blox.com/en/product/max-8-series) and [L1/L2 ZED-F9P](https://www.u-blox.com/en/product/zed-f9p-module).

It should support the following messages:
- UBX-NAV-PVT
- UBX-NAV2-COV (UBX-NAV-COV)
- UBX-NAV-RELPOSNED (for Moving Baseline)

It also should provide a possibility to configure the device to required settings.

The driver should be stateful (keep previous parsed buffer state) and tolerate to errors.

## Usage example:

```c++
#include "ublox.h"

static UbxNavPvt_t ubx_nav_pvt;
static UbxNavStatus_t ubx_nav_status;
static UbxNavCov_t ubx_nav_cov;
...

void application_init() {
    ...
}

void application_parse_serial_buffer(const uint8_t serial_buffer[], size_t serial_buffer_size) {
    auto ubx_package_type = ubloxParse(serial_buffer, serial_buffer_size);

    if (ubx_package_type == UBX_NAV_PVT) {
        ubloxGetNavPvt(ubx_nav_pvt);
    } else if (ubx_package_type == UBX_NAV_STATUS) {
        ubloxGetNavStatus(ubx_nav_status);
    } else if (ubx_package_type == UBX_NAV_COV) {
        ubloxGetNavCov(ubx_nav_cov);
    } else {
        ...
    }
}

```
