#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define LED_GPIO_2      2   //PIN 4

ledc_channel_config_t   OUT_PWM;


void app_main() {

    // Configure GPIO2 as output push-pull
    gpio_set_direction(LED_GPIO_2, GPIO_MODE_OUTPUT);

    //------------------------------------------------------------------------------------------------------------------------------------------- PWM
    ledc_timer_config_t TIMER_PWM = {
        .duty_resolution = LEDC_TIMER_14_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg =  LEDC_AUTO_CLK
    };
    
    ledc_timer_config(&TIMER_PWM);

    OUT_PWM.channel = LEDC_TIMER_0;
    OUT_PWM.duty = 0;
    OUT_PWM.gpio_num = LED_GPIO_2;
    OUT_PWM.hpoint = 0;
    OUT_PWM.timer_sel = LEDC_TIMER_0;
    OUT_PWM.speed_mode = LEDC_LOW_SPEED_MODE;

    ledc_channel_config(&OUT_PWM);
    //------------------------------------------------------------------------------------------------------------------------------------------- PWM

    while(1) {

        //0% = 0d
        ledc_set_duty(OUT_PWM.speed_mode, OUT_PWM.channel, 0);
        ledc_update_duty(OUT_PWM.speed_mode, OUT_PWM.channel);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        //25% = 4096d (2^14bits / 4 = 16384d / 4 = 4096d)
        ledc_set_duty(OUT_PWM.speed_mode, OUT_PWM.channel, 4096);
        ledc_update_duty(OUT_PWM.speed_mode, OUT_PWM.channel);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        //50% = 8192d (2^14bits / 2 = 16384d / 2 = 8192d)
        ledc_set_duty(OUT_PWM.speed_mode, OUT_PWM.channel, 8192);
        ledc_update_duty(OUT_PWM.speed_mode, OUT_PWM.channel);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        //100% = 16384d (2^14bits = 16384d)
        ledc_set_duty(OUT_PWM.speed_mode, OUT_PWM.channel, 16384);
        ledc_update_duty(OUT_PWM.speed_mode, OUT_PWM.channel);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }   

}