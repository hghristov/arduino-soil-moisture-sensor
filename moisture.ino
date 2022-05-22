// libs required to display stuff on the 0.96" 128x64 OLED screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1); // screen doesn't have a reset pin, so set to nonexisting -1 pin

#define SCREEN_WIDTH = 128
#define SCREEN_HEIGHT = 64

#define SENSOR_ANALOG_READ A0
#define SENSOR_POWER_PIN 7
#define SENSOR_READINGS_COUNT 100
#define SENSOR_POWER_ON_TIME 100 // milliseconds the sensor is on, no idea how much time it needs to take readings
#define SENSOR_READ_INTERVAL 1000 * 2
#define CALIBRATION_AIR_VALUE 990   // sensor goes to 1023 but after average of 100 readings value drops to ~990 because first readings are wrong
#define CALIBRATION_WATER_VALUE 380 // seen my sensor readings vary from 260 to 400 when dipped in water

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_ANALOG_READ, INPUT);
  pinMode(SENSOR_POWER_PIN, OUTPUT); // use if sensor is connected to a digital pin on the Arduino
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
  // readAnalogAndPrint() // use if sensor is connected to an analog pin on the Arduino
  readAnalogInIntervals(); // use if sensor is connected to a digital pin on the Arduino
  delay(SENSOR_READ_INTERVAL);
}

/**
 * Function to dead analog data by powering the sensor only for a certain amount of time
 * This increases the sensor's life by only using it whenever we want to make a reading
 * To be used when sensor is connected to a digital pin
 */
void readAnalogInIntervals()
{
  digitalWrite(SENSOR_POWER_PIN, HIGH); // powers the sensor on
  readAnalogAndPrint();
  delay(SENSOR_POWER_ON_TIME);
  digitalWrite(SENSOR_POWER_PIN, LOW); // powers the sensor off
}

/**
 * Function that prints the sensor's values to the Serial Monitor
 */
void readAnalogAndPrint()
{
  int sensorValue = 0;
  int moisturePercent = 0;

  sensorValue = getAverageOfAnalogReadings(SENSOR_READINGS_COUNT);
  moisturePercent = map(sensorValue, CALIBRATION_AIR_VALUE, CALIBRATION_WATER_VALUE, 0, 100);

  if (moisturePercent > 100) // don't display values over 100%
    moisturePercent = 100;

  if (moisturePercent < 0) // don't display values under 0%
    moisturePercent = 0;

  Serial.print(sensorValue);
  Serial.print(", ");
  Serial.print(moisturePercent);
  Serial.println("%");

  displayPercentage(moisturePercent); // show value on display
}

/**
 * Function to get the average of a number of sensor readings
 * First couple of values returned by the sensor are usually inaccurate so we get the average of multiple readings
 * @param readingsCount
 * @return the averaged value of the specified number of readings
 */
int getAverageOfAnalogReadings(int readingsCount)
{
  long value = 0; // we're summing up potentially hundreds of values that go up to around 1000, so int is not enough
  for (int i = 0; i < readingsCount; i++)
  {
    value = value + analogRead(SENSOR_ANALOG_READ);
    delay(1);
  }
  return value / readingsCount;
}

/**
 * Function to display the moisture percentage to a connected display
 * @param percent
 */
void displayPercentage(int percent)
{
  // Display Text
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 1);
  display.setTextColor(WHITE);
  display.println("MOISTURE");
  display.setTextSize(2);
  display.setCursor(0, 12);
  display.print(percent);
  display.println("%");
  display.display();
}
