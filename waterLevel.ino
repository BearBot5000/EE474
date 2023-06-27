/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: SAFETY CONTROLS
*/


//map for returning the water level value 0-35 as a percentage 0-100
int mapValue(int value) {
  return map(value, 0, 350, 1, 100);
}

void waterLevelChange() {
  int previousWaterLevel = waterLevel;
  waterLevel = analogRead(waterSensor);
  //map the water level to a 0-100% value
  waterLevel = mapValue(waterLevel);

  if (((previousWaterLevel >= waterLevel) && ((previousWaterLevel - waterLevel) > 10)) ||
       ((previousWaterLevel < waterLevel) && ((waterLevel - previousWaterLevel) > 10))) {
    printToLCD("Water Level", String(waterLevel));
  }
}

void waterLevelGet() {
  waterLevel = analogRead(waterSensor);
  waterLevel = mapValue(waterLevel);
  setDisplay(WATER_LEVEL);
  //value set to 60 due to innacurate readings when rapidly drained
  if (waterLevel < 60) {
    waterLevelLow = true;
    //stop the pump form running if the level is too low
    canRunPump = false;
    blinkEnable = true;
    if (displayWatL) {
      printToLCD("WATER LEVEL LOW!", "Refill Water =)");
    }
  } else {
    //enable the pump to run
    canRunPump = true;
    if (displayWatL) {
      printToLCD("Water Level", String(waterLevel) + " % ");
    }
  }
}
