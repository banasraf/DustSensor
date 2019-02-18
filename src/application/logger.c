#include <logger.h>
#include <stddef.h>
#include <memory.h>
#include <uart.h>

#define BUFFER_SIZE (1 << 14)

static char buffer[BUFFER_SIZE];
static size_t buf_origin = 0;
static size_t buf_count = 0;

static void bufferPop(uint32_t count) {
  buf_count -= count;
}

void uartLog(const char *str) {
  size_t len = strlen(str);
  if (len < BUFFER_SIZE - buf_count) {
    size_t first_part = BUFFER_SIZE - buf_origin < len ? BUFFER_SIZE - buf_origin : len;
    memcpy(buffer + buf_origin, str, first_part);
    buf_count += first_part;
    uartSend(buffer + buf_origin, first_part, &bufferPop);
    if (first_part < len) {
      memcpy(buffer, str + first_part, len - first_part);
      buf_count += len - first_part;
      uartSend(buffer, len - first_part, &bufferPop);
    }
    buf_origin = (buf_origin + len) % BUFFER_SIZE;
  }
}
