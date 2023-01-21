#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/timer.h"


#define LED_GPIO_2      2   //PIN 4

bool flag_led_timer = false;


void IRAM_ATTR timer_group0_isr (void *param){
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
    
    if (flag_led_timer)
    {
        gpio_set_level(LED_GPIO_2, 0);
        flag_led_timer = false;
    }
    else
   {
        gpio_set_level(LED_GPIO_2, 1);
        flag_led_timer = true;
   } 
}


void app_main() {

    // Configure GPIO2 as output push-pull
    gpio_set_direction(LED_GPIO_2, GPIO_MODE_OUTPUT);


    //------------------------------------------------------------------------------------------------------------------------------------------- TIMER
    timer_config_t timer0_cfg = {
        .divider = 80,                                      // APB clock running at 80 MHz, The divider’s range is from from 2 to 65536
        .counter_dir = TIMER_COUNT_UP,                      // TIMER_COUNT_UP, TIMER_COUNT_DOWN
        .alarm_en = TIMER_ALARM_EN,                         // TIMER_ALARM_DIS, TIMER_ALARM_EN
        .intr_type = TIMER_INTR_LEVEL,                      // TIMER_INTR_T0, TIMER_INTR_T1, TIMER_INTR_WDT, TIMER_INTR_NONE
        .auto_reload = TIMER_AUTORELOAD_EN,                 // TIMER_AUTORELOAD_EN, TIMER_AUTORELOAD_DIS 
        .counter_en = TIMER_PAUSE,                          // ENABLE, DISABLE
    };
        
    timer_init(TIMER_GROUP_0, TIMER_0, &timer0_cfg);        //0 TIMER_GROUP_0 or 1 for TIMER_GROUP_1
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);     //set timer for 0
    timer_set_counter_mode(TIMER_GROUP_0, TIMER_0, TIMER_COUNT_UP);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, &timer_group0_isr, TIMER_0, ESP_INTR_FLAG_IRAM, NULL);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000000);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_start(TIMER_GROUP_0, TIMER_0);

    // f = 1 / T
    // T = 1 / (APB clock / dividir)
    // T = 1 / (80MHz / 80)
    // T = 1us
    // for 1s delay and T = 1us -> Value = 1000000us / 1us = 1000000

    //other                                         
    //timer_pause(TIMER_GROUP_0, TIMER_0);
    //timer_set_auto_reload(TIMER_GROUP_0, TIMER_0, reload mode);
    //timer_set_divider(TIMER_GROUP_0, TIMER_0, 16);                  //APB clock running at 80 MHz, The divider’s range is from from 2 to 65536.
    //timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group0_isr, void *arg, int intr_alloc_flags)
    //timer_isr_callback_remove(TIMER_GROUP_0, TIMER_0);
    //timer_disable_intr(TIMER_GROUP_0, TIMER_0);
    //------------------------------------------------------------------------------------------------------------------------------------------- TIMER

    while(1) {}   
}