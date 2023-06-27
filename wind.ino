/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: SAFETY CONTROLS
*/


void WindSensorInit() {
  xTaskCreate(TaskSampleWind, "SampleWind", 128, NULL, 3, &WindSampleHandle);
  xTaskCreate(TaskCheckWind, "CheckWind", 128, NULL, 2, &WindSensorHandle);
}
void windy(int wind) {
  // Subtract the oldest reading from the total
  windtotal -= windreadings[windex];
  // Store the new reading in the array
  windreadings[windex] = windAnalogValue;
  // Add the new reading to the total
  windtotal += windreadings[windex];
  // Move to the next position in the array
  windex = (windex + 1) % windReadings;
  // Calculate the average reading
  windaverage = windtotal / windReadings;
  boolean newIsWindy = (windaverage > 500);
  if (newIsWindy != isWindy) {
    isWindy = newIsWindy;
    lcdNeedsUpdating = true;
  }
}

// task to sample wind sensor
void TaskSampleWind(void *pvParameters) {
  windaverage = 0;
  for (;;) {
    // Read wind sensor
    windAnalogValue = analogRead(windSensorAnalog);
    // Record the value in the wind history
    windy(windAnalogValue);
    // Delay for a bit before next sample
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

// task to check wind history and update LCD
void TaskCheckWind(void *pvParameters) {
  for (;;) {
    // Check if it's been windy for the majority of the time and lid is not already closed
    if (windaverage > (WINDOW_LENGTH / 2)) {
      closeLID();
      windtotal = 0;
    }
    // Update the LCD if necessary
    if (isWindy) {
      String line1 = "WIND READ: ";
      line1 += String(windAnalogValue);
      String line2;
      if (isWindy) {
        line2 = "IT'S WINDY!!";
      }
      //print the windy statement
      if (isWindy) {
        printToLCD(line1, line2);
        lcdNeedsUpdating = false;
      }
    }
    // Delay for a bit before next check
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

