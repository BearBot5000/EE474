/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: ENVIRONMENTAL CONTROLS
*/


void LightDetectorInit() {
  xTaskCreate(TaskLightDetector, "PhotoCell", 128, NULL, 3, &LightDetectorHandle);
}


void TaskLightDetector(void *pvParameters) {
  for (;;) {
    int lightValuePrev = analogRead(photocell);
    if (!isLightOn) {
      vTaskDelay(pdMS_TO_TICKS(DARK_TIME));  // delay for DARK_TIME ms, then check again
      int lightValueNext = analogRead(photocell);
      if (lightValueNext < LIGHT_THRESHOLD) {  // if it is still dark, turn on the light
        // It's been dark for at least DARK_TIME and lights are not on
        // Turn on lights if current time is not between 22:00 and 08:00 from timeKeeper
        if (!lightOFF) {
          fakeSunON();
          isLightOn = true;
        }
      }
    }
    if (lightValuePrev > LIGHT_THRESHOLD) {
      // If lights are on, turn them off
      if (isLightOn) {
        lcd.print(lightValuePrev);
        fakeSunOFF();
        isLightOn = false;
      }
    }
  }
  vTaskDelay(pdMS_TO_TICKS(2000));
}
