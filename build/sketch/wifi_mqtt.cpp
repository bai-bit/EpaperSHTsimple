#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\wifi_mqtt.cpp"
#include "wifi_mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient); // 创建一个PubSub客户端, 传入创建的WIFI客户端

static uint32_t isrWifiReady;

void setupWifi(char *ssid, const char *password, char *wifi_flag)
{
  if (isrWifiReady > 0)
  {
    printf("ok\r\n WIFI connect success\r\n");
  }
  else
  {
    delay(10);
    printf("connect WIFI\r\n");

    WiFi.begin(ssid, password);
    while (!WiFi.isConnected())
    {
      printf(".");
      delay(3000);
      WiFi.begin(ssid, password);
      isrWifiReady++;
      if (isrWifiReady > 3)
      { //为了在不连接网络的时候进行温度的测量和显示
        printf("WIFI connect failed\r\n");
        isrWifiReady = 0;
        break;
      }
    }
  }

  if(WiFi.isConnected())
  {
    *wifi_flag = 1;
    printf("WIFI connect success\r\n");
    
  }
  else
  {
    *wifi_flag = 0;
    printf("WIFI connect failed\r\n");
  }
}

void clientReconnect(char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *m_flag)
{
  int count = 5;
  while (!client.connected() && count-- > 0) //再重连客户端
  {
    if (isrWifiReady == 0)
    {
      printf("wifi not connected\r\n");
      delay(200);
      break;
    }

    printf("reconnect MQTT ...\r\n");
    if (client.connect(mqtt_devid, mqtt_pubid, mqtt_password))
    {
      printf("connected \r\n");
    }
    else
    {
      printf("failed\r\n");
      printf("%d", client.state());
      printf("try again in 2 sec\r\n");
      delay(2000);
    }
  }

  if(WiFi.isConnected() && !client.connected())
    *m_flag = 1;
  else if(WiFi.isConnected() && client.connected())
    *m_flag = 3;
  else
    *m_flag = 0;

  if(count == 0)
  {
    printf("MQTT connect failed\r\n");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  printf("message rev : %s\r\n", topic);
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    putchar(payload[i]);
  }
  Serial.println();
  putchar(10);
}
  char msg_buf[500];                                                                                                                                                                                                         //发送信息缓冲区
  char dataTemplate[] = "{\"t1\":%.2f,\"h1\":%.2f,\"t2\":%.2f,\"h2\":%.2f,\"t3\":%.2f,\"h3\":%.2f,\"t4\":%.2f,\"h4\":%.2f,\"t5\":%.2f,\"h5\":%.2f,\"t6\":%.2f,\"h6\":%.2f,\"t7\":%.2f,\"t8\":%.2f,\"Time\":%02d%c%02d%c%02d%c}"; //信息模板
  char msgJson[500]; 
  char datestr[] ="hms";                                                                                                                                                                                                        //要发送的json格式的数据
  unsigned short json_len = 0;
//向主题发送模拟的温湿度数据
void sendTempAndHumi(rtc_date_t *date, float *temp, float *humidity)
{
  snprintf(msgJson, 500, dataTemplate, temp[0], humidity[0], temp[1], humidity[1], temp[2], humidity[2], temp[3], humidity[3], temp[4], humidity[4], temp[5], humidity[5], temp[6], temp[7], date->hour, datestr[0], date->minute, datestr[1], date->second, datestr[2]);

  if (client.connected())
  {
    // delay(200);
    json_len = strlen(msgJson);                    // msgJson的长度
    msg_buf[0] = char(0x03);                       //要发送的数据必须按照ONENET的要求发送, 根据要求,数据第一位是3
    msg_buf[1] = char(json_len >> 8);              //数据第二位是要发送的数据长度的高八位
    msg_buf[2] = char(json_len & 0xff);            //数据第三位是要发送数据的长度的低八位
    memcpy(msg_buf + 3, msgJson, strlen(msgJson)); //从msg_buf的第四位开始,放入要传的数据msgJson
    msg_buf[3 + strlen(msgJson)] = 0;              //添加一个0作为最后一位, 这样要发送的msg_buf准备好了

    printf("public message : %s\r\n", msgJson);
    client.publish("$dp", (uint8_t *)msg_buf, 3 + strlen(msgJson)); //发送数据到主题$dp
    printf("send success\r\n");
    // delay(500);
  }
  else
  {
    printf("mqtt error\r\n");
    //将温度数据写入flash中
  }
}

void checkout_connet(char *ssid, const char *password, char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *w_m_flag)
{
  if (!WiFi.isConnected()) //先看WIFI是否还在连接
  {
    setupWifi(ssid, password, w_m_flag);
  }

  if (!client.connected()) //如果客户端没连接ONENET, 重新连接
  {
    clientReconnect(mqtt_devid, mqtt_pubid, mqtt_password, w_m_flag);
  }
}

void mqtt_init(char *mqtt_server, uint32_t port_number, char *mqtt_devid, const char *mqtt_pubid, const char *mqtt_password, char *m_flag)
{
  client.setServer(mqtt_server, port_number);             //设置客户端连接的服务器,连接Onenet服务器, 使用6002端口
  client.connect(mqtt_devid, mqtt_pubid, mqtt_password);  //客户端连接到指定的产品的指定设备.同时输入鉴权信息
  client.setCallback(callback);                           //设置好客户端收到信息是的回调
 // clientReconnect(mqtt_devid, mqtt_pubid, mqtt_password, m_flag);
}
