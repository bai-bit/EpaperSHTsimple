#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\boardSensor.h"
#ifndef __BOARD_SENSOR_H__
#define __BOARD_SENSOR_H__

#include <Arduino.h>
#include "driver/rtc_io.h"
#include "tsic50x_driver.h"

#define SENSOR_POWER 32 
#define KEY1 11
#define SENSOR_1_J10 13
#define SENSOR_2_J16 15
#define SENSOR_3_J7 25
#define SENSOR_4_J13 33
#define SENSOR_5_J8 27
#define SENSOR_6_J14 26
#define SENSOR_7_J9 0
#define SENSOR_8_J15 12
#define LED_IO19 19

typedef struct
{
  uint8_t reg_val;
  uint8_t pinNum;
} sensor_temprature_ctlPin;

static sensor_temprature_ctlPin sensor_temprature_list[] =
{
  {1, SENSOR_1_J10},
  {2, SENSOR_2_J16},
  {3, SENSOR_3_J7},
  {4, SENSOR_4_J13},
  {5, SENSOR_5_J8},
  {6, SENSOR_6_J14},
  {7, SENSOR_7_J9},
  {8, SENSOR_8_J15}
};

void boardInit(void);
void disableGpio(void);
uint8_t getTemperatureSensor_eight(uint8_t pinNum, float *value);

#endif
