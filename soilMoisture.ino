/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: WATERING SYSTEM
*/

void SoilMoistureTaskInit() {
  xTaskCreate(TaskSoilMoisture, "SoilMoisture", 1000, NULL, 2, &SoilMoistureTaskHandle);
}

void TaskSoilMoisture(void *pvParameters) {
  pinMode(moisturePump, OUTPUT);
  pinMode(moistureIn, INPUT);
  digitalWrite(moisturePump, HIGH);

  for (;;)  // A Task shall never return or exit.
  {
    dt = clock.getDateTime();

    // Run this task every day at 0800
    if ((dt.hour == 8 && dt.minute == 0) || manualRun) {

      waterLevelGet();
      float moistureValue = analogRead(moistureIn);
      if (moistureValue < 600 && !waterLevelLow) {
        // If it's dry and the water level is not low, start the pump
        lcd.print("!!");
        digitalWrite(moisturePump, LOW);
        pumpStartMillis = millis();  // Remember when the pump was started
        pumpIsRunning = true;        // Remember that the pump is running
        setDisplay(SOIL);
      } else {
        digitalWrite(moisturePump, HIGH);
        pumpIsRunning = false;  // The pump is not running
        manualRun = false;
      }

      if (displaySoil) {
        printToLCD("Moisture Level", String(moistureValue));
      }
      // Check if the pump has been running for 5 seconds
      if (pumpIsRunning && millis() - pumpStartMillis >= 1500) {
        // If the pump has been running for 3 seconds, stop it
        digitalWrite(moisturePump, LOW);
        pumpIsRunning = false;
        manualRun = false;
      }

      // delay task for one minute after executing to avoid repetition during the 8th hour
      vTaskDelay(60 * 1000 / portTICK_PERIOD_MS);
    }

    // small delay to prevent the task from occupying the CPU all the time
    vTaskDelay(pdMS_TO_TICKS(100));  // delay for 1 second
  }
}
