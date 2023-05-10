#include <Arduino.h>
#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
// include library, include base class, make path known
#include "system_config.h"
#include "SHT3x.h"
#define CONFIG_ARDUINO_LOOP_STACK_SIZE  20480
#define configMINIMAL_STACK_SIZE		    20480

#define BM8563_I2C_SDA    21
#define BM8563_I2C_SCL    22
#define TEMPRE_INFO_SIZE  32
#define SHT3X_SENSOR      0
#define DEBUG

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__) 
#else
#define LOG(...)
#endif

void at_thread_putchar(uint8_t *buf, uint32_t len, void *param, void *data);
void atCmd_init(void);
I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire1);
static I2C_BM8563_DateTypeDef dateStruct;
static I2C_BM8563_TimeTypeDef timeStruct;
char w_m_cnt_flag;
char wifi_flag;

TaskHandle_t getSensorData_handler;
TaskHandle_t wifiConnect_handler;
TaskHandle_t atcmd_handler;
rtc_date_t bm8563_date;
float temp[8];
float humidity[8];

float vbat = 0;
float vbat_percentage = 0;
char sleep_flag = 0;
struct tm timeinfo;
rtc_date_t date;

#line 40 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void tcaselect(uint8_t i);
#line 50 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void tsic_power(uint8_t sensor_status);
#line 54 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
uint8_t tsic_data_output(uint8_t pinNum);
#line 66 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void AT_CMD_Task(void *pvParamerters);
#line 96 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void WIFI_MQTT_Task(void *pvParamerters);
#line 124 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void getSensorData_Task(void *pvParamerters);
#line 272 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void setup();
#line 303 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void loop();
#line 40 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\EpaperSHTsimple.ino"
void tcaselect(uint8_t i)
{
  if (i > 7)
    return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void tsic_power(uint8_t sensor_status)
{
}

uint8_t tsic_data_output(uint8_t pinNum)
{
  return digitalRead(pinNum);
}

tsic_oper_t tsic_opt = {
    .tsic_delayus = delayMicroseconds,
    .tsic_delayms = delay,
    .tsic_power = tsic_power,
    .tsic_data_output = tsic_data_output,
};

void AT_CMD_Task(void *pvParamerters)
{
  uint8_t *atcmd = (uint8_t *)malloc(128);
  sysConfig *param = (sysConfig *)pvParamerters;

  while (true)
  {
    if (xQueueReceive(receiveData_queue, (void *)atcmd, (portTickType)portMAX_DELAY))
    {
      //发送AT+CMD进入配置模式
      //发送AT+QUIT退出配置模式
      at_thread_putchar(atcmd, strlen((const char*)atcmd), param, NULL);
      
      if (param->temp_param.output_flag == 0 && getSensorData_handler != NULL )
      {
        param->temp_param.output_flag = 2;
        printf("Enter Configuration Mode\r\n");
        vTaskSuspend(getSensorData_handler);
      }
      else if (param->temp_param.output_flag == 1 && getSensorData_handler != NULL )
      {
        param->temp_param.output_flag = 3;
        vTaskResume(getSensorData_handler);
        printf("Exit Configuration Mode\r\n");
      }
    }
    memset(atcmd, 0, sizeof(atcmd));
  }
}

void WIFI_MQTT_Task(void *pvParamerters)
{
  sysConfig *param = (sysConfig *)pvParamerters;

  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  while(!param->cfg_param.work_mode)
  {
    delay(3000);
  }

  setupWifi(param->cfg_param.wifi_name, param->cfg_param.wifi_password, &w_m_cnt_flag);     /* 连接WIFI */

  mqtt_init(param->cfg_param.mqtt_server, param->cfg_param.port_number, param->cfg_param.mqtt_devid, param->cfg_param.mqtt_pubid, param->cfg_param.mqtt_password, &w_m_cnt_flag); /* 链接MQTT*/

  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);                  /* 获取网络时间并设置本地RTC的时间 */
  
  wifi_flag = 1;
  xTaskNotifyGive(getSensorData_handler);

  while(1)
  {
    delay(1000);
  }

}


void getSensorData_Task(void *pvParamerters)
{
  /**/
  char buf[TEMPRE_INFO_SIZE] = "";
  int ret = 0;
  float tem, hum;
  int a = 0, b = 0, c = 0;
  
  sysConfig *param = (sysConfig *)pvParamerters;

  while(true)
  {
    rtc.begin();
    rtc.clearIRQ();
    // rtc.SetAlarmIRQ(60);

    rtc.getDate(&dateStruct);
    delay(10);
    rtc.getTime(&timeStruct);

    bm8563_date.year    = dateStruct.year;
    bm8563_date.month   = dateStruct.month;
    bm8563_date.day     = dateStruct.date;
    bm8563_date.hour    = timeStruct.hours;
    bm8563_date.minute  = timeStruct.minutes;
    bm8563_date.second  = timeStruct.seconds;

    a = param->cfg_param.timeToSleep;
    if((b = a - timeStruct.seconds) > 3)
      rtc.SetAlarmIRQ(b - 2);
    else
      rtc.SetAlarmIRQ(a - 3);
    // rtc.SetAlarmIRQ(60);

    LOG("get bm8563 %d-%d-%d %d:%d:%d\r\n", bm8563_date.year, bm8563_date.month, bm8563_date.day, bm8563_date.hour, bm8563_date.minute, bm8563_date.second);
    
    show_info_text();

    memset(temp, 0, sizeof(float) * 8);
    digitalWrite(SENSOR_POWER, HIGH);   /* 用一个io口来控制电源 */
    delay(5);

    for (int i = 0; i < 8; i++)
    {
      ret = getTemperatureSensor_eight(i, &temp[i]);

      if (ret)
      {
        LOG("read sensor error: %d  ", ret);
      }
      else
        read_tsic506_status |= 1 << i;

      LOG("%d : %0.2f\r\n", i + 1, temp[i]);

      delay(5);
    }

#if SHT3X_SENSOR
    for (uint8_t i = 0; i < 8; i++)
    {
      tcaselect(4);
      delay(1);

      SHT3x Sensor;

      Sensor.UpdateData();
      tem = Sensor.GetTemperature();
      hum = Sensor.GetRelHumidity();
      // temp[i] = Sensor.GetTemperature();
      humidity[i] = Sensor.GetRelHumidity();
    }
#endif

    putchar(10);

    digitalWrite(SENSOR_POWER, LOW);

    vbat = analogRead(35);
    vbat = vbat * (3.50F/2048.0F);
    if(vbat <= 3.2) vbat = 3.2;
    if(vbat >= 4.2) vbat = 4.2;
    vbat_percentage = (vbat - 3.2) / (4.2 - 3.2) * 100;
    LOG("vbat = %.2f %.2f%%\r\n", vbat, vbat_percentage);

    show_info_sensor_data(&bm8563_date, temp,vbat, vbat_percentage);

    if(!param->cfg_param.work_mode || param->cfg_param.timeToSleep < 30)
    {
      printf("delay time\r\n");
      delay(param->cfg_param.timeToSleep * 1000);

    }
    else
    {
      printf("break\r\n");
      break;
    }
      
  }
  
  disableGpio();

  xTaskNotifyGive(wifiConnect_handler);

  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

  while(wifi_flag != 1)
  {
    delay(200);
  }

  if (getLocalTime(&timeinfo))   //获取时间，这个时间是从网络上获取到之后，配置到本地的rtc时间
  {
    //获取时间成功，把时间配置到时钟芯片，并写入flash中
    dateStruct.weekDay = timeinfo.tm_wday;
    dateStruct.month   = timeinfo.tm_mon + 1;
    dateStruct.date    = timeinfo.tm_mday;
    dateStruct.year    = timeinfo.tm_year + 1900;
    rtc.setDate(&dateStruct);

    timeStruct.hours   = timeinfo.tm_hour;
    timeStruct.minutes = timeinfo.tm_min;
    timeStruct.seconds = timeinfo.tm_sec;
    rtc.setTime(&timeStruct);

    // //写入flash
    date.year    = dateStruct.year;
    date.month   = dateStruct.month;
    date.day     = dateStruct.date;
    date.hour    = timeStruct.hours;
    date.minute  = timeStruct.minutes;
    date.second  = timeStruct.seconds;
    
    LOG("wifi date %d-%d-%d %d:%d:%d\r\n", date.year, date.month, date.day, date.hour, date.minute, date.second);
  }

  checkout_connet(param->cfg_param.wifi_name, param->cfg_param.wifi_password, param->cfg_param.mqtt_devid, param->cfg_param.mqtt_pubid, param->cfg_param.mqtt_password, &w_m_cnt_flag);
  show_info_wifi_mqtt_status(w_m_cnt_flag);
  
  wifi_flag = 2;
  while(1)
  {
    delay(2000);
  }
}

sysConfig *param = (sysConfig *)malloc(sizeof(sysConfig));
void setup()
{
  boardInit();                                                               /* 初始化gpio口 */
  SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI);                                    /* 初始化SPI总线 */
  spi_flash_init();                                                          /* 初始化flash */
  Wire1.begin(BM8563_I2C_SDA, BM8563_I2C_SCL);                               /* 初始化IIC总线 */

  /* 获取配置 */
  memset(param, 0, sizeof(sysConfig));
  sysConfig_init(param, sizeof(sysConfig));
 
  atCmd_init();                                                                   /* 初始化AT指令 */
  esp_uart_init(param->cfg_param.serialBaud);                                     /* 初始化串口 */
  displayInit();                                                                  /* 初始化墨水屏 */
  xTaskCreate(AT_CMD_Task, "AT_CMD_Task", 10240, (void *)param, 3, &atcmd_handler);          /* 创建AT指令线程 */
       
  LOG("System start init \r\n");

  Tsic_Init(&tsic_opt);                                                           /* 初始化TSIC506温度传感器 */

  // esp_sleep_enable_timer_wakeup(param->cfg_param.timeToSleep * param->cfg_param.usToS_factor);              /* 内部时钟唤醒 */
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0);                                                                  /* 外部触发唤醒 */
  // uint64_t ext1_wakeup_pin_mask = 1ULL << 2;
  // esp_sleep_enable_ext1_wakeup(ext1_wakeup_pin_mask, ESP_EXT1_WAKEUP_ALL_LOW);
  LOG("Setup ESP32 to sleep for every %d Seconds \r\n", param->cfg_param.timeToSleep);

  xTaskCreate(WIFI_MQTT_Task, "Wifi_Mqtt_Task", 10240, (void *)param, 1, &wifiConnect_handler);                /* 创建 WIFI 线程 */
  xTaskCreate(getSensorData_Task, "getSensorData_Task", 30000, (void *)param, 2, &getSensorData_handler);      /* 创建 读取传感器线程 */

}

void loop()
{
  while(wifi_flag != 2)
  {
    delay(200);
  }

  sendTempAndHumi(&bm8563_date, temp, humidity);

  wifi_flag = 0;

  vTaskDelete(wifiConnect_handler);

  vTaskDelete(atcmd_handler);

  vTaskDelete(getSensorData_handler);

  delay(500);
  free(param);
  param = NULL;
  printf("Going to sleep now\r\n");
  esp_deep_sleep_start();
  // client.loop(); //客户端循环检测
  delay(30000);
}

