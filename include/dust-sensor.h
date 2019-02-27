#ifndef DUSTSENSOR_DUST_SENSOR_H
#define DUSTSENSOR_DUST_SENSOR_H

#include <stdint.h>

#define ILED_GPIO GPIOA
#define ILED_PIN 1
#define VIN_CHANNEL 0

#define VOLTAGE_BIAS 300U // millivolts
#define SLOPE 200U // ug/(m^3*V)
#define VOLTAGE_SCALE 11U

void dustSensorSetup();

void dustSensorSetHandler(void (*handler)(uint32_t));

void dustSensorMeasure();

#endif //DUSTSENSOR_DUST_SENSOR_H
