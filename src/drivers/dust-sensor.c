#include <dust-sensor.h>
#include <gpio.h>
#include <adc.h>
#include <timers.h>

// The procedure of measurement:
// - turn on ILED
// - wait MEASURE_DELAY microseconds for output stabilization
// - measure the voltage on output
// - scale up the vale by VOLTAGE_SCALE
// - calculate the dust concentration using the linear relationship with voltage

#define MEASURE_DELAY 280 // microseconds

#define ILEDon() \
  ILED_GPIO->BSRRL = 1 << ILED_PIN
#define ILEDoff() \
  ILED_GPIO->BSRRH = 1 << ILED_PIN

#define ILEDsetup() \
  GPIOoutConfigure(ILED_GPIO,ILED_PIN,GPIO_OType_PP,GPIO_Low_Speed,GPIO_PuPd_NOPULL);

#define ClockSetup() \
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

static void nop(uint32_t _) {}

static void (*_handler)(uint32_t) = &nop;

uint32_t volToConcentration(uint32_t millivolts) {
  uint32_t scaled_v = millivolts * VOLTAGE_SCALE;
  uint32_t result;
  if (scaled_v <= VOLTAGE_BIAS) {
    result = 0;
  } else {
    result = (scaled_v - VOLTAGE_BIAS) * SLOPE / 1000;
  }
  return result;
}

void adcHandle(uint32_t millivolts) {
  ILEDoff();
  uint32_t result = volToConcentration(millivolts);
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

void dustSensorSetHandler(void (*handler)(uint32_t)) {
  _handler = handler;
}
void dustSensorMeasure() {
  ILEDon();
  microTimerStart(MEASURE_DELAY);
}
