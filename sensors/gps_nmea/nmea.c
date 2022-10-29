/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file nmea.c
 * @author ko.burdinov, p.ponomarev
 * @date 2019
 */
#include "nmea.h"
#include <stdio.h>
#include <math.h>


#define SENTENCE_MAX_LEN        128


static uint8_t rmc_buffer[SENTENCE_MAX_LEN] = {0};
static uint8_t gga_buffer[SENTENCE_MAX_LEN] = {0};


static bool isThisIndexStart(uint8_t byte);
static bool isItGga(uint8_t idx);
static bool isItRmc(uint8_t idx);


bool nmeaProcess(uint8_t gns_buffer[], size_t gns_buffer_size, GnssFix2_t* uavcan_fix2) {
    bool is_package_updated = false;
    ringBufferInit(gns_buffer, gns_buffer_size);
    for (size_t idx = 0; idx < gns_buffer_size; idx++) {
        if (!isThisIndexStart(gns_buffer[idx])) {
            continue;
        }
        if (isItGga(idx)) {
            ringBufferCopy(gga_buffer, idx, SENTENCE_MAX_LEN);
            parseGGA(gga_buffer, uavcan_fix2);
            is_package_updated = true;
        } else if (isItRmc(idx)) {
            ringBufferCopy(rmc_buffer, idx, SENTENCE_MAX_LEN);
            parseRMC(rmc_buffer, uavcan_fix2);
            is_package_updated = true;
        }
    }
    return is_package_updated;
}


bool isThisIndexStart(uint8_t byte) {
    return byte == '$';
}

bool isItGga(uint8_t idx) {
    uint8_t hdr[] = "GGA";
    if (ringBufferCompare(hdr, 3, idx + 3)) {
        return true;
    }
    return false;
}

bool isItRmc(uint8_t idx) {
    uint8_t hdr[] = "RMC";
    if (ringBufferCompare(hdr, 3, idx + 3)) {
        return true;
    }
    return false;
}

char *strtok_fr(char *s, char delim, char **save_ptr) {
    char *tail;
    char c;

    if (s == NULL) {
        s = *save_ptr;
    }
    tail = s;
    if ((c = *tail) == '\0') {
        s = NULL;
    } else {
        do {
            if (c == delim) {
                *tail++ = '\0';
                break;
            }
        } while ((c = *++tail) != '\0');
    }
    *save_ptr = tail;
    return s;
}

char *strtok_f(uint8_t *s, uint8_t delim) {
    static char *save_ptr;
    return strtok_fr ((char*)s, (char)delim, &save_ptr);
}

int8_t parseRMC(uint8_t buffer[], GnssFix2_t* Fix2) {
    char* Message_ID = strtok_f(buffer, ',');
    char* Time = strtok_f(NULL, ',');
    char* Data_Valid = strtok_f(NULL, ',');
    char* Raw_Latitude = strtok_f(NULL, ',');
    char* N_S = strtok_f(NULL, ',');
    char* Raw_Longitude = strtok_f(NULL, ',');
    char* E_W = strtok_f(NULL, ',');
    char* Speed = strtok_f(NULL, ',');
    char* COG = strtok_f(NULL, ',');
    char* Date = strtok_f(NULL, ',');
    char* Magnetic_Variation = strtok_f(NULL, ',');
    char* M_E_W = strtok_f(NULL, ',');
    char* Positioning_Mode = strtok_f(NULL, ',');

    if (Message_ID == NULL || Time == NULL || Data_Valid == NULL ||
            Raw_Latitude == NULL || N_S == NULL || Raw_Longitude == NULL ||
            E_W == NULL || Speed == NULL || COG == NULL || Date == NULL ||
            Magnetic_Variation == NULL || M_E_W == NULL || Positioning_Mode == NULL) {
        return STATUS_ERROR;
    }

    float Latitude = atof(Raw_Latitude);
    float latitude_deg_1e8 = round(Latitude/100.0);
    latitude_deg_1e8 = (Latitude-latitude_deg_1e8*100.0)/60 + latitude_deg_1e8;
    latitude_deg_1e8 = latitude_deg_1e8*100000000;

    float Longitude = atof(Raw_Longitude);
    float longitude_deg_1e8 = round(Longitude/100.0);
    longitude_deg_1e8 = (Longitude-longitude_deg_1e8*100.0)/60 + longitude_deg_1e8;
    longitude_deg_1e8 = longitude_deg_1e8*100000000;

    Fix2->longitude_deg_1e8 = longitude_deg_1e8;
    Fix2->latitude_deg_1e8 = latitude_deg_1e8;

    float speed = atof(Speed);
    speed = speed * 0.514444;  //  m/s
    float cog = atof(COG);
    cog = cog / 180.0 * 3.14159265;  //  rad

    float vel_n = speed*(float)cos((double)(cog));
    float vel_e = speed*(float)sin((double)(cog));
    float vel_d = 0;

    Fix2->ned_velocity[0] = vel_n;
    Fix2->ned_velocity[1] = vel_e;
    Fix2->ned_velocity[2] = vel_d;

    return STATUS_OK;
}

int8_t parseGGA(uint8_t buffer[], GnssFix2_t* Fix2) {
    char* Message_ID = strtok_f(buffer, ',');
    char* Time = strtok_f(NULL, ',');
    char* Raw_Latitude = strtok_f(NULL, ',');
    char* N_S = strtok_f(NULL, ',');
    char* Raw_Longitude = strtok_f(NULL, ',');
    char* E_W = strtok_f(NULL, ',');
    char* GPS_Quality_Indicator = strtok_f(NULL, ',');
    char* Number_of_satellites_in_view = strtok_f(NULL, ',');
    char* Horizontal_Dilution_of_precision = strtok_f(NULL, ',');
    char* Altitude = strtok_f(NULL, ',');
    char* Units_Altitude = strtok_f(NULL, ',');
    char* Geoidal = strtok_f(NULL, ',');
    char* Units_Geoidal = strtok_f(NULL, ',');
    char* Age = strtok_f(NULL, ',');
    char* Differential_reference_station_ID = strtok_f(NULL, ',');

    if (Message_ID == NULL ||
            Time == NULL ||
            Raw_Latitude == NULL ||
            N_S == NULL ||
            Raw_Longitude == NULL ||
            E_W == NULL ||
            GPS_Quality_Indicator == NULL ||
            Number_of_satellites_in_view == NULL ||
            Horizontal_Dilution_of_precision == NULL ||
            Altitude == NULL ||
            Units_Altitude == NULL ||
            Geoidal == NULL ||
            Units_Geoidal == NULL ||
            Age == NULL ||
            Differential_reference_station_ID == NULL) {
        return STATUS_ERROR;
    }

    Fix2->status = STATUS_NO_FIX;
    float fTime = atof(Time);
    if (fTime > 0) {
        Fix2->status = STATUS_TIME_ONLY;
    }

    float Latitude = atof(Raw_Latitude);
    float latitude_deg_1e8 = round(Latitude/100.0);
    latitude_deg_1e8 = (Latitude-latitude_deg_1e8*100.0)/60 + latitude_deg_1e8;
    latitude_deg_1e8 = latitude_deg_1e8*100000000;

    float Longitude = atof(Raw_Longitude);
    float longitude_deg_1e8 = round(Longitude/100.0);
    longitude_deg_1e8 = (Longitude-longitude_deg_1e8*100.0)/60 + longitude_deg_1e8;
    longitude_deg_1e8 = longitude_deg_1e8*100000000;

    if ((latitude_deg_1e8 != 0) && (longitude_deg_1e8 != 0)) {
        Fix2->status = STATUS_2D_FIX;
    }

    Fix2->longitude_deg_1e8 = longitude_deg_1e8;
    Fix2->latitude_deg_1e8 = latitude_deg_1e8;

    float altitude = atof(Altitude);
    altitude = altitude*1000.0;


    if (altitude != 0) {
        Fix2->status = STATUS_3D_FIX;
    }

    float geo = atof(Geoidal);
    geo = geo*1000.0;

    Fix2->height_msl_mm = altitude;
    Fix2->height_ellipsoid_mm = geo;

    uint8_t sat_num = atoi(Number_of_satellites_in_view);
    Fix2->sats_used = sat_num;

    float pdop = atof(Horizontal_Dilution_of_precision);
    Fix2->pdop = pdop;

    return STATUS_OK;
}
