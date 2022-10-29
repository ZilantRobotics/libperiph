/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file esp8266.cpp
 * @author d.ponomarev
 * @brief Implementation of work with esp8266
 * @date Jul 15, 2018
 */

#include "esp8266.hpp"
#include "text.hpp"

WifiEsp8266 Wifi;

// implement your own version from the outside
__attribute__ ((weak)) void serialSend(const uint8_t* arr, uint8_t length) {
}

__attribute__ ((weak)) void serialRecv(uint8_t* ptrArr, uint8_t length) {
}

__attribute__ ((weak)) void waitMs(uint16_t ms) {
}


void WifiEsp8266::Init() {
    uint8_t counterOfCommand = 0;
    uint8_t length;

    while (1) {
        switch(counterOfCommand++) {
            case 0:
                Send_AT_CWMODE();
                waitMs(5000);
                break;
            case 1:
                Send_AT_RST();
                waitMs(12000);
                break;
            case 2:
                ConnectToApp();
                waitMs(8000);
                break;
            case 3:
                GetLocalIpAddress();
                waitMs(4000);
                break;
            case 4:
                EstablishTcpConnection();
                waitMs(4000);
                break;
            case 5:
                SendHelloWorld();
                waitMs(500);
                break;
            default:
                return;
        }
    }
}


void WifiEsp8266::Transmit(const uint8_t* ptrArr, const uint8_t& length) {
    uint8_t header[] = "AT+CIPSEND=";
    serialSend(header, sizeof(header)/sizeof(header[0]));

    uint8_t tail[] = "\r\n";
    if (length == 4) {
        uint8_t str[4];
        num2str(length+2, str);
        serialSend(str, 1);
        serialSend((uint8_t*)"\r\n", 2);
        waitMs(1);
        serialSend(ptrArr, length);
        serialSend(tail, sizeof(tail)/sizeof(tail[0]));
    }
}

void WifiEsp8266::Send_AT_CWMODE() {
    uint8_t cmd[] = "AT+CWMODE=3\r\n";
    serialSend(cmd, sizeof(cmd)/sizeof(cmd[0]));
}


void WifiEsp8266::Send_AT_RST() {
    uint8_t cmd[] = "AT+RST\r\n";
    serialSend(cmd, sizeof(cmd)/sizeof(cmd[0]));
}

void WifiEsp8266::ConnectToApp() {
    /// Param <ssid> string, AP SSID
    /// Param <password> string, MAX: 64 bytes ASCII
    uint8_t cmd[] = "AT+CWJAP=\"Room-24\",\"7y1QMxcD\"\r\n";
    serialSend(cmd, sizeof(cmd)/sizeof(cmd[0]));
}

void WifiEsp8266::GetLocalIpAddress() {
    uint8_t cmd[] = "AT+CIFSR\r\n";
    serialSend(cmd, sizeof(cmd)/sizeof(cmd[0]));
}

/// Establish TCP connection, UDP transmission or SSL connection
/// Param <type> string, "TCP" or "UDP"
/// Param <remote IP> string, remote IP address
/// Param <remote port> string, remote port number
void WifiEsp8266::EstablishTcpConnection() {
    uint8_t cmd[] = "AT+CIPSTART=\"TCP\",\"192.168.1.22\",8088\r\n";
    serialSend(cmd, sizeof(cmd)/sizeof(cmd[0]));
}

void WifiEsp8266::SendHelloWorld() {
    /// Param <length> data length, MAX 2048 bytes
    uint8_t header[] = "AT+CIPSTART=\"TCP\",\"192.168.1.22\",8088\r\n";
    serialSend(header, sizeof(header)/sizeof(header[0]));

    uint8_t msg[] = "HELLO FROM STM32";
    serialSend(msg, sizeof(msg)/sizeof(msg[0]));
}

/**
* @brief Get pointer on response array and length of array
* @param ptrArr - pointer on response array
* @param length - link on length of array
*/
void WifiEsp8266::GetResponse(uint8_t* ptrArr, uint8_t length) {
}
