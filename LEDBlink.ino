/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: SAFETY CONTROLS
*/


void LEDInit() {
  xTaskCreate(TaskBlink, "LED", 128, NULL, 3, &LEDHandle);
}

//CUURENTLY SET TO BLINK TASK FOR SET INSTRUCTIONS BUT WILL CHANGE
// TO LOW LEVEL WATER ALERT
void TaskBlink(void *pvParameters) {
  pinMode(53, OUTPUT);
  for (;;) {
    if (blinkEnable) {
      digitalWrite(53, HIGH);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      digitalWrite(53, LOW);
      vTaskDelay(200 / portTICK_PERIOD_MS);

    } else {
      //if task turned off ensures does't hpg cpu time
      vTaskDelay(pdMS_TO_TICKS(3000));
    }
  }
}