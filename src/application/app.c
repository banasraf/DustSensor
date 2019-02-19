#include <app.h>
#include <uart.h>
#include <dust-sensor.h>
#include <timers.h>
#include <multi-measure.h>
#include <logger.h>
#include <stdio.h>
#include <stm32.h>

#define MESSAGE_MAX_SIZE 128
#define MESSAGE_FMT "PM 2.5 concentration: %.0f ug/m3 \r\n"

void sendResult(float concentration) {
  char message[MESSAGE_MAX_SIZE];
  sprintf(message, MESSAGE_FMT, concentration);
  uartLog(message);
}

void appStartup() {
  uartSetup();
  dustSensorSetup();
  multiMeasureSetup(&sendResult);
  schedulerSetup();
  schedulerStart(WAIT_TIME_SECONDS, &multiMeasure);
}

void mainLoop() {
  for (;;) {
    __WFI();
    waitForSend();
  }
}
