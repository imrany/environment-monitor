#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Server endpoint
const char* serverUrl = "http://<your-server-url>:5000/api/sensor_data";

void setup() {
  Serial.begin(9600);       // Serial to communicate with the Arduino
  Serial2.begin(9600);      // Serial2 to communicate with Arduino

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');  // Read the comma-separated data from Arduino
    Serial.println("Received data from Arduino:");
    Serial.println(data);

    // Parse and send data as JSON
    if (sendDataToServer(parseCSVtoJSON(data))) {
      Serial.println("Data sent successfully to server.");
    } else {
      Serial.println("Failed to send data to server.");
    }
  }
  delay(3000);
}

// Function to parse CSV data to JSON
String parseCSVtoJSON(String data) {
  String keys[] = {"humidity", "temperature", "heatIndex", "noise", "mq2_ppm", "mq3_ppm", "mq135_ppm", "dustDensityStr"};
  String json = "{";
  int index = 0;
  int keyCount = sizeof(keys) / sizeof(keys[0]);

  while (data.length() > 0 && index < keyCount) {
    int commaIndex = data.indexOf(',');
    String value = (commaIndex == -1) ? data : data.substring(0, commaIndex);
    json += "\"" + keys[index++] + "\":" + value;

    if (commaIndex == -1) break;
    data = data.substring(commaIndex + 1);
    json += ",";
  }
  json += "}";

  Serial.println("JSON data created:");
  Serial.println(json);
  return json;
}

// Function to send JSON data to the server
bool sendDataToServer(String jsonData) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);  // Send JSON data in POST request
    http.end();
    return httpResponseCode > 0;  // Return true if the response is positive
  } else {
    Serial.println("WiFi not connected.");
    return false;
  }
}
