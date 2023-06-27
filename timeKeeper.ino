/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: INTERFACE
*/

void TimeKeeperInit() {
  xTaskCreate(TaskTimeKeeper, "TimeKeeperTask", 1000, NULL, 3, &TimeKeeperHandle);
}


void TaskTimeKeeper(void *pvParameters) {
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  for (;;) {  // Start of task loop
    unsigned long currentMillis = millis();
    if (currentMillis - previousTimeMillis >= timeInterval) {
      previousTimeMillis = currentMillis;
      dt = clock.getDateTime();
      // Convert time to string
      String timeStr = String(dt.month) + "-" + String(dt.day) + "-" + String(dt.year - 2000) +
                     " " + String(dt.hour) + ":" + String(dt.minute) + ":" + String(dt.second);
      if (displayTime) {
        printToLCD("Planty Mcplanter", timeStr);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // Small delay to prevent the task from occupying the CPU all the time
  }
}
