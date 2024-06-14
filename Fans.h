#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlockNot.h>

// Define the OneWire bus pin (the data pin of the DS18B20 sensor)
#define ONE_WIRE_BUS 9  

// Create instances for OneWire and DallasTemperature classes
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BlockNot updateTimer(1, SECONDS);

// Motor A and Motor B control pins
#define IN1 2  // Motor A direction control
#define IN2 4  // Motor A direction control
#define PWMA 3 // Motor A speed control (PWM)

#define IN3 5   // Motor B direction control
#define IN4 7   // Motor B direction control
#define PWMB 6  // Motor B speed control (PWM)

// Standby pin
#define STBY 8  // Pin to enable/disable motors

// Relay pin
#define RELAY_PIN 3 // Pin to control a relay

// Temperature thresholds
const float TEMP_START = 28.0; // Temperature at which STBY is set to HIGH
const float TEMP_MAX = 35.0;   // Temperature at which PWM reaches maximum value
const float TEMP_LOW = 18.0;   // Temperature below which the relay is activated

// Temperature offset
const float TEMP_OFFSET = 0.0; // Example offset, adjust as needed

class Fans{
public:
  void setup() {
      // Set all control pins as OUTPUT
      pinMode(IN1, OUTPUT);
      pinMode(IN2, OUTPUT);
      pinMode(PWMA, OUTPUT);
      pinMode(IN3, OUTPUT);
      pinMode(IN4, OUTPUT);
      pinMode(PWMB, OUTPUT);
      pinMode(STBY, OUTPUT);
      pinMode(RELAY_PIN, OUTPUT);

      // Initialize the temperature sensor
      sensors.begin();
      
      // Initialize the serial monitor for debugging
      Serial.begin(9600);

      // Set initial states for standby and relay
      digitalWrite(STBY, LOW);   // Disable motors initially
      digitalWrite(RELAY_PIN, LOW); // Deactivate relay initially
  }

  void loop() {
    if(updateTimer.TRIGGERED){
      // Request temperature readings from the DS18B20
      sensors.requestTemperatures();

      // Read the temperature in Celsius and apply the offset
      float temperatureC = sensors.getTempCByIndex(0) + TEMP_OFFSET;

      // Output temperature reading to the serial monitor
      Serial.print("Temperature: ");
      Serial.print(temperatureC);
      Serial.println(" °C");

      // Control the relay based on temperature
      if (temperatureC < TEMP_LOW) {
          digitalWrite(RELAY_PIN, HIGH); // Activate relay if temperature is below 20 degrees (with offset)
      } else {
          digitalWrite(RELAY_PIN, LOW); // Deactivate relay otherwise
      }

      // Determine if the standby pin should be high
      if (temperatureC >= TEMP_START) {
          digitalWrite(STBY, HIGH); // Enable motors when temperature is at or above 28 degrees

          // Calculate PWM based on temperature with offset
          int motorSpeed = (temperatureC >= TEMP_MAX)
              ? 255 // Maximum speed at or above 35 degrees
              : map(temperatureC, TEMP_START, TEMP_MAX, 100, 255);

          Serial.print("Motor Speed: ");
          Serial.println(motorSpeed);

          // Set motor A direction and speed
          digitalWrite(IN1, HIGH);  // Motor A direction
          digitalWrite(IN2, LOW);   // Motor A direction
          analogWrite(PWMA, motorSpeed); // Adjust PWM for Motor A

          // Set motor B direction and speed
          digitalWrite(IN3, HIGH);  // Motor B direction
          digitalWrite(IN4, LOW);   // Motor B direction
          analogWrite(PWMB, motorSpeed); // Adjust PWM for Motor B

      } else {
          digitalWrite(STBY, LOW); // Disable motors when temperature is below 28 degrees
          
          // Ensure motor speed is set to zero when STBY is LOW
          analogWrite(PWMA, 0); // Stop Motor A
          analogWrite(PWMB, 0); // Stop Motor B

          // Set motor direction to neutral when STBY is LOW
          digitalWrite(IN1, LOW); 
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
      }
  }
  }
};