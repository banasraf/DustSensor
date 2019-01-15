#include <leds.h>
#include <uart.h>
#include <string.h>
#include <stdio.h>

void nop(uint32_t _) {}

volatile int sec = 0;

void TIM3_IRQHandler(void) {
  uint32_t it_status = TIM3->SR & TIM3->DIER;
  if (it_status & TIM_SR_UIF) {
    TIM3->SR = ~TIM_SR_UIF;
    char str[32];
    sprintf(str, "%lu \r\n", ADC1->DR);
    uartSend(str, strlen(str), &nop);
  }
}

void setupTimer() {
  TIM3->CR1 = 0; // count up
  TIM3->PSC = 244; // set prescaler
  TIM3->EGR = TIM_EGR_UG;; // update prescaler
  TIM3->SR = ~(TIM_SR_UIF);
  TIM3->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(TIM3_IRQn);
  TIM3->CR1 |= TIM_CR1_CEN;
}

int main() {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  uartSetup();

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//    ADC->CCR; // prescaler?
  ADC1->CR1 = ADC_CR1_RES_0 | ADC_CR1_RES_1; // resolution
  ADC1->SMPR1 = 30;
  ADC1->SQR1 = 0U << 20; /* jedna konwersja w grupie */
  ADC1->SQR2 = 0;
  ADC1->SQR3 = 17U; // v_ref
  ADC1->CR2 |= ADC_CR2_CONT;
  ADC1->CR2 |= ADC_CR2_ADON;
  ADC1->CR2 |= ADC_CR2_SWSTART;

  setupTimer();

  for (;;) {}
}

