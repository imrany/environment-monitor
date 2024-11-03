#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Dust Sensor settings
const int dustSensorPin = 4;   // Analog pin connected to Vo (Pin 4 of the dust sensor)
const int ledPowerPin = 9;      // Digital pin to control the LED (Pin 3 of the dust sensor)
unsigned long samplingTime = 280;  // Duration to keep the LED on for dust sampling
unsigned long deltaTime = 40;      // Wait time after reading
unsigned long sleepTime = 9680;    // Time between consecutive readings (in ms)

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 columns, 2 rows

// DHT22 Sensor settings
int DHTPin = 2;  
#define DHTTYPE DHT22
DHT dht(DHTPin, DHTTYPE);

float h, t, hic;

// Sound Sensor settings
int digitalPin_SS = 7;
int AnalogPin_SS = 0;
int BuzzerPin = 13;
int LedPin = 8;
int digitalVal;
int analogVal;

// MQ2 Gas Sensor settings
#define MQ2_AnalogPin 1
#define MQ2_Threshold 400
float Analog_sensorVal_MQ2;
float MQ2Voltage_Val;
float ppm_MQ2;

// MQ3 Gas Sensor settings
int Analogpin_MQ3 = 2;
float SensorVal_Analog_MQ3;
float MQ3Voltage_Val;
float ppm_MQ3;
#define MQ3Threshold 400

// MQ135 Gas Sensor
int MQ135_analogPin = 3;
float MQ135_Sensor_val;
float MQ135_SensVoltage;
float ppm_MQ135;

void setup() 
{
  Serial.begin(9600);
  // Initialize the LCD
  lcd.begin(16, 2); // Update with the correct LCD size (16 columns, 2 rows)
  lcd.backlight();

  // Initialize sensors and actuators
  pinMode(digitalPin_SS, INPUT);
  pinMode(AnalogPin_SS, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(LedPin, OUTPUT);

  // Initialize dust sensor LED pin
  pinMode(ledPowerPin, OUTPUT);
  digitalWrite(ledPowerPin, LOW);  // Turn off LED initially

  dht.begin();
  delay(3000);
}

void loop() 
{
  // DHT Sensor
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor"));
    lcd.setCursor(0, 0);
    lcd.print("DHT error");
    return;
  }
  
  hic = dht.computeHeatIndex(t, h, false);
  
  // Display DHT Readings
  displayDHTReadings();

  // Sound Sensor
  digitalVal = digitalRead(digitalPin_SS);
  analogVal = analogRead(AnalogPin_SS);
  displaySoundSensorReadings();

  // MQ2 Gas Sensor
  Analog_sensorVal_MQ2 = analogRead(MQ2_AnalogPin);
  MQ2Voltage_Val = (Analog_sensorVal_MQ2 / 1024.0) * 5.0;
  ppm_MQ2 = (MQ2Voltage_Val - 0.04) / 0.00035;
  displayMQ2Readings();

  // MQ3 Gas Sensor
  SensorVal_Analog_MQ3 = analogRead(Analogpin_MQ3);
  MQ3Voltage_Val = (SensorVal_Analog_MQ3 / 1024.0) * 5.0;
  ppm_MQ3 = (MQ3Voltage_Val - 0.04) / 0.00035;
  displayMQ3Readings();

  // MQ135 Gas Sensor
  MQ135_Sensor_val = analogRead(MQ135_analogPin);
  MQ135_SensVoltage = (MQ135_Sensor_val/1024.0) * 5.0;
  ppm_MQ135 =(MQ135_SensVoltage - 0.04) / 0.00035;
  displayMQ135Readings();

  // Dust Sensor
  float dustDensity = readDustSensor();
  displayDustSensorReadings(dustDensity);

  // Send the collected data to ESP32 in a comma-separated format
  sendDataToESP32(dustDensity);

  delay(3000);
}

// Function to display DHT sensor readings on LCD and Serial
void displayDHTReadings() {
  Serial.println("___________________");
  Serial.println("DHT Readings");
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temp: "));
  Serial.print(t);
  Serial.print(F("°C  HeatIndex: "));
  Serial.println(hic);
  Serial.println("  ");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("°C");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HeatIndex: ");
  lcd.print(hic);
  lcd.print("°C");
  delay(3000);
}

// Function to display Sound sensor readings on LCD and Serial
void displaySoundSensorReadings() {
  Serial.println("Sound Sensor");
  Serial.print(F("Sound Readings: "));
  Serial.println(analogVal);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sound: ");
  lcd.print(analogVal);
  if (digitalVal == HIGH)
  {
    digitalWrite(LedPin, HIGH);
    Serial.println("High noise detected!");
    lcd.setCursor(0, 1);
    lcd.print("High noise!");
  }
  else
  {
    digitalWrite(LedPin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Normal");
  }
  delay(3000);
  Serial.println(" ");
}

// Function to display MQ2 sensor readings on LCD and Serial
void displayMQ2Readings() {
  Serial.println("MQ2 Gas Sensor");
  Serial.print("Sensor Value: ");
  Serial.print(Analog_sensorVal_MQ2);
  Serial.print(" ppm: ");
  Serial.println(ppm_MQ2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ2: ");
  lcd.print(ppm_MQ2);
  lcd.print(" ppm");

  if (Analog_sensorVal_MQ2 > MQ2_Threshold) 
  {
    Serial.println("| Smoke detected!");
    lcd.setCursor(0, 1);
    lcd.print("Smoke detected!");
    tone(BuzzerPin, 1000);
  } 
  else 
  {
    noTone(BuzzerPin);
    lcd.setCursor(0, 1);
    lcd.print("No smoke");
  }
  delay(3000);
}

// Function to display MQ3 sensor readings on LCD and Serial
void displayMQ3Readings() {
  Serial.println("MQ3 Gas Sensor");
  Serial.print("Sensor Value: ");
  Serial.print(SensorVal_Analog_MQ3);
  Serial.print("  ppm: ");
  Serial.println(ppm_MQ3);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ3: ");
  lcd.print(ppm_MQ3);
  lcd.print(" ppm");

  if (SensorVal_Analog_MQ3 > MQ3Threshold) 
  {
    Serial.println("High Alcohol levels detected!!");
    lcd.setCursor(0, 1);
    lcd.print("High Alcohol!");
    tone(BuzzerPin, 1000);
  } 
  else 
  {
    Serial.println("Average Alcohol levels");
    lcd.setCursor(0, 1);
    lcd.print("Avg Alcohol");
    noTone(BuzzerPin);
  }
  delay(3000);
}

// Function to display MQ135 sensor readings on LCD and Serial
void displayMQ135Readings() {
  Serial.println(" ");
  Serial.println("MQ135 Gas Sensor");
  Serial.print("Sensor Value: ");
  Serial.print(MQ135_Sensor_val);
  Serial.print(" ppm: ");
  Serial.println(ppm_MQ135);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ135: ");
  lcd.print(ppm_MQ135);
  lcd.setCursor(0, 1);
  lcd.print(" ppm");
}

// Function to read dust sensor value and return the dust density in mg/m³
float readDustSensor() {
  digitalWrite(ledPowerPin, HIGH);  // Turn on the LED
  delayMicroseconds(samplingTime);

  int sensorValue = analogRead(dustSensorPin);
  digitalWrite(ledPowerPin, LOW);  // Turn off the LED

  float voltage = sensorValue * (5.0 / 1024.0);  // Convert to voltage
  float dustDensity = (voltage - 0.6) * 0.5;     // Convert to dust density in mg/m³
  
  return dustDensity > 0 ? dustDensity : 0;  // Return only positive values
}

// Function to display Dust sensor readings on LCD and Serial
void displayDustSensorReadings(float dustDensity) {
  Serial.println("Dust Sensor");
  Serial.print("Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" mg/m³");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dust: ");
  lcd.print(dustDensity);
  lcd.print(" mg/m3");
  delay(3000);
}

// Function to send data to ESP32 via Serial2 in a comma-separated format
void sendDataToESP32(float dustDensity) {
  // Prepare the comma-separated data
  String data = String(h) + "," + String(t) + "," + String(hic) + "," + String(analogVal) + "," + String(ppm_MQ2) + "," + String(ppm_MQ3) + "," + String(ppm_MQ135) + "," + String(dustDensity);
    
  // Send data to ESP32 over Serial
  Serial.println(data);
}
