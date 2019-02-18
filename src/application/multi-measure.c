#include <multi-measure.h>
#include <timers.h>
#include <dust-sensor.h>

static void (*_handler)(float);

static float buffer[MEASURES_COUNT];
static unsigned measures = 0;

void handleFinalResult() {
  float sum = 0.f;
  for (unsigned i = 0; i < MEASURES_COUNT; ++i) sum += buffer[i];
  (*_handler)(sum / MEASURES_COUNT);
  measures = 0;
}

static void handleMeasurment(float result) {
  buffer[measures++] = result;
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

void multiMeasureSetup(void (*handler)(float)) {
  _handler = handler;
  milliTimerSetup();
  milliTimerSetHandler(&measure);
  dustSensorSetHandler(&handleMeasurment);
}
