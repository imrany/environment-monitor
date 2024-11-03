#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Server IP address and endpoint
const char* serverUrl = "http://<your_server_ip>:5000/api/sensor_data";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    
    // Check if serial data is available from the microcontroller
    if (Serial.available()) {
      String data = Serial.readStringUntil('\n');

      // Replace with actual sensor data
      float temperature = 25.5;
      float humidity = 60.3;
      float heatIndex = 27.2;
      int soundLevel = 300;
      float mq2Ppm = 0.04;
      float mq3Ppm = 0.05;
      float mq135Ppm = 0.02;
      float dustDensity = 0.1;
      
      // Create JSON payload
      String payload = "{";
      payload += "\"temperature\":" + String(temperature) + ",";
      payload += "\"humidity\":" + String(humidity) + ",";
      payload += "\"heat_index\":" + String(heatIndex) + ",";
      payload += "\"sound_level\":" + String(soundLevel) + ",";
      payload += "\"mq2_ppm\":" + String(mq2Ppm) + ",";
      payload += "\"mq3_ppm\":" + String(mq3Ppm) + ",";
      payload += "\"mq135_ppm\":" + String(mq135Ppm) + ",";
      payload += "\"dust_density\":" + String(dustDensity);
      payload += "}";

      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(payload);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(5000); // Adjust delay as needed
}


void loop() {
  
}
