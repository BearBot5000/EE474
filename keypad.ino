/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: INTERFACE
*/

void KeyboardInit() {
  xTaskCreate(keyboardTask, "KeyboardTask", 1000, NULL, 3, &KeyboardHandle);
}

void keyboardTask(void *pvParameters) {
  for (;;) {
    char customKey = customKeypad.getKey();
    //display the time
    if (customKey == 'A') {
      setDisplay(TIME);
    }
    // disokay the temp and humidity
    if (customKey == 'B') {
      setDisplay(TEMP);
    }
    // display water level
    if (customKey == 'C') {
      waterLevelGet();
    }
    //manually turn on and off lights
    if (customKey == '1') {
      if (lightsON) {
        fakeSunOFF();
        lightsON = false;
      } else {
        fakeSunON();
        lightsON = true;
      }
    }
    // manually turn on and off fan
    if (customKey == '2') {
      if (fanRunning) {
        fanOFF();
        fanRunning = false;
      } else {
        fanON();
        fanRunning = true;
      }
    }
    // manually open and close the lid
    if (customKey == '3') {
      if (lidOpen) {
        closeLID();
        lidOpen = false;
      } else {
        openLID();
        lidOpen = true;
      }
    }
    //manually turn on and off the low light function
    if (customKey == '4') {
      //toggle bool lightOFF = false;
      if (lightOFF) {
        lightOFF = false;
      } else {
        lightOFF = true;
      }
    }
    //manually turn on and off the blinking LED
    if (customKey == '5') {
      // Toggle LED blink
      if (blinkEnable) {
        blinkEnable = false;
      } else {
        blinkEnable = true;
      }
    }
    // display soil moisture level
    if (customKey == '6') {
      float moistureValue = analogRead(moistureIn);
      setDisplay(SOIL);
      printToLCD("Moisture Level", String(moistureValue));
    }
    // manually turn on and off the pump
    if (customKey == '7') {
      // Toggle manual run
      if (manualRun) {
        manualRun = false;
      } else {
        manualRun = true;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));  // delay for 1 second
  }
}