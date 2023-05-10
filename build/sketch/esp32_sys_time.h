#line 1 "D:\\chaohe\\HiPNUC\\EpaperSHTsimple\\EpaperSHTsimple\\esp32_sys_time.h"
#ifndef __ESP32_SYS_TIME_H__
#define __ESP32_SYS_TIME_H__
#include <esp_spi_flash.h>
#include <string.h>

#define LEN 1
#define SEC_3 502
#define SEC_4 503
#define SEC_OFFSET 0

typedef struct _rtc_date_t {
    uint16_t  year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   minute;
    uint8_t   second;
    uint16_t  crcValue;
}rtc_date_t;


uint8_t getSysTime(rtc_date_t *read_data, size_t len);
uint8_t sysTime_writeFlash(rtc_date_t *write_data);

#endif