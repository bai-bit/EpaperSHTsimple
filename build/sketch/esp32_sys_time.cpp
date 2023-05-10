#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\esp32_sys_time.cpp"
#include "esp32_sys_time.h"

static void crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes)
{
  uint32_t crc = *currectCrc;
  uint32_t j;
  for (j = 0; j < lengthInBytes; ++j)
  {
    uint32_t i;
    uint32_t byte = src[j];
    crc ^= byte << 8;
    for (i = 0; i < 8; ++i)
    {
      uint32_t temp = crc << 1;
      if (crc & 0x8000)
      {
        temp ^= 0x1021;
      }
      crc = temp;
    }
  }
  *currectCrc = crc;
}

uint8_t getSysTime(rtc_date_t *read_data, size_t len)
{
  rtc_date_t read_data_temp_1, read_data_temp_2;
  uint16_t crc_1 = 0, crc_2 = 0;
  uint16_t tempCrc_1 = 0, tempCrc_2 = 0;

  spi_flash_read(SEC_3 * 4 * 1024 + SEC_OFFSET, (char *)&read_data_temp_1, sizeof(read_data_temp_1));
  spi_flash_read(SEC_4 * 4 * 1024 + SEC_OFFSET, (char *)&read_data_temp_2, sizeof(read_data_temp_2));
  crc_1 = read_data_temp_1.crcValue;
  crc_2 = read_data_temp_2.crcValue;

  read_data_temp_1.crcValue = 0;
  read_data_temp_2.crcValue = 0;

  crc16_update(&tempCrc_1, (uint8_t *)&(read_data_temp_1), sizeof(read_data_temp_1));
  crc16_update(&tempCrc_2, (uint8_t *)&(read_data_temp_2), sizeof(read_data_temp_2));

  if(crc_1 == crc_2)
  {
    memcpy((rtc_date_t *)(read_data), (rtc_date_t *)&(read_data_temp_1), sizeof(read_data_temp_1));
    return 0;
  }
  else if(tempCrc_1 == crc_1)
  {
    memcpy((rtc_date_t *)(read_data), (rtc_date_t *)&(read_data_temp_1), sizeof(read_data_temp_1));
    return 0;
  }
  else if(tempCrc_2 == crc_2)
  {
    memcpy((rtc_date_t *)(read_data), (rtc_date_t *)&(read_data_temp_2), sizeof(read_data_temp_2));
    return 0;
  }
  else 
  {
    printf("get system time error\r\n");
  }

  return 1;
}


uint8_t sysTime_writeFlash(rtc_date_t *write_data)
{
  uint8_t ret = 0;
  uint16_t currectCrc = 0;
  write_data->crcValue = 0;

  crc16_update(&currectCrc, (uint8_t *)write_data, sizeof(rtc_date_t));
  write_data->crcValue = currectCrc;

  spi_flash_erase_sector(SEC_3);
  ret = spi_flash_write(SEC_3 * 4 * 1024 + SEC_OFFSET, (char *)write_data, sizeof(rtc_date_t));

  spi_flash_erase_sector(SEC_4);
  ret = spi_flash_write(SEC_4 * 4 * 1024 + SEC_OFFSET, (char *)write_data, sizeof(rtc_date_t));

  return ret;
}

