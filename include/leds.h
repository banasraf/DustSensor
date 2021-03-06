#ifndef LEDS_H
#define LEDS_H

#include <delay.h>
#include <gpio.h>
#include <stm32.h>

#define GREEN2_LED_GPIO GPIOA
#define GREEN2_LED_PIN 5

#define Green2LEDon() \
  GREEN2_LED_GPIO->BSRRL = 1 << GREEN2_LED_PIN
#define Green2LEDoff() \
  GREEN2_LED_GPIO->BSRRH = 1 << GREEN2_LED_PIN

#define Green2LEDsetup() \
  GPIOoutConfigure(GREEN2_LED_GPIO,GREEN2_LED_PIN,GPIO_OType_PP,GPIO_Low_Speed,GPIO_PuPd_NOPULL);

#endif
