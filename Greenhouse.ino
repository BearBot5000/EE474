/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: MAIN

GREENHOUSE: Greenhouse is a final project that creates a "nearly" self-sufficient
ecosystem. There is a light sensor that will turn on LEDS to give the plant light
if the light goes out. The system will check the soil and once a day water it if the soil is below 
the desired wetness level. Prior to pumping it will ensure using a water sensor that the pump
will not turn on if the water is low. The system is constantly checking the temperature and humidity and if the
temperature gets too high it will open the lid if not already open and turn on the fan. 
The system is also monitoring constantly the wind levels, and if the wind levels become too strong
it will close the lid if open.  The ecosystem named "Planty McPlanter" additionally has an LCD which will
display the name/date/time and the temp/humidity levels. There is a keypad which acts as our human interface 
and which one can manually run all of the functions.
*/

// Including necessary libraries and modules
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <dht11.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "DHT.h"
#include <Servo.h>
#include <Wire.h>
#include <DS3231.h>


// ----------------------LCD DISPLAY----------------------------
// Initialize LiquidCrystal library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  
String currentLCDLine1 = "";
String currentLCDLine2 = "";


// ---------------------------KEYPAD----------------------------
// Define keypad layout and connections
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {29, 28, 27, 26}; 
byte colPins[COLS] = {25, 24, 23, 22};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
#define KEYBOARD_TASK_PRIORITY (tskIDLE_PRIORITY + 1)


// ---------------------------TEMP AND HUMID---------------------------
#define DHT_SENSOR_TYPE DHT11  // define sensor type as DHT11
#define DHT_SENSOR_PIN 6  // pin to which the sensor is connected
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
const int max_temperature = 90;  // maximum temperature allowed
TaskHandle_t TaskDisplayTempHumidityHandle; // task handle for displaying temperature and humidity
static unsigned long measurement_timestamp = millis( );

// ---------------------------SOIL MOISTURE---------------------------
#define moistureIn A0
#define moisturePump 3
unsigned long previousMoistureMillis = 0;
const long moistureInterval = 1000; // interval at which to refresh the moisture reading (milliseconds)
unsigned long pumpStartMillis = 0;
bool manualRun = false;
bool pumpIsRunning = false;
boolean canRunPump = true; // flag to check if the pump can be run

// --------------------------SERVO----------------------------
Servo myservo;
#define servopin 13
bool lidOpen = false;  // flag to check if lid is open


// --------------------------LED SUN----------------------------
#define sun 34
bool lightsON = false;  // flag to check if lights are on
#define photocell A3
#define LIGHT_THRESHOLD 50  // The threshold below which it's considered dark
#define DARK_TIME 5000  // The amount of time it needs to be dark before lights are turned on
unsigned long lightDetectedTime = 0;  // When light was last detected
bool isLightOn = false;  // Whether the lights are currently on
bool lightOFF = false;  // Whether the light is off


// -------------------------WATER LEVEL----------------------------
#define waterSensor A1
int waterLevel = 0;
int waterLevelHistory = 0;
bool waterLevelLow = false; //if low water


// -------------------------CLOCK----------------------------
DS3231 clock;
RTCDateTime dt;
unsigned long previousTimeMillis = 0;
const long timeInterval = 100; // interval at which to refresh the time (milliseconds)


// -------------------------WIND SENSOR----------------------------
#define windSensorAnalog A2
#define windSensorPin 33
int windAnalogValue = 0;
const int windReadings = 100;  // Number of readings to average
int windreadings[windReadings];  // Array to store the readings
int windex = 0;  // Index of the current reading
int windtotal = 0;  // Running total of the readings
int windaverage = 0;  // Average reading
bool isWindy = false;  // Boolean variable to indicate windy condition
const long winterval = 200;               // winterval at which to refresh the LCD (milliseconds)
bool lcdNeedsUpdating = false;        // flag to mark when the LCD needs to be updated


// -------------------------LID CONTROL----------------------------
#define WINDOW_LENGTH 30                    // determines sensityivity
unsigned long previousWindCheckMillis = 0;  // will store the last time we checked the wind
boolean windHistory[WINDOW_LENGTH];         // will store the history of the wind state
int windTrueCount = 0;                      // will count the number of times wind was true
int windIndex = 0;                          // will index into the windHistory array


// -------------------------OTHER----------------------------
#define fan 32
boolean fanRunning = false;  // flag to check if fan is running
#define LED 53
bool blinkEnable = true;  // flag to enable LED blink


// -------------------------DISPLAY CONTROL--------------------------
enum DisplayState {SOIL, WIND, TIME, WATER_LEVEL, TEMP};
bool displaySoil = false;
bool displayWind = false;
bool displayTime = false;
bool displayWatL = false;
bool displayTemp = false;
void setDisplay(DisplayState state) {
    // Set all to false
    displaySoil = false;
    displayWind = false;
    displayTime = false;
    displayWatL = false;
    displayTemp = false;

    // Set the selected one to true
    switch(state) {
        case SOIL:
            displaySoil = true;
            break;
        case WIND:
            displayWind = true;
            break;
        case TIME:
            displayTime = true;
            break;
        case WATER_LEVEL:
            displayWatL = true;
            break;
        case TEMP:
            displayTemp = true;
            break;
    }
}

// Global task handles
TaskHandle_t LightDetectorHandle;
TaskHandle_t TempHumidDetHandle;
TaskHandle_t TimeKeeperHandle;
TaskHandle_t SoilMoistureTaskHandle;
TaskHandle_t WindSensorHandle;
TaskHandle_t WindSampleHandle;
TaskHandle_t LEDHandle;
TaskHandle_t DisplayManagerHandle;
TaskHandle_t KeyboardHandle;

// -------------------------DEFINE TASKS----------------------------
void TaskSoilMoisture(void *pvParameters);
void TaskTemperatureCheck(void *pvParameters);
void TaskWindSensor(void *pvParameters);
void TaskBlink(void *pvParameters);
void TaskTimeKeeper(void *pvParameters);
void TaskLightDetector(void *pvParameters);
void TaskDisplayManager(void *pvParameters);
void keyboardTask(void *pvParameters);
void TaskSampleWind(void *pvParameters);
void TaskCheckWind(void *pvParameters);


void setup() {
  LCDsetup(); 
  SERVOsetup();
  fansetup();
  fakeSunsetup();
  dht.begin();

  LightDetectorInit();
  TempHumidDetInit();
  TimeKeeperInit();
  SoilMoistureTaskInit();
  WindSensorInit();
  LEDInit();
  DisplayManagerInit();
  KeyboardInit();

  vTaskStartScheduler();
}

void loop() {
  //nothing
}
