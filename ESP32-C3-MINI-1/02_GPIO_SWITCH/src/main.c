#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO_2      2   //PIN 4
#define SWITCH_GPIO_0   0   //PIN 9

void app_main() {

    // Configure GPIO2 as output push-pull
    gpio_set_direction(LED_GPIO_2, GPIO_MODE_OUTPUT);

    // Configure GPIO0 as input
    gpio_set_direction(SWITCH_GPIO_0, GPIO_MODE_INPUT);
    // Configure GPIO0 as pull-down
    gpio_set_pull_mode(SWITCH_GPIO_0, GPIO_PULLDOWN_ONLY);

    while(1) {

        if (gpio_get_level(SWITCH_GPIO_0))
        {
            // Turn on LED_GPIO_2
            gpio_set_level(LED_GPIO_2, 1);
        }
        else
        {
            // Turn off LED_GPIO_2
            gpio_set_level(LED_GPIO_2, 0);
        }
        
    }
    
}