/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: Reset
*/

void resetTasks() {
  //delete all tasks
  vTaskDelete(LightDetectorHandle);
  vTaskDelete(TempHumidDetHandle);
  vTaskDelete(TimeKeeperHandle);
  vTaskDelete(SoilMoistureTaskHandle);
  vTaskDelete(WindSampleHandle);
  vTaskDelete(WindSensorHandle);
  vTaskDelete(LEDHandle);
  vTaskDelete(DisplayManagerHandle);
  vTaskDelete(KeyboardHandle);

  //restart all tasks
  LightDetectorInit();  // Make sure these functions recreate tasks and update respective task handles.
  TempHumidDetInit();
  TimeKeeperInit();
  SoilMoistureTaskInit();
  WindSensorInit();
  LEDInit();
  DisplayManagerInit();
  KeyboardInit();

  vTaskStartScheduler();
}