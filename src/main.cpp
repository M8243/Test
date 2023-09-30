#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

#define I2C_SDA  5
#define I2C_SCL  6

void oledTask(void * pvParam) {
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* I2C_SCL=*/ I2C_SCL, /* I2C_SDA=*/ I2C_SDA);
  u8g2.begin();
  u8g2.enableUTF8Print();//允许打印中文
  for (;;) {
    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_ncenB12_tr);	// choose a suitable font
    u8g2.drawStr(15, 30, "LOVE  YOU");	// write something to the internal memory
    u8g2.setFont(u8g2_font_wqy14_t_gb2312b);//切换中文字体
    u8g2.drawUTF8(40,50,"小毛毛");
    u8g2.sendBuffer();					// transfer internal memory to the display
    vTaskDelay(1000);
  }
}

void setup() { //loopBack , Priority 1, Core 1
  xTaskCreatePinnedToCore(oledTask, "OLED Task", 1024 * 6, NULL, 1, NULL, 1);

  vTaskDelete(NULL);
}

void loop() {

}