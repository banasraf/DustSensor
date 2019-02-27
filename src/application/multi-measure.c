#include <multi-measure.h>
#include <timers.h>
#include <dust-sensor.h>

static void (*_handler)(uint32_t);

static uint32_t sum = 0;
static unsigned measures = 0;

void handleFinalResult() {
  (*_handler)(sum / MEASURES_COUNT);
  sum = 0;
  measures = 0;
}

static void handleMeasurment(uint32_t result) {
  sum += result;
  ++measures;
  if (measures == MEASURES_COUNT) handleFinalResult();
}

static void measure() {
  if (measures < MEASURES_COUNT) {
    dustSensorMeasure();
    if (measures < MEASURES_COUNT - 1) milliTimerStart(MEASURE_PERIOD);
  }
}

void multiMeasure() {
    measure();
}

void multiMeasureSetup(void (*handler)(uint32_t)) {
  _handler = handler;
  milliTimerSetup();
  milliTimerSetHandler(&measure);
  dustSensorSetHandler(&handleMeasurment);
}
