# Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

SRC += $(wildcard ${LIBPERIPH_DIR}/sensors/barometer/*.c) \
       $(wildcard ${LIBPERIPH_DIR}/sensors/current_sensor/*.c) \
       $(wildcard ${LIBPERIPH_DIR}/sensors/esc/*.c) \
       $(wildcard ${LIBPERIPH_DIR}/sensors/gps/*.c) \
       $(wildcard ${LIBPERIPH_DIR}/sensors/imu/*.c) \
       $(wildcard ${LIBPERIPH_DIR}/sensors/magnetometer/*.c) \
       ${LIBPERIPH_DIR}/sensors/rangefinder/garmin_lite/garmin_lite.c \
       ${LIBPERIPH_DIR}/sensors/rangefinder/sf1xx/sf1xx.c \
       ${LIBPERIPH_DIR}/sensors/rangefinder/tf_mini/tf_mini.c \
       ${LIBPERIPH_DIR}/sensors/rangefinder/vl53l0x/vl53l0x.c \
       $(wildcard ${LIBPERIPH_DIR}/sensors/temperature_sensor/*.c)
INC += -I${REPOSITORY_PATH}/Lib/libperiph/sensors
