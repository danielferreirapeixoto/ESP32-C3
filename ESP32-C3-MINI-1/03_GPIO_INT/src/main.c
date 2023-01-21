#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO_2      2   //PIN 4
#define SWITCH_GPIO_0   0   //PIN 9

bool flag_led = 0;


static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    int pinNumber = (int)args;

    if (pinNumber == SWITCH_GPIO_0)
    {
        if (flag_led)
        {
            gpio_set_level(LED_GPIO_2, 0);
            flag_led = false;
        } 
        else
        {
            gpio_set_level(LED_GPIO_2, 1) ;
            flag_led = true;
        } 
    }

}

void app_main() {

    // Configure GPIO2 as output push-pull
    gpio_set_direction(LED_GPIO_2, GPIO_MODE_OUTPUT);


    // Configure GPIO0 as input
    gpio_set_direction(SWITCH_GPIO_0, GPIO_MODE_INPUT);
    // Configure GPIO0 as pull-down
    gpio_set_pull_mode(SWITCH_GPIO_0, GPIO_PULLDOWN_ONLY);
    // Configure GPIO0 edge
    gpio_set_intr_type(SWITCH_GPIO_0, GPIO_INTR_POSEDGE);
    // Configure GPIO0 external interrupt
    gpio_install_isr_service(0);
    gpio_isr_handler_add(SWITCH_GPIO_0, gpio_interrupt_handler, (void *)SWITCH_GPIO_0);
    

    while(1) {}

}