#line 1 "D:\\chaohe\\HiPNUC\\EpaperSHTsimple\\EpaperSHTsimple\\display_driver.h"
#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__
#include <stdio.h>
#include <Arduino.h>
#include <GxEPD.h>

// #include GxEPD_BitmapExamples
 #include <Fonts/FreeSans9pt7b.h>
// #include <Fonts/FreeMonoBold9pt7b.h>
// #include <Fonts/FreeMonoBold12pt7b.h>
// #include <Fonts/FreeMonoBold18pt7b.h>
// #include <Fonts/FreeMonoBold24pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "board_def.h"
#include "esp32_sys_time.h"


#define DEFALUT_FONT FreeSans9pt7b

typedef enum
{
  RIGHT_ALIGNMENT = 0,
  LEFT_ALIGNMENT,
  CENTER_ALIGNMENT,
} Text_alignment;
extern uint8_t read_tsic506_status;

void displayInit(void);
void displayText(const String &str, int16_t y, uint8_t alignment);
void show_info_text(void);
void show_info_read_sensor_num(uint16_t num);
void show_info_sensor_data(rtc_date_t *date, float *temp, float vbat, float percentage);
void show_info_wifi_mqtt_status(uint8_t net_status);

#endif