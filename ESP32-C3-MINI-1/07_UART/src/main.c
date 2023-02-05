#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"


#define TX_UART1    GPIO_NUM_5 //PIN 10
#define RX_UART1    GPIO_NUM_4 //PIN 11
#define RTS_UART1   UART_PIN_NO_CHANGE
#define CTS_UART1   UART_PIN_NO_CHANGE
#define BUF_SIZE    (1024)



void app_main(void)
{
    // Configure parameters of an UART driver, communication pins and install the driver
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    int intr_alloc_flags = 0;


    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TX_UART1, RX_UART1, RTS_UART1, CTS_UART1);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_1, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        
        if (len) {
        // Write data back to the UART
            uart_write_bytes(UART_NUM_1, (const char *) data, len);
            len = 0;
        }
    } 
}
