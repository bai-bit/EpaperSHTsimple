#line 1 "D:\\chaohe\\HiPNUC\\EpaperSHTsimple\\EpaperSHTsimple\\boardSensor.cpp"
#include "boardSensor.h"

void boardInit(void)
{
  // rtc_gpio_init(GPIO_NUM_32);
  // rtc_gpio_init(GPIO_NUM_11);
  // rtc_gpio_init(GPIO_NUM_13);
  // rtc_gpio_init(GPIO_NUM_0);
  // rtc_gpio_init(GPIO_NUM_15);
  // rtc_gpio_init(GPIO_NUM_25);
  // rtc_gpio_init(GPIO_NUM_33);
  // rtc_gpio_init(GPIO_NUM_27);
  // rtc_gpio_init(GPIO_NUM_26);
  // rtc_gpio_init(GPIO_NUM_12);


  pinMode(SENSOR_POWER, OUTPUT);
  pinMode(KEY1, INPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(2,  INPUT_PULLUP);
  pinMode(35, INPUT);
}

void disableGpio(void)
{
  pinMode(SENSOR_POWER, INPUT_PULLDOWN);
  #if 0
  pinMode(13, INPUT_PULLDOWN);
  pinMode(0, INPUT_PULLDOWN);
  pinMode(15, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  pinMode(33, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(12, INPUT_PULLDOWN);
  #else
  pinMode(13, ANALOG);
  pinMode(0, ANALOG);
  pinMode(15, ANALOG);
  pinMode(25, ANALOG);
  pinMode(33, ANALOG);
  pinMode(27, ANALOG);
  pinMode(26, ANALOG);
  pinMode(12, ANALOG);

  // rtc_gpio_isolate(GPIO_NUM_32);
  // rtc_gpio_isolate(GPIO_NUM_11);
  // rtc_gpio_isolate(GPIO_NUM_13);
  // rtc_gpio_isolate(GPIO_NUM_0);
  // rtc_gpio_isolate(GPIO_NUM_15);
  // rtc_gpio_isolate(GPIO_NUM_25);
  // rtc_gpio_isolate(GPIO_NUM_33);
  // rtc_gpio_isolate(GPIO_NUM_27);
  // rtc_gpio_isolate(GPIO_NUM_26);
  // rtc_gpio_isolate(GPIO_NUM_12);

  #endif
}

uint8_t readTemperature_tsic50x(uint8_t pinNum, float *value)
{
  uint8_t ret = 0;
  float count = 0;
  int loop_num = 3;
  ret = read_tsic506_byte(pinNum, &count);

  while (loop_num-- && ret || count > 60 || count < -10)
  {
    ret = read_tsic506_byte(pinNum, &count);
  }
  if (ret || count > 60 || count < -10)
  {
    value[0] =  0;
    return ret;
  }
  else
    value[0] = count;

  return ret;
}

uint8_t getTemperatureSensor_eight(uint8_t sensorNum, float *value)
{
  uint8_t ret = 0;
  float data[3];
  uint8_t i = 2;

  while (i--)
  {
    ret = readTemperature_tsic50x(sensor_temprature_list[sensorNum].pinNum, &data[0]);
    delay(1);
    ret = readTemperature_tsic50x(sensor_temprature_list[sensorNum].pinNum, &data[1]);
    delay(1);
    ret = readTemperature_tsic50x(sensor_temprature_list[sensorNum].pinNum, &data[2]);
    delay(1);

    if (data[0] - data[1] < 5 && data[1] - data[2] < 5 && data[2] - data[0] < 5)
    {
      value[0] = data[2];
      return ret;
    }
  }

  if (!i)
  {
    printf("this %d sensor read error, please check\r\n", sensor_temprature_list[i].pinNum);
  }

  return ret;
}


