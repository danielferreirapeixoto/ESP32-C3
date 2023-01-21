#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO_2  2   //PIN 4
#define LED_GPIO_3  3   //PIN 5

void app_main() {

    // Configure GPIO2 as output push-pull
    gpio_set_direction(LED_GPIO_2, GPIO_MODE_OUTPUT);

    // Configure GPIO3 as output open drain
    gpio_set_direction(LED_GPIO_3, GPIO_MODE_OUTPUT_OD);

    while(1) {

        // Turn on LED_GPIO_2, Turn off LED_GPIO_3
        gpio_set_level(LED_GPIO_2, 1);
        gpio_set_level(LED_GPIO_3, 1);
    
        // Delay 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // Turn off LED_GPIO_2, Turn on LED_GPIO_3
        gpio_set_level(LED_GPIO_2, 0);
        gpio_set_level(LED_GPIO_3, 0);

        // Delay 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }

}