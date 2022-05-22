#define SENSOR_ANALOG_READ A0
#define SENSOR_POWER_PIN 7
#define SENSOR_POWER_ON_TIME 50
#define SENSOR_READ_INTERVAL 1000 * 5
#define SENSOR_READINGS_COUNT 100
#define CALIBRATION_AIR_VALUE 990
#define CALIBRATION_WATER_VALUE 380

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_ANALOG_READ, INPUT);
  pinMode(SENSOR_POWER_PIN, OUTPUT); // use if sensor is connected to a digital pin on the Arduino
}

void loop()
{
  // readAnalogAndPrint() // use if sensor is connected to an analog pin on the Arduino
  readAnalogInIntervals(); // use if sensor is connected to a digital pin on the Arduino
  delay(SENSOR_READ_INTERVAL);
}

/**
 * Function to dead analog data by powering the sensor only for a certain amount of time
 * This increases the sensor's life
 * To use, connect sensor to a digital pin
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
}

/**
 * Function to get the average of a number of sensor readings
 * First couple of values returned by the sensor are usually inaccurate so we get the average of multiple readings
 * @param readingsCount
 * @return the averaged value of the specified number of readings
 */
int getAverageOfAnalogReadings(int readingsCount)
{
  long value = 0;
  for (int i = 0; i < readingsCount; i++)
  {
    value = value + analogRead(SENSOR_ANALOG_READ);
    delay(1);
  }
  return value / readingsCount;
}
