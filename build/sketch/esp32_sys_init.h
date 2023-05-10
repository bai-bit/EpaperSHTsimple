#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\esp32_sys_init.h"
#ifndef __ESP32_SYS_INIT_H__
#define __ESP32_SYS_INIT_H__
#include <esp_spi_flash.h>
#include <string.h>

#define LEN 1
#define SEC_1 500
#define SEC_2 501

#define SEC_OFFSET 0

typedef struct {
        uint8_t work_mode;    //0:  normal    1:sleep mode
        uint32_t device_id;
        uint32_t serialBaud;
        char wifi_name[32];
        char wifi_password[32];

        char mqtt_server[32];     //onenet 的 IP地址
        int port_number;
        char mqtt_devid[32];      //设备ID
        char mqtt_pubid[32];      //产品ID
        char mqtt_password[32];   //鉴权信息

        uint32_t  usToS_factor;  // Conversion factor for micro seconds to seconds 
        uint32_t  timeToSleep;   // 睡眠时长
        uint16_t crcValue;
    }cfgParam;

typedef struct {
        uint8_t output_flag;
    }tempParam;

typedef struct 
{
    cfgParam cfg_param;
    tempParam temp_param;
}sysConfig;

void sysConfig_init(sysConfig *read_data, size_t len);
uint8_t sysConfig_writeFlash(cfgParam *write_data);
esp_err_t resetSysConfig(cfgParam *param);
  
#endif
