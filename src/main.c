#include <leds.h>

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    __NOP();
    Green2LEDoff();

    GPIOoutConfigure(GREEN2_LED_GPIO,
                     GREEN2_LED_PIN,
                     GPIO_OType_PP,
                     GPIO_Low_Speed,
                     GPIO_PuPd_NOPULL);
    for (;;) {
        Green2LEDon();
        Delay(500000);
        Green2LEDoff();
        Delay(300000);
    }
}
