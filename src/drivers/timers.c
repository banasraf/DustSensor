#include <timers.h>
#include <gpio.h>

// SCHEDULER_ARR * SCHEDULER_PRESCALER / CLOCK_FREQ = 1 second
#define SCHEDULER_ARR 65306u
#define SCHEDULER_PRESCALER 245u

// MILLI_TIMER_PRESCALER / CLOCK_FREQ = 1 millisecond
#define MILLI_TIMER_PRESCALER 16000u

static void nop() {};

static void (*micro_handler)() = &nop;

static void (*milli_handler)() = &nop;

static void (*scheduler_handler)() = &nop;

void microTimerSetup() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->CR1 = 0; // count up
  TIM3->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(TIM3_IRQn);
}

void microTimerSetHandler(void (*handler)()) {
  micro_handler = handler;
}
void microTimerStart(uint16_t microseconds) {
  TIM3->ARR = (CLOCK_FREQ / 1000000u) * microseconds;
  TIM3->CNT = 0;
  TIM3->CR1 |= TIM_CR1_CEN;
}

void schedulerSetup() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  TIM4->CR1 = 0; // count up
  TIM4->ARR = SCHEDULER_ARR;
  TIM4->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(TIM4_IRQn);
}

void schedulerStart(uint8_t seconds, void (*handler)()) {
  scheduler_handler = handler;
  TIM4->CR1 |=  TIM_CR1_UDIS;
  TIM4->PSC = seconds * SCHEDULER_PRESCALER;
  TIM4->CR1 &= ~TIM_CR1_UDIS;
  TIM4->EGR = TIM_EGR_UG; // update
  TIM4->SR = ~TIM_SR_UIF;
  TIM4->CR1 |= TIM_CR1_CEN;
}

void milliTimerSetup() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  TIM5->CR1 = 0; // count up
  TIM5->PSC = MILLI_TIMER_PRESCALER;
  TIM5->EGR = TIM_EGR_UG; // update
  TIM5->SR = ~TIM_SR_UIF;
  TIM5->DIER = TIM_DIER_CC1IE;
  NVIC_EnableIRQ(TIM5_IRQn);
}

void milliTimerSetHandler(void (*handler)()) {
  milli_handler = handler;
}

void milliTimerStart(uint16_t milliseconds) {
  TIM5->CCR1 = milliseconds;
  TIM5->CNT = 0;
  TIM5->CR1 |= TIM_CR1_CEN;
}

void TIM5_IRQHandler() {
  if (TIM5->SR & TIM5->DIER & TIM_SR_CC1IF) {
    TIM5->SR = ~TIM_SR_CC1IF;
    TIM5->CR1 &= ~TIM_CR1_CEN;
    (*milli_handler)();
  }
}

void TIM4_IRQHandler() {
  if (TIM4->SR & TIM4->DIER & TIM_SR_UIF) {
    TIM4->SR = ~TIM_SR_UIF;
    (*scheduler_handler)();
  }
}

void TIM3_IRQHandler() {
  if (TIM3->SR & TIM3->DIER & TIM_SR_UIF) {
    TIM3->SR = ~TIM_SR_UIF;
    TIM3->CR1 &= ~TIM_CR1_CEN;
    (*micro_handler)();
  }
}




