#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/uart.h"
#include "driver/gpio.h"


#define TX_UART1    GPIO_NUM_5 //PIN 10
#define RX_UART1    GPIO_NUM_4 //PIN 11
#define RTS_UART1   UART_PIN_NO_CHANGE
#define CTS_UART1   UART_PIN_NO_CHANGE
#define BUF_SIZE    (1024)


static QueueHandle_t uart1_queue;
 

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);


        for(;;) 
        {
            //Waiting for UART event
            if(xQueueReceive(uart1_queue, (void * )&event, (TickType_t)portMAX_DELAY)) 
            {
                bzero(dtmp, BUF_SIZE);

                switch(event.type) 
                {
                    // Event of UART receving data
                    /* We'd better handler data event fast, there would be much more data events than
                       other types of events. If we take too much time on data event, the queue might be full */                 
                    case UART_DATA:
                        uart_read_bytes(UART_NUM_1, dtmp, event.size, portMAX_DELAY);
                        uart_write_bytes(UART_NUM_1, (const char*) dtmp, event.size);
                        break;

                    // Event of HW FIFO overflow detected
                    case UART_FIFO_OVF:
                        /* If fifo overflow happened, you should consider adding flow control for your application
                           The ISR has already reset the rx FIFO,
                           As an example, we directly flush the rx buffer here in order to read more data */ 
                        uart_flush_input(UART_NUM_1);
                        xQueueReset(uart1_queue);
                        break;

                    // Event of UART ring buffer full
                    case UART_BUFFER_FULL:
                        /* If buffer full happened, you should consider increasing your buffer size
                           As an example, we directly flush the rx buffer here in order to read more data */ 
                        uart_flush_input(UART_NUM_1);
                        xQueueReset(uart1_queue);
                        break;

                    // Event of UART RX break detected
                    case UART_BREAK:
                        break;

                    // Event of UART parity check error
                    case UART_PARITY_ERR:
                        break;

                    // Event of UART frame error
                    case UART_FRAME_ERR:
                        break;

                    // UART_PATTERN_DET
                    case UART_PATTERN_DET:
                        break;

                    // Others
                    default:
                        break;
                }
            }
        }

        free(dtmp);

        dtmp = NULL;
        vTaskDelete(NULL);
}

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

        // Set UART configuration parameters
        uart_param_config(UART_NUM_1, &uart_config);    

        // Assign signals of a UART peripheral to GPIO pins
        uart_set_pin(UART_NUM_1, TX_UART1, RX_UART1, RTS_UART1, CTS_UART1);

        // Install UART driver, and get the queue
        uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart1_queue, 0);


        // Create a task to handler UART event from ISR
        xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}
