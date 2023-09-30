#include <Arduino.h>

// 养成良好习惯，被多进程和中断调用的变量使用 volatile 修饰符
volatile uint32_t inventory = 100; //总库存
volatile uint32_t retailCount = 0; //线下销售量


void retailTask(void *pvParam) {
  while (1) {

    //以下实现了带有随机延迟的 inventory减1；
    //等效为 inventory--； retailCount++；
    uint32_t inv = inventory;
    for (int i; i < random(10, 100); i++) vTaskDelay(pdMS_TO_TICKS(i));
    if (inventory > 0) {
      inventory = inv - 1;
      retailCount++;
    }

  };

  vTaskDelay(10); //老板要求慢一些，客户升级后，可以再加快速度
}



void showTask(void *pvParam) {
  while (1) {

    printf("Inventory : %d\n", inventory);
    printf("  Retail : %d\n", retailCount);


    if (inventory == 0 ) {
      printf("\n-----SALES SUMMARY-----\n");
      printf("  Total Sales:  %d\n\n", retailCount);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreate(retailTask,
              "Online Channel",
              1024 * 4,
              NULL,
              1,
              NULL);

  xTaskCreate(showTask,
              "Display Inventory",
              1024 * 4,
              NULL,
              1,
              NULL);

}

void loop() {
}