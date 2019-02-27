#ifndef DUSTSENSOR_TIMERS_H
#define DUSTSENSOR_TIMERS_H

/**
 * Module gives access to three different timers:
 *  - micro timer - delayed execution with time resolution of a microsecond
 *  - milli timer - delayed execution with time resolution of a millisecond
 *  - scheduler - repeated call with period resolution of a second
 */

#include <stdint.h>

#define CLOCK_FREQ 16000000U

void microTimerSetup();

void microTimerSetHandler(void (*handler)());

void microTimerStart(uint16_t microseconds);

void milliTimerSetup();

void milliTimerSetHandler(void (*handler)());

void milliTimerStart(uint16_t milliseconds);

void schedulerSetup();

void schedulerStart(uint8_t seconds, void (*handler)());

#endif //DUSTSENSOR_TIMERS_H
