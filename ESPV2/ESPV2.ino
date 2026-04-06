
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

 
// Replace with your WiFi and Web App details
//const char* ssid = "📶WirelessFirmware📶";
//const char* password = "ElektroCyberSecurityKonijn";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbymWAoic5WTgZoxp1sjdReDGKZ3s_Arxp831kt8tEvcVZpcdE2lo33ol5yg4g38s-jPCw/exec";
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
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();


    HTTPClient http;

    http.begin(*client, scriptURL);
    http.addHeader("Content-Type", "application/json");
 
    // Replace with your sensor data
    float temp = random(20, 30);  // Simulated temperature
    float hum = random(40, 60);   // Simulated humidity
    float time = random(40, 60);
    float pres = random(40, 60);
    float masss = random(40, 60);
    
    //String jsonData = "{\"time\": " + String(Time) + ", \"temp\": " + String(temp) + ", \"hum\": " + String(hum) + ", \"pres\": " + String(pres) + ", \"mass\": " + String(mass) +"}";
 
    //float temp = random(20, 30);  // Simulated temperature
    //float hum = random(40, 60);   // Simulated humidity
 
    String jsonData = "{\"temp\": " + String(temp) + ", \"hum\": " + String(hum) + ", \"time\": " + String(time) + ", \"pres\": " + String(pres) + "}";


    int httpResponseCode = http.POST(jsonData);
 
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
 
  delay(10000); // Send data every 10 seconds
}
