#include <dust-sensor.h>
#include <gpio.h>
#include <adc.h>
#include <timers.h>

#define MEASURE_DELAY 280 // microseconds

#define ILEDon() \
  ILED_GPIO->BSRRL = 1 << ILED_PIN
#define ILEDoff() \
  ILED_GPIO->BSRRH = 1 << ILED_PIN

#define ILEDsetup() \
  GPIOoutConfigure(ILED_GPIO,ILED_PIN,GPIO_OType_PP,GPIO_Low_Speed,GPIO_PuPd_NOPULL);

#define ClockSetup() \
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

static void nop(float _) {}

static void (*_handler)(float) = &nop;

float volToConcentration(float millivolts) {
  float scaled_v = millivolts * VOLTAGE_SCALE;
  float result;
  if (scaled_v <= VOLTAGE_BIAS) {
    result = 0;
  } else {
    result = (scaled_v - VOLTAGE_BIAS) * SLOPE;
  }
  return result;
}

void adcHandle(float millivolts) {
  ILEDoff();
  float result = volToConcentration(millivolts);
  (*_handler)(result);
}

void startProbing() {
  adcMeasure(&adcHandle);
}

void dustSensorSetup() {
  ClockSetup();
  ILEDsetup();
  AdcClockSetup();
  adcInit(VIN_CHANNEL);
  microTimerSetup();
  microTimerSetHandler(&startProbing);
}

void dustSensorSetHandler(void (*handler)(float)) {
  _handler = handler;
}
void dustSensorMeasure() {
  ILEDon();
  microTimerStart(280);
}
