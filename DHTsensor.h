#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <Ticker.h>

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

/**************************************************************/
/* Example how to read DHT sensors from an ESP32 using multi- */
/* tasking.                                                   */
/* This example depends on the Ticker library to wake up      */
/* the task every 20 seconds                                  */
/**************************************************************/

DHTesp dht;

void tempTask(void *pvParameters);
bool getTemperature();
void triggerGetTemp();

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Ticker for temperature reading */
Ticker tempTicker;
/** Comfort profile */
ComfortState cf;
/** Flag if task should run */
bool tasksEnabled = false;
/** Pin number for DHT11 data pin */
int dhtPin = 23;

/**
 * initTemp
 * Setup DHT library
 * Setup task and timer for repeated measurement
 * @return bool
 *    true if task and timer are started
 *    false if task or timer couldn't be started
 */

class DHT{ 
public:
  float humidity;
  float temperature;
  float vpd;

  bool initTemp() {
    byte resultValue = 0;
    // Initialize temperature sensor
    dht.setup(dhtPin, DHTesp::DHT22);
    Serial.println("DHT initiated");

    // Start task to get temperature
    xTaskCreatePinnedToCore(
        DHT::tempTask,                  /* Function to implement the task */
        "tempTask ",                    /* Name of the task */
        4000,                           /* Stack size in words */
        this,                           /* Task input parameter */
        5,                              /* Priority of the task */
        &tempTaskHandle,                /* Task handle. */
        1);                             /* Core where the task should run */

    if (tempTaskHandle == NULL) {
      Serial.println("Failed to start task for temperature update");
      return false;
    } else {
      // Start update of environment data every 20 seconds
      tempTicker.attach(20, DHT::triggerGetTemp);
    }
    return true;
  }

  /**
  * triggerGetTemp
  * Sets flag dhtUpdated to true for handling in loop()
  * called by Ticker getTempTimer
  */
  static void triggerGetTemp() {
    if (tempTaskHandle != NULL) {
      xTaskResumeFromISR(tempTaskHandle);
    }
  }

  /**
  * Task to reads temperature from DHT11 sensor
  * @param pvParameters
  *    pointer to task parameters
  */
  static void tempTask(void *pvParameters) {
    DHT *self = static_cast<DHT*>(pvParameters);
    self->tempTaskImpl();
  }

  void tempTaskImpl() {
    Serial.println("tempTask loop started");
    while (1) // tempTask loop
    {
      if (tasksEnabled) {
        // Get temperature values
        getTemperature();
      }
      // Got sleep again
      vTaskSuspend(NULL);
    }
  }

  float computeVPD(float temperature, float humditiy){
    // Calculate saturation vapor pressure (SVP)
    float svp = 0.6108 * exp((17.27 * temperature) / (temperature + 237.3));
    
    // Calculate vapor pressure deficit (VPD)
    float vpd = svp * (1 - (humidity / 100.0));

    return vpd;
  }

  /**
  * getTemperature
  * Reads temperature from DHT11 sensor
  * @return bool
  *    true if temperature could be aquired
  *    false if aquisition failed
  */
  bool getTemperature() {
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    TempAndHumidity newValues = dht.getTempAndHumidity();
    // Check if any reads failed and exit early (to try again).
    if (dht.getStatus() != 0) {
      Serial.println("DHT11 error status: " + String(dht.getStatusString()));
      return false;
    }

    humidity = newValues.humidity;
    temperature = 5/9*(newValues.temperature - 32); // C = 5/9 x (F-32)
    float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
    float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
    float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);
    vpd = computeVPD(newValues.temperature, newValues.humidity);

    String comfortStatus;
    switch(cf) {
      case Comfort_OK:
        comfortStatus = "Comfort_OK";
        break;
      case Comfort_TooHot:
        comfortStatus = "Comfort_TooHot";
        break;
      case Comfort_TooCold:
        comfortStatus = "Comfort_TooCold";
        break;
      case Comfort_TooDry:
        comfortStatus = "Comfort_TooDry";
        break;
      case Comfort_TooHumid:
        comfortStatus = "Comfort_TooHumid";
        break;
      case Comfort_HotAndHumid:
        comfortStatus = "Comfort_HotAndHumid";
        break;
      case Comfort_HotAndDry:
        comfortStatus = "Comfort_HotAndDry";
        break;
      case Comfort_ColdAndHumid:
        comfortStatus = "Comfort_ColdAndHumid";
        break;
      case Comfort_ColdAndDry:
        comfortStatus = "Comfort_ColdAndDry";
        break;
      default:
        comfortStatus = "Unknown:";
        break;
    };

    Serial.println(" T:" + String(newValues.temperature) + " H:" + String(newValues.humidity) + " I:" + String(heatIndex) + " D:" + String(dewPoint) + " " + comfortStatus + " VPD:" + String(vpd));
    return true;
  }

  void setup()
  {
    Serial.begin(115200);
    Serial.println();
    Serial.println("DHT ESP32 example with tasks");
    initTemp();
    // Signal end of setup() to tasks
    tasksEnabled = true;
  }

  void loop() {
    if (!tasksEnabled) {
      // Wait 2 seconds to let system settle down
      delay(2000);
      // Enable task that will read values from the DHT sensor
      tasksEnabled = true;
      if (tempTaskHandle != NULL) {
        vTaskResume(tempTaskHandle);
      }
    }
    yield();
  }
};
