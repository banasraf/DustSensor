#include <uart.h>
#include <gpio.h>

static int uart_initialized = 0;

typedef struct {
  const char *data;
  uint32_t size;
  void (*callback)(uint32_t);
} SendBlock;

typedef struct {
  char *buffer;
  uint32_t size;
  void (*callback)(char *, uint32_t);
} ReceiveBlock;

static SendBlock send_queue[SEND_QUEUE_SIZE];
static uint32_t send_queue_elems_count = 0;
static uint32_t send_queue_index = 0;

static ReceiveBlock receive_queue[RECEIVE_QUEUE_SIZE];
static uint32_t receive_queue_elems_count = 0;
static uint32_t receive_queue_index = 0;

#define TransmissionInProgress() (!((DMA1_Stream6->CR & DMA_SxCR_EN) == 0 && (DMA1->HISR & DMA_HISR_TCIF6) == 0))
#define ReceivingInProgress() (!((DMA1_Stream5->CR & DMA_SxCR_EN) == 0 && (DMA1->HISR & DMA_HISR_TCIF5) == 0))

#define ClocksConfigure() \
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA1EN; \
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

#define PinConfigure() \
    GPIOafConfigure(GPIOA,            \
                    2,                \
                    GPIO_OType_PP,    \
                    GPIO_Fast_Speed,  \
                    GPIO_PuPd_NOPULL, \
                    GPIO_AF_USART2);  \
    GPIOafConfigure(GPIOA,            \
                    3,                \
                    GPIO_OType_PP,    \
                    GPIO_Fast_Speed,  \
                    GPIO_PuPd_UP,     \
                    GPIO_AF_USART2);

#define DmaTConfigure() \
    DMA1_Stream6->CR = 4U << 25       | \
                    DMA_SxCR_PL_1     | \
                    DMA_SxCR_MINC     | \
                    DMA_SxCR_DIR_0    | \
                    DMA_SxCR_TCIE;      \
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;

#define DmaRConfigure() \
    DMA1_Stream5->CR = 4U << 25       | \
                   DMA_SxCR_PL_1      | \
                   DMA_SxCR_MINC      | \
                   DMA_SxCR_TCIE;       \
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;

void uartSetup() {
  ClocksConfigure();
  PinConfigure();

  USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
  USART2->CR2 = 0x0000;
  USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;

  uint32_t const baudrate = 9600U;
  USART2->BRR = (PCLK1_HZ + (baudrate / 2U)) / baudrate;

  DmaTConfigure();
  DmaRConfigure();

  DMA1->HIFCR = DMA_HIFCR_CTCIF6 |
      DMA_HIFCR_CTCIF5;
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  USART2->CR1 |= USART_CR1_UE;
  uart_initialized = 1;
}

void initSend() {
  DMA1_Stream6->M0AR = (uint32_t) send_queue[send_queue_index].data;
  DMA1_Stream6->NDTR = send_queue[send_queue_index].size;
  DMA1_Stream6->CR |= DMA_SxCR_EN;
}

UartIOResult uartSend(const char *data, uint32_t size, void (*callback)(uint32_t)) {
  if (!uart_initialized) return UART_UNINITIALIZED;
  if (send_queue_elems_count == SEND_QUEUE_SIZE) return UART_QUEUE_FULL;
  send_queue[(send_queue_index + send_queue_elems_count++) % SEND_QUEUE_SIZE] = (SendBlock) {data, size, callback};
  if (!TransmissionInProgress()) {
    initSend();
  }
  return UART_OK;
}

void DMA1_Stream6_IRQHandler() {
  uint32_t isr = DMA1->HISR;
  if (isr & DMA_HISR_TCIF6) {
    (*send_queue[send_queue_index].callback)(send_queue[send_queue_index].size);
    send_queue_index = (send_queue_index + 1) % SEND_QUEUE_SIZE;
    send_queue_elems_count--;
    DMA1->HIFCR = DMA_HIFCR_CTCIF6;
    if (send_queue_elems_count > 0) initSend();
  }
}

void initReceive() {
  DMA1_Stream5->M0AR = (uint32_t) receive_queue[receive_queue_index].buffer;
  DMA1_Stream5->NDTR = receive_queue[receive_queue_index].size;
  DMA1_Stream5->CR |= DMA_SxCR_EN;
}

UartIOResult uartReceive(char *buffer, uint32_t size, void (*callback)(char *, uint32_t)) {
  if (!uart_initialized) return UART_UNINITIALIZED;
  if (receive_queue_elems_count == RECEIVE_QUEUE_SIZE) return UART_QUEUE_FULL;
  receive_queue[(receive_queue_index + receive_queue_elems_count++) % RECEIVE_QUEUE_SIZE] =
      (ReceiveBlock) {buffer, size, callback};
  if (!ReceivingInProgress()) {
    initReceive();
  }
  return UART_OK;
}

void DMA1_Stream5_IRQHandler() {
  uint32_t isr = DMA1->HISR;
  if (isr & DMA_HISR_TCIF5) {
    (*receive_queue[receive_queue_index].callback)(receive_queue[receive_queue_index].buffer,
                                                   receive_queue[receive_queue_index].size);
    receive_queue_index = (receive_queue_index + 1) % RECEIVE_QUEUE_SIZE;
    receive_queue_elems_count--;
    DMA1->HIFCR = DMA_HIFCR_CTCIF5;
    if (receive_queue_elems_count > 0) initReceive();
  }
}