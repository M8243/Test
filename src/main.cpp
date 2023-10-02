#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

#define I2C_SDA  5
#define I2C_SCL  6

TaskHandle_t taskHandle;

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
void showStockTask(void *ptParam) {
  static float stockPrice = 99.57; //股票价格
  TickType_t xLastWakeTime = xTaskGetTickCount();

  const TickType_t xFrequency = 3000; // 间隔 3000 ticks = 3 seconds

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    Serial.println(xTaskGetTickCount());
    stockPrice = stockPrice * (1 + random(1, 20) / 100.0);
    // vTaskDelay(random(500, 2000));
    Serial.print("Stock Price : $");
    Serial.println(stockPrice);
  }

}
void setup() { //loopBack , Priority 1, Core 1
  xTaskCreatePinnedToCore(oledTask, "OLED Task", 1024 * 6, NULL, 1, &taskHandle, 1);
  Serial.begin(115200);
  

  vTaskDelay(2000);
  int waterMark = uxTaskGetStackHighWaterMark(taskHandle);
  Serial.print("Task Free Memory: ");
  Serial.print(waterMark);
  Serial.println(" Bytes");
  Serial.print("Task Used Memory: ");
  Serial.print(1024*6 - waterMark);
  Serial.println(" Bytes");

  xTaskCreate(showStockTask, "Show Stock Price", 1024 * 6, NULL, 1, NULL);
  vTaskDelete(NULL);
}

void loop() {

}