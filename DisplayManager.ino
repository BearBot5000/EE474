/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: INTERFACE
*/


void DisplayManagerInit() {
  xTaskCreate(TaskDisplayManager, "Display", 128, NULL, 4, &DisplayManagerHandle);
}

void TaskDisplayManager(void *pvParameters) {
  int x = 0;
  for (;;) {
    switch (x) {
      case 0:
        setDisplay(TIME);
        break;
      case 1:
        setDisplay(TEMP);
        break;
      default:
        break;
    }
    // Pause for a bit between displays.
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Display each screen for 5 seconds. Adjust as needed.
    //a;ternate between displays after the delay
    if (x == 0) {
      x = 1;
    } else {
      x = 0;
    }
  }
}
