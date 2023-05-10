#include <stdio.h>
#include "Esp.h"
#include "esp32_sys_init.h"

uint8_t checkout_output_flag(sysConfig *param)
{
  if(param->temp_param.output_flag != 2)
  {
    printf("Please input AT+CMD,Enter Configuration Mode!\r\n");
    return 1;
  }

  return 0;
}

int BUILD(int argc, char *const argv[])
{
  /*  */
  printf(" %s\r\n", __DATE__);

  return 0;
}

int RST(int argc, char *const argv[])
{
  /* 复位单片机 */
  printf("esp reset\r\n");
  esp_restart();

  return 0;
}

int CMD(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  param->temp_param.output_flag = 0;

  return 0;
}
int QUIT(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  param->temp_param.output_flag = 1;

  return 0;
}

int INFO(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
    if(checkout_output_flag(param))
    return 1;

  printf("%-16s %d\r\n", "Mode:", param->cfg_param.work_mode);
  printf("%-16s %d\r\n", "Device ID:", param->cfg_param.device_id);
  printf("%-16s %d\r\n", "Serial baud:", param->cfg_param.serialBaud);
  printf("%-16s %s\r\n", "Wifi name:", param->cfg_param.wifi_name);
  printf("%-16s %s\r\n", "Wifi password:", param->cfg_param.wifi_password);
  printf("%-16s %s\r\n", "Mqtt server:", param->cfg_param.mqtt_server);
  printf("%-16s %d\r\n", "Port number:", param->cfg_param.port_number);
  printf("%-16s %s\r\n", "Mqtt devid:", param->cfg_param.mqtt_devid);
  printf("%-16s %s\r\n", "Mqtt pubid:", param->cfg_param.mqtt_pubid);
  printf("%-16s %s\r\n", "Mqtt password:", param->cfg_param.mqtt_password);
  printf("%-16s %d\r\n", "usToS factor:", param->cfg_param.usToS_factor);
  printf("%-16s %d\r\n", "timeToSleep:", param->cfg_param.timeToSleep);
}

int ODR(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  return 0;
}

int BAUD(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  uint32_t val = strtoul(argv[2]+1, 0, 0);
  if(checkout_output_flag(param))
    return 1;

  switch(val)
  {
    case 9600:
      printf("val = [%d]\r\n", val);
      param->cfg_param.serialBaud = 9600;
      break;
    case 115200:
      printf("val = [%d]\r\n", val);
      param->cfg_param.serialBaud = 115200;
      break;
    case 230400:
      printf("val = [%d]\r\n", val);
      param->cfg_param.serialBaud = 230400;
      break;
    case 460800:
      printf("val = [%d]\r\n", val);
      param->cfg_param.serialBaud = 460800;
      break;
    case 921600:
      printf("val = [%d]\r\n", val);
      param->cfg_param.serialBaud = 921600;
      break;
    default:
      printf("%-16s %d\r\n", "Serial baud:", param->cfg_param.serialBaud);
      printf("Configuration error,please input again!\r\n");
  }

  sysConfig_writeFlash(&(param->cfg_param));
  return 0;
}

int ID(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  uint32_t val = strtoul(argv[2]+1, 0, 0);
  if(checkout_output_flag(param))
    return 1;
  param->cfg_param.device_id = val;
  printf("%-16s %d\r\n", "Device ID:", param->cfg_param.device_id);

  sysConfig_writeFlash(&(param->cfg_param));
  return 0;
}

int MODE(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;
  uint8_t val = strtoul(argv[2] + 1, 0, 0);
  if(val != 0 && val != 1)
  {
    val = 1;
    printf("Input error,please input 0 or 1\r\n");
  }
    
  param->cfg_param.work_mode = val;
  printf("%-16s %d\r\n", "Modo:", param->cfg_param.work_mode);
  sysConfig_writeFlash(&(param->cfg_param));
  return 0;
}

int CLEAN(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  resetSysConfig(&(param->cfg_param));
  return 0;
}

int WINAME(int argc, char *const argv[])
{
  char winame[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];

  if(checkout_output_flag(param))
    return 1;

  sscanf(argv[2]+1, " %s", winame);
  printf("wifi name : [%s]\r\n", winame);
  if((len = strlen(winame)) > 31)
    len = 31;

  memcpy(param->cfg_param.wifi_name, winame, len);
  param->cfg_param.wifi_name[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int WIPASSWORD(int argc, char *const argv[])
{
  char wipw[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];

  if(checkout_output_flag(param))
    return 1;

  sscanf(argv[2] + 1, "%32s", wipw);
  printf("wifi password : [%s]\r\n", wipw);
  if((len = strlen(wipw)) > 31)
    len = 31;

  memcpy(param->cfg_param.wifi_password, wipw, len);
  param->cfg_param.wifi_password[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int MQSERVER(int argc, char *const argv[])
{
  char mqser[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  sscanf(argv[2] + 1, "%32s", mqser);
  printf("mqtt server : [%s]\r\n", mqser);
  if((len = strlen(mqser)) > 31)
    len = 31;

  memcpy(param->cfg_param.mqtt_server, mqser, len);
  param->cfg_param.mqtt_server[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int MQDEVID(int argc, char *const argv[])
{
  char mqdev[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;
  
  sscanf(argv[2] + 1, "%32s", mqdev);
  printf("mqtt devid : [%s]\r\n", mqdev);
  if((len = strlen(mqdev)) > 31)
    len = 31;

  memcpy(param->cfg_param.mqtt_devid, mqdev, len);
  param->cfg_param.mqtt_devid[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int MQPUBID(int argc, char *const argv[])
{
  char mqpub[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  sscanf(argv[2] + 1, "%32s", mqpub);
  printf("mqtt pubid : [%s]\r\n", mqpub);
  if((len = strlen(mqpub)) > 31)
    len = 31;
 
  memcpy(param->cfg_param.mqtt_pubid, mqpub, len);
  param->cfg_param.mqtt_pubid[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int MQPASSWORD(int argc, char *const argv[])
{
  char mqpw[32] = "";
  int len = 0;
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  sscanf(argv[2] + 1, "%32s", mqpw);
  printf("mqtt password : [%s]\r\n", mqpw);
  if((len = strlen(mqpw)) > 31)
    len = 31;

  memcpy(param->cfg_param.mqtt_password, mqpw, len);
  param->cfg_param.mqtt_password[len] = '\0';
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int MQPNUM(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  int val = strtoul(argv[2]+1, 0, 0);
  printf("port number val = [%d]\r\n", val);
  param->cfg_param.port_number = val;
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int USFACTOR(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  int val = strtoul(argv[2]+1, 0, 0);
  if(val < 0)
  {
    printf("The value entered must be greater than zero\r\n");
    return 0;
  }

  printf("factor val = [%d]\r\n", val);
  param->cfg_param.usToS_factor = val;
  sysConfig_writeFlash(&param->cfg_param);

  return 0;
}

int TIMESLEEP(int argc, char *const argv[])
{
  sysConfig *param = (sysConfig *)argv[0];
  if(checkout_output_flag(param))
    return 1;

  int val = strtoul(argv[2]+1, 0, 0);
  if(val < 0 && val > (24 * 3600))
  {
    printf("The entered value is too large\r\n");
    return 0;
  }
   
  printf("time val = [%d]\r\n", val);
  param->cfg_param.timeToSleep = val;
  sysConfig_writeFlash(&(param->cfg_param));
  return 0;
}
