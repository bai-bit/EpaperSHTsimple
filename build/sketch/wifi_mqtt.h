#line 1 "D:\\chaohe\\HiPNUC\\EpaperSHTsimple\\EpaperSHTsimple\\wifi_mqtt.h"
#ifndef __ONENET_MQTT_H__
#define __ONENET_MQTT_H__
#include <stdio.h>
#include <Arduino.h>
#include "WiFi.h"
#include "PubSubClient.h"
#include "I2C_BM8563.h"
#include "esp32_sys_time.h"

void setupWifi(char *ssid, const char *password, char *wifi_flag);
void clientReconnect(char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *m_flag);

void sendTempAndHumi(rtc_date_t *date, float *temp, float *humidity);
void callback(char *topic, byte *payload, unsigned int length);

void mqtt_init(char *mqtt_server,uint32_t port_number,char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *m_flag);
void checkout_connet(char *ssid, const char *password, char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *w_m_flag);

#endif
