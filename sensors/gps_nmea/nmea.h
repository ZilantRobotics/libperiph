/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file nmea.h
 * @author ko.burdinov, p.ponomarev
 * @date 2019
 * @note http://www.tronico.fi/OH6NT/docs/NMEA0183.pdf
 *      https://ru.wikipedia.org/wiki/NMEA_0183
 *      http://forum.rcdesign.ru/blogs/86494/blog21244.html
 */

#ifndef SENSORS_GPS_NMEA_H_
#define SENSORS_GPS_NMEA_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


typedef enum {
    H_UNKNOWN,
    GGA,    // -  данные о последнем определении местоположения
    GLL,    // -  координаты, широта/долгота
    GSA,    // -  DOP (GPS) и активные спутники
    GSV,    // -  наблюдаемые спутники
    WPL,    // -  параметры заданной точки
    BOD,    // -  азимут одной точки относительно другой
    RMB,    // -  рекомендуемый минимум навигационных данных для достижения заданной точки
    RMC,    // -  рекомендуемый минимум навигационных данных (см. выше)
    RTE     // -  маршруты
} NMEA_header_t;

typedef enum {
    S_UNKNOWN,
    GP,     // GPS
    GL,     // «GL» — ГЛОНАСС
    GA,     // «GA» — Галилео, «GN» — ГЛОНАСС+GPS и т. п.
    GN,     // «GN» — ГЛОНАСС+GPS и т. п.
} NMEA_source_t;

/*
1 2 3 4 5 6 7 8 9 10 11|
| | | | | | | | | | | |
$--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh
1) Time (UTC)
2) Status, V = Navigation receiver warning
3) Latitude
4) N or S
5) Longitude
6) E or W
7) Speed over ground, knots
8) Track made good, degrees true
9) Date, ddmmyy
10) Magnetic Variation, degrees
11) E or W
12) Checksum
*/

typedef struct {
    // ,091106.00,A,5545.17828,N,04844.65973,E,0.339,,090620,,,A*62
    char sentence[128];
    char end;
    char time[9];
    char valid;
    char Latitude[10];
    char N_S;
    char Longitude[11];
    char E_W;
    char vel[20];
    char dir[20];
    char date[6];
    char mag[20];
    char mE_W;
    char m;
    char crc;
} RMC_t;

/*
1 2 3 4 5 6 7 8 9 10 | 12 13 14 15
| | | | | | | | | | | | | | |
$--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
1) Time (UTC)
2) Latitude
3) N or S (North or South)
4) Longitude
5) E or W (East or West)
6) GPS Quality Indicator,
0 - fix not available,
1 - GPS fix,
2 - Differential GPS fix
7) Number of satellites in view, 00 - 12
8) Horizontal Dilution of precision
9) Antenna Altitude above/below mean-sea-level (geoid)
10) Units of antenna altitude, meters
11) Geoidal separation, the difference between the WGS-84 earth
ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
12) Units of geoidal separation, meters
13) Age of differential GPS data, time in seconds since last SC104
type 1 or 9 update, null field when DGPS is not used
14) Differential reference station ID, 0000-1023
15) Checksum
*/

typedef struct {
    // ,091106.00,A,5545.17828,N,04844.65973,E,0.339,,090620,,,A*62
    char sentence[128];
    char end;
    uint8_t release;
    char time[9];
    char valid;
    char Latitude[10];
    char N_S;
    char Longitude[11];
    char E_W;
    char Quality;
    char SatNum[2];
    char precision[10];
    char Altitude[10];
    char unitsA;
    char mean_sea_level[10];
    char unitsMSL;
    char age;
    char id[4];
    char crc;
} GGA_t;


/*
 * @brief Parse raw nmea buffer and save data to uavcan fix2 buffer in success.
 * @param gns_buffer is an input raw buffer (or his part)
 * @param gns_buffer_size is a size of this buffer
 * @param uavcan_fix2 is an output buffer
 * @return true, if package successfully has been parsed
 */
bool nmeaProcess(uint8_t gns_buffer[], size_t gns_buffer_size, GnssFix2_t* uavcan_fix2);

int8_t parseRMC(uint8_t buffer[], GnssFix2_t* Fix2);
int8_t parseGGA(uint8_t buffer[], GnssFix2_t* Fix2);

#endif  // SENSORS_GPS_NMEA_H_
