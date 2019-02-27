#ifndef DUSTSENSOR_MULTI_MEASURE_H
#define DUSTSENSOR_MULTI_MEASURE_H

#include <stdint.h>

#define MEASURES_COUNT 10
#define MEASURE_PERIOD 1000 // milliseconds

void multiMeasureSetup(void (*handler)(uint32_t));

void multiMeasure();

#endif //DUSTSENSOR_MULTI_MEASURE_H
