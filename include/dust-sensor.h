#ifndef DUSTSENSOR_DUST_SENSOR_H
#define DUSTSENSOR_DUST_SENSOR_H

#define ILED_GPIO GPIOA
#define ILED_PIN 1
#define VIN_CHANNEL 0

#define VOLTAGE_BIAS 300.f // millivolts
#define SLOPE 0.2f // ug/(m^3*mV)
#define VOLTAGE_SCALE 11.f

void dustSensorSetup();

void dustSensorSetHandler(void (*handler)(float));

void dustSensorMeasure();

#endif //DUSTSENSOR_DUST_SENSOR_H
