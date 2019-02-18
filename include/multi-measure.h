#ifndef DUSTSENSOR_MULTI_MEASURE_H
#define DUSTSENSOR_MULTI_MEASURE_H

#define MEASURES_COUNT 10
#define MEASURE_PERIOD 1000 // milliseconds

void multiMeasureSetup(void (*handler)(float));

void multiMeasure();

#endif //DUSTSENSOR_MULTI_MEASURE_H
