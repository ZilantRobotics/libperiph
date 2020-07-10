/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file esp8266.hpp
 * @author d.ponomarev
 * @brief Class of wi-fi module ESP8266
 * @date Jul 15, 2018
 */

#ifndef ESP8266_HPP_
#define ESP8266_HPP_

#include "stdint.h"


/**
 * @brief Work with wi-fi module ESP8266
 */
class WifiEsp8266
{
    public:
        void Init();
        void GetResponse(uint8_t* ptrArr, uint8_t& length);

        /**
         * @brief Transmit data to TCP server
         */
        void Transmit(const uint8_t* ptrArr, const uint8_t& length);
    private:
        /**
         * @brief Restart the module
         */
        void Send_AT_RST();

        /**
         * @brief WiFi mode
         * Param = 3 => softAP + station mode
         * This setting will be stored in the flash system parameter area.
         * It wont be erased even when the power is off and restarted.
         */
        void Send_AT_CWMODE();

        void ConnectToApp();
        void GetLocalIpAddress();
        void EstablishTcpConnection();
        void SendHelloWorld();
};


#endif    // __ESP8266_HPP
