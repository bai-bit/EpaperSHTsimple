#ifndef __ESP_UART_H__
#define __ESP_UART_H__

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#define EX_UART_NUM UART_NUM_0
extern QueueHandle_t receiveData_queue;

void esp_uart_init(uint32_t baud);
#endif