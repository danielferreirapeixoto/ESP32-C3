#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

static esp_adc_cal_characteristics_t *adc_chars;

void app_main() {

    //--------------------------------------------------------------------------------------------------------------------------------------------------- ADC
    int adc_reading = 0, voltage = 0;
    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL_1, ADC_ATTEN_DB_11); // GPIO1/ADC1_1, ATENUACAO_DB_11
    /*
        +-----------------+--------------------------------+
        Attenuation         Measurable input voltage range
        +-----------------+--------------------------------+
        ADC_ATTEN_DB_0      100 mV ~ 950 mV
        ADC_ATTEN_DB_2_5    100 mV ~ 1250 mV
        ADC_ATTEN_DB_6      150 mV ~ 1750 mV
        ADC_ATTEN_DB_11     150 mV ~ 2450 mV
        +-----------------+--------------------------------+      

        +----------+-------------+-----------------+
        |          | attenuation | suggested range |
        |    SoC   |     (dB)    |      (mV)       |
        +==========+=============+=================+
        |          |       0     |    100 ~  950   |
        |          +-------------+-----------------+
        |          |      2.5    |    100 ~ 1250   |
        |   ESP32  +-------------+-----------------+
        |          |       6     |    150 ~ 1750   |
        |          +-------------+-----------------+
        |          |      11     |    150 ~ 2450   |
        +----------+-------------+-----------------+
        |          |       0     |      0 ~  750   |
        |          +-------------+-----------------+
        |          |      2.5    |      0 ~ 1050   |
        | ESP32-S2 +-------------+-----------------+
        |          |       6     |      0 ~ 1300   |
        |          +-------------+-----------------+
        |          |      11     |      0 ~ 2500   |
        +----------+-------------+-----------------+

        VREF = 1.1V = 1100mV
    */

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
    //--------------------------------------------------------------------------------------------------------------------------------------------------- ADC


    while (1) {

        adc_reading = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_1);
        voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    
    }

}