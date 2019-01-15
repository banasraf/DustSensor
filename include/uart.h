#ifndef DUSTSENSOR_UART_H
#define DUSTSENSOR_UART_H

#include <stdint.h>

#define HSI_HZ 16000000U
#define PCLK1_HZ HSI_HZ

#define SEND_QUEUE_SIZE 32
#define RECEIVE_QUEUE_SIZE 32

typedef enum {
  UART_OK,
  UART_QUEUE_FULL,
  UART_UNINITIALIZED
} UartIOResult;

void uartSetup();

UartIOResult uartSend(const char *data, uint32_t size, void (*callback)(uint32_t));

UartIOResult uartReceive(char *buffer, uint32_t size, void (*callback)(char *, uint32_t));

#endif //DUSTSENSOR_UART_H
