#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <string.h>

#include "wifi_mqtt.h"
#include "SPI.h"
#include "Ticker.h"
#include "Esp.h"
#include "esp32_sys_init.h"
#include "Wire.h"

#include "tsic50x_driver.h"
#include "esp_uart.h"
#include "boardSensor.h"
#include "esp32_sys_time.h"
#include "display_driver.h"

#define TCAADDR 0x70

extern char wifi_flag;

Ticker tim1; //定时器,用来循环上传数据


const char *ntpServer = "time.windows.com";
const long gmtOffset_sec = 28800;
const int daylightOffset_sec = 0;

#endif
