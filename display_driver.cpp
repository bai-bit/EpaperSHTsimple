#include "display_driver.h"

GxIO_Class io(SPI, ELINK_SS, ELINK_DC, ELINK_RESET);
GxEPD_Class display(io, ELINK_RESET, ELINK_BUSY);

uint8_t read_tsic506_status = 0;

const GFXfont *fonts[] = {
    &FreeSans9pt7b};

void displayInit(void)
{
  display.init();
  display.setRotation(1);
  display.eraseDisplay();
  
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&DEFALUT_FONT);
  display.setTextSize(1);
}

void displayText(const String &str, int16_t y, uint8_t alignment)
{
  int16_t x = 0;
  int16_t x1, y1;
  uint16_t w, h;
  display.setCursor(x, y);
  display.getTextBounds(str, x, y, &x1, &y1, &w, &h);

  switch (alignment)
  {
  case RIGHT_ALIGNMENT:
    display.setCursor(display.width() - w - x1, y);
    break;
  case LEFT_ALIGNMENT:
    display.setCursor(0, y);
    break;
  case CENTER_ALIGNMENT:
    display.setCursor(display.width() / 2 - ((w + x1) / 2), y);
    break;
  default:
    break;
  }
  display.println(str);
}

void show_info_text(void)
{
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);

  display.setCursor(10, 15);
  display.print("Start read sensor -->");
  display.setCursor(10, 35);
  display.print("   TSIC506");
  display.setCursor(10, 55);
  display.print("Start read Voltage -->");
  display.setCursor(10, 75);
  display.print("   BATTERY");

  display.update();
}

void show_info_wifi_mqtt_status(uint8_t net_status)
{
  uint16_t box_x = 160;
  uint16_t box_y =10;
  uint16_t box_w = 80;
  uint16_t box_h = 18;
  display.setRotation(1);
  display.updatefillRect(box_x, box_y, box_w, box_h, true);
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(170, 15);
  switch(net_status)
  {
    case 0:
        display.print("Wi? Mq?");
        break;
    case 1:
        display.print("Wi  Mq?");
    break;
    case 2:
    break;
    case 3:
      display.print("Wi  Mq");
    break;
    default:
    break;
  }

  display.updateWindow(box_x, box_y, box_w, box_h, true);
  delay(200);
}

void show_info_read_sensor_num(uint16_t num)
{
  uint16_t box_x = 145;
  uint16_t box_y = 40;
  uint16_t box_w = 90;
  uint16_t box_h = 80;
  display.setRotation(1);
  display.updatefillRect(box_x, box_y, box_w, box_h, true);
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(150, 55);
  display.print(num, 200);
  display.updateWindow(box_x, box_y, box_w, box_h, true);
  delay(200);
}

void show_info_sensor_data(rtc_date_t *date, float *temp, float vbat, float percentage)
{
  char timeStringBuff[50] = ""; // 50 chars should be enough

  snprintf(timeStringBuff, sizeof(timeStringBuff), "%d-%d-%d %02d:%02d:%02d", date->year, date->month, date->day, date->hour, date->minute, date->second);

  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  printf("%s\r\n", timeStringBuff);

  displayText(timeStringBuff, 18, LEFT_ALIGNMENT);
  // displayText(msgJson, 37, LEFT_ALIGNMENT);

  for(int i = 0; i < 8; i++)
  {
    if(read_tsic506_status & 1 << i)
    {
        display.setCursor(15 + i % 4 * 60, 37 + i / 4 * 40);
        display.print('T');
        display.setCursor(25 + i % 4 * 60, 37 + i / 4 * 40);
        display.print(i + 1);
        display.setCursor(i % 4 * 60, 57 + i / 4 * 40);
        display.print(temp[i]);
    }
  }

  display.setCursor(10, 120);
  display.print("Voltage:");
  display.setCursor(80, 120);
  display.print(vbat);
  display.setCursor(130, 120);
  display.print(percentage);
  display.setCursor(185, 120);
  display.print("%");

  display.update();
}