#include <adc.h>
#include <gpio.h>

static void (*next_callback)(float);

#define CYCLES_MAX 8U

#define CHANNEL_BBASE 8U

#define CHANNEL_CBASE 10U

#define ADC_DR_MAX_VALUE 4096.f

void adcInit(uint16_t channel) {
  if (channel < CHANNEL_CBASE) {
    ADC1->SMPR2 |= CYCLES_MAX << channel * 3;
    if (channel < CHANNEL_BBASE) {
      GPIOainConfigure(GPIOA, channel);
    } else {
      GPIOainConfigure(GPIOB, channel - CHANNEL_BBASE);
    }
  } else {
    ADC1->SMPR1 |= CYCLES_MAX << ((channel - CHANNEL_CBASE) * 3);
    GPIOainConfigure(GPIOC, channel - CHANNEL_CBASE);
  }
  ADC1->SQR3 = channel;
  NVIC_EnableIRQ(ADC_IRQn);
  ADC1->CR1 |= ADC_CR1_EOCIE;
  ADC1->CR2 |= ADC_CR2_ADON;
}

void adcMeasure(void (*callback)(float)) {
  next_callback = callback;
  ADC1->CR2 |= ADC_CR2_SWSTART;
}

void ADC_IRQHandler() {
  if (ADC1->SR & ADC_SR_EOC) {
    float vol = (REF_V / ADC_DR_MAX_VALUE) * ADC1->DR;
    (*next_callback)(vol);
  }
}