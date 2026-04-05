
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// Replace with your WiFi and Web App details
//const char* ssid = "📶WirelessFirmware📶";
//const char* password = "ElektroCyberSecurityKonijn";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbyPjrZTHLC2_Jok2N8-GvB7FWMFaMFcZbVBjeDbG-SJF6LC_jhGntdcSAEA6uIJueUM9w/exec";
const char* ssid = "S21 van Sjoerd";
const char* password = "ugki4500";
 
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
    WiFiClient  ;

    http.begin(client, scriptURL);
    http.addHeader("Content-Type", "application/json");
 
    // Replace with your sensor data
    //float temp = random(20, 30);  // Simulated temperature
    //float hum = random(40, 60);   // Simulated humidity
    //String Time = "time"; 
    //String pres = "Pres";
    //String mass = "Mass";
    
    //String jsonData = "{\"Temp\": " + String(Time) + ", \"Temp\": " + String(temp) + ", \"Hum\": " + String(hum) + ", \"Pres\": " + String(pres) + ", \"Mass\": " + String(mass) +"}";
 
    float temp = random(20, 30);  // Simulated temperature
    float hum = random(40, 60);   // Simulated humidity
 
    String jsonData = "{\"temp\": " + String(temp) + ", \"hum\": " + String(hum) + "}";


    int httpResponseCode = http.POST(jsonData);
 
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
 
  delay(10000); // Send data every 10 seconds
}
