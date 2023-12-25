# Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

list(APPEND LIBPERIPH_DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/tf_mini.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CMAKE_CURRENT_LIST_DIR}/../../../platform_specific/hal_uart/CMakeLists.txt)