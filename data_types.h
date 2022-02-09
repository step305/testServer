//
// Created by sanch on 04.01.2022.
//

#ifndef TESTSERVER_DATA_TYPES_H
#define TESTSERVER_DATA_TYPES_H

typedef struct {
    float bw[3];
    float sw[3];
    float mw[6];
    float euler[3];
    float t;
    float adc;
}T_report_packet;

#endif //TESTSERVER_DATA_TYPES_H
