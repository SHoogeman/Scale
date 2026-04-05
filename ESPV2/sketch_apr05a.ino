
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// Replace with your WiFi and Web App details
const char* ssid = "📶WirelessFirmware📶";
const char* password = "ElektroCyberSecurityKonijn";
const char* scriptURL = "https://script.google.com/macros/s/AKfycby6KvrA3uAB1FLEMg6Bq_c-dJKz_QHkdpoLjahj5M_vWBPY-2mPQZLRW2NHZ4unQE316g/exec";
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    http.begin(client, scriptURL);
    http.addHeader("Content-Type", "application/json");
 
    // Replace with your sensor data
    float temp = random(20, 30);  // Simulated temperature
    float hum = random(40, 60);   // Simulated humidity
    String Time = "time"; 
    String pres = "Pres";
    String mass = "Mass";
    
    String jsonData = "{\"Temp\": " + String(Time) + ", \"Temp\": " + String(temp) + ", \"Hum\": " + String(hum) + ", \"Pres\": " + String(pres) + ", \"Mass\": " + String(mass) +"}";
 
    int httpResponseCode = http.POST(jsonData);
 
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
 
  delay(10000); // Send data every 10 seconds
}
