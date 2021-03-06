#ifndef DUSTSENSOR_ADC_H
#define DUSTSENSOR_ADC_H

#include <stdint.h>

#define AdcClockSetup() \
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

#define REF_V 3300U

void adcInit(uint16_t channel);

void adcMeasure(void (*callback)(uint32_t));

#endif //DUSTSENSOR_ADC_H
