#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

int sensorVal;
const int ANALOG_READ_PIN = A0; // Change to A0 if you are using analog pin A0
const int RESOLUTION = 12;      // Could be 9-12
const int threshold = 4000;

#define DHTPIN 4   // Pin where the DHT22 is connected
#define DHTTYPE DHT22   // DHT type (DHT11, DHT22, AM2302, etc.)

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Airtel_Deathnote";
const char *password = "deathnote@123";
const char *serverUrl = "http://192.168.1.11:3001/updateData";

void setup() {
  Serial.begin(9600);
  delay(1000);  

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  analogReadResolution(RESOLUTION);
  sensorVal = analogRead(ANALOG_READ_PIN);

    // Read DHT sensor data
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create a JSON object with sensor and weather data
  String jsonPayload = "{";
  jsonPayload += "\"humidity\":" + String(h) + ",";
  jsonPayload += "\"temperature\":" + String(t) + ",";
  jsonPayload += "\"airPressure\":0,"; // You can add the air pressure data if needed

  // Use light sensor to determine if it's sunny
  bool isSunny = (sensorVal < threshold);

  // Basic weather conditions based on temperature and humidity
  if (t > 25.0 && h > 70) {
    jsonPayload += "\"isRainy\":true,";
    jsonPayload += "\"isCloudy\":false,";
    jsonPayload += "\"isSunny\":" + String(isSunny ? "true" : "false");
    Serial.println("Weather: Rainy");
  } else if (t > 25.0) {
    jsonPayload += "\"isRainy\":false,";
    jsonPayload += "\"isCloudy\":true,";
    jsonPayload += "\"isSunny\":" + String(isSunny ? "true" : "false");
    Serial.println("Weather: Cloudy");
  } else {
    jsonPayload += "\"isRainy\":false,";
    jsonPayload += "\"isCloudy\":false,";
    jsonPayload += "\"isSunny\":" + String(isSunny ? "true" : "false");
    Serial.println("Weather: Sunny");
  }

  jsonPayload += "}";

  // Send PUT request to the server
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(jsonPayload);
  http.end();

  // Check if the PUT request was successful
  if (httpCode > 0) {
    Serial.println("PUT request successful");
  } else {
    Serial.println("PUT request failed");
  }

  
}
