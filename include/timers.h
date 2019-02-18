#ifndef DUSTSENSOR_TIMERS_H
#define DUSTSENSOR_TIMERS_H

#include <stdint.h>

#define CLOCK_FREQ 16000000u

void microTimerSetup();

void microTimerSetHandler(void (*handler)());

void microTimerStart(uint16_t microseconds);

void milliTimerSetup();

void milliTimerSetHandler(void (*handler)());

void milliTimerStart(uint16_t milliseconds);

void schedulerSetup();

void schedulerStart(uint8_t seconds, void (*handler)());

#endif //DUSTSENSOR_TIMERS_H
