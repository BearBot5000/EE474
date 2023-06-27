/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: ENVIRONMENTAL CONTROLS
*/

void TempHumidDetInit() {
  
  xTaskCreate(TaskTemperatureCheck, "TemperatureCheck", 2000, NULL, 3, &TempHumidDetHandle);
}


// this task measures the temp and humidity and if temp is above
// max will open the lid and turn on the fan
void TaskTemperatureCheck(void *pvParameters) {
  float temperature;
  float humidity;

  // Task loop
  for (;;) {

    /* Measure once every four seconds */
    if (xTaskGetTickCount() - measurement_timestamp > pdMS_TO_TICKS(1500)) {

      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
      measurement_timestamp = xTaskGetTickCount();
      // Convert temperature to Fahrenheit
      float temperatureF = celsiusToFahrenheit(temperature);
      // If temperature is too high, open lid and turn fan on
      if (temperatureF > max_temperature && !lidOpen) {
        openLID();
        fanON();
      }
      if (displayTemp) {
        // Update LCD with temperature and humidity
        String tempStr = "Temp:  " + String(temperatureF) + " F";
        String humidityStr = "Humid: " + String(humidity) + " %";
        printToLCD(tempStr, humidityStr);
      }
    }
    vTaskDelay(1);  // Delay for 1 tick to prevent WDT reset.
  }
}


//helper task to convert celsius to Farenheit
float celsiusToFahrenheit(float celsius) {
  return ((celsius * 9.0) / 5.0) + 32.0;
}
