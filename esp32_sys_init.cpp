#include "esp32_sys_init.h"

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

uint16_t checkout_param(cfgParam *read_data, size_t len)
{
  if(read_data->work_mode != 0 && read_data->work_mode != 1)
    read_data->work_mode = 1;
  if(read_data->serialBaud != 9600 && read_data->serialBaud != 115200 && read_data->serialBaud != 230400 && read_data->serialBaud != 460800 && read_data->serialBaud != 921600)
  {
    read_data->serialBaud = 115200;
  }

  if(read_data->device_id < 0)
  {
    read_data->device_id = 1;
  }

  if(read_data->port_number < 0 && read_data->port_number > 65535)
  {
    read_data->port_number = 6002;
  }

  if(read_data->usToS_factor < 0)
  {
    read_data->usToS_factor = 1000000;
  }

  if(read_data->timeToSleep < 0)
  {
    read_data->timeToSleep = 60;
  }

  return 0;
}


esp_err_t resetSysConfig(cfgParam *param)
{
  int32_t ret;
  cfgParam sysConfig_data;
  memset((void *)&sysConfig_data, 0, sizeof(sysConfig_data));

  uint16_t currectCrc = 0;
  sysConfig_data.work_mode = 1;
  sysConfig_data.device_id = 1;
  sysConfig_data.serialBaud = 115200;
  sysConfig_data.port_number = 6002;
  memcpy(sysConfig_data.wifi_name, "TP-LINK_914C", sizeof("TP-LINK_914C"));
  memcpy(sysConfig_data.wifi_password, "chaohedianzi", sizeof("chaohedianzi"));

  // memcpy(sysConfig_data.wifi_name, "TP-LINK_0728", sizeof("TP-LINK_0728"));
  // memcpy(sysConfig_data.wifi_password, "203160507@jiafeimao", sizeof("203160507@jiafeimao"));

  memcpy(sysConfig_data.mqtt_server, "183.230.40.39", sizeof("183.230.40.39"));
  memcpy(sysConfig_data.mqtt_devid, "1012769315", sizeof("1012769315"));
  memcpy(sysConfig_data.mqtt_pubid, "554949", sizeof("554949"));
  memcpy(sysConfig_data.mqtt_password, "TSIC50x", sizeof("TSIC50x"));

  sysConfig_data.usToS_factor = 1000000; 
  sysConfig_data.timeToSleep = 60;       
  sysConfig_data.crcValue = 0;
  crc16_update(&currectCrc, (uint8_t *)&sysConfig_data, sizeof(sysConfig_data));
  sysConfig_data.crcValue = currectCrc;

  
  printf("currectCrc value : %d\r\n", sysConfig_data.crcValue );
  spi_flash_erase_sector(SEC_1);
  ret = spi_flash_write(SEC_1 * 4 * 1024 + SEC_OFFSET, (char *)&sysConfig_data, sizeof(sysConfig_data));

  spi_flash_erase_sector(SEC_2);
  ret = spi_flash_write(SEC_2 * 4 * 1024 + SEC_OFFSET, (char *)&sysConfig_data, sizeof(sysConfig_data));

  memcpy((param), &(sysConfig_data), sizeof(sysConfig_data));

  return ret;
}


uint8_t getSysConfig(cfgParam *read_data, size_t len)
{
  cfgParam read_data_temp_1, read_data_temp_2;
  uint16_t crc_1 = 0, crc_2 = 0;
  uint16_t tempCrc_1 = 0, tempCrc_2 = 0;

  spi_flash_read(SEC_1 * 4 * 1024 + SEC_OFFSET, (char *)&read_data_temp_1, sizeof(read_data_temp_1));
  spi_flash_read(SEC_2 * 4 * 1024 + SEC_OFFSET, (char *)&read_data_temp_2, sizeof(read_data_temp_2));
  crc_1 = read_data_temp_1.crcValue;
  crc_2 = read_data_temp_2.crcValue;

  read_data_temp_1.crcValue = 0;
  read_data_temp_2.crcValue = 0;

  crc16_update(&tempCrc_1, (uint8_t *)&(read_data_temp_1), sizeof(read_data_temp_1));
  crc16_update(&tempCrc_2, (uint8_t *)&(read_data_temp_2), sizeof(read_data_temp_2));

  if(crc_1 == crc_2)
  {
    memcpy((cfgParam *)(read_data), (cfgParam *)&(read_data_temp_1), sizeof(read_data_temp_1));
  }
  else if(tempCrc_1 == crc_1)
  {
    memcpy((cfgParam *)(read_data), (cfgParam *)&(read_data_temp_1), sizeof(read_data_temp_1));
  }
  else if(tempCrc_2 == crc_2)
  {
    memcpy((cfgParam *)(read_data), (cfgParam *)&(read_data_temp_2), sizeof(read_data_temp_2));
  }
  else 
  {
    printf("reset sys config\r\n");
    resetSysConfig(read_data);
  }

  checkout_param(read_data, len);

  return 0;
}


uint8_t sysConfig_writeFlash(cfgParam *write_data)
{
  uint8_t ret = 0;
  uint16_t currectCrc = 0;
  write_data->crcValue = 0;

  crc16_update(&currectCrc, (uint8_t *)write_data, sizeof(cfgParam));
  write_data->crcValue = currectCrc;

  spi_flash_erase_sector(SEC_1);
  ret = spi_flash_write(SEC_1 * 4 * 1024 + SEC_OFFSET, (char *)write_data, sizeof(cfgParam));

  spi_flash_erase_sector(SEC_2);
  ret = spi_flash_write(SEC_2 * 4 * 1024 + SEC_OFFSET, (char *)write_data, sizeof(cfgParam));

  return ret;
}


size_t getFlashSize()
{
  return spi_flash_get_chip_size();
}

 
void sysConfig_init(sysConfig *read_data, size_t len)
{
  getSysConfig(&(read_data->cfg_param), sizeof(read_data->cfg_param));
  read_data->temp_param.output_flag = 3;
}
