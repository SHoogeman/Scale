
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <SoftwareSerial.h>

 
// Replace with your WiFi and Web App details
//const char* ssid = "📶WirelessFirmware📶";
//const char* password = "ElektroCyberSecurityKonijn";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbweea9BWjkBLszlII4J5-T4UEJ6stplfIiWQCkz4vzkXJwXTtZEKKcMP3T76XCkXbnQyA/exec";
//const char* ssid = "S21 van Sjoerd";
//const char* password = "ugki4500";
const char* ssid = "XTRA-Gast";
const char* password = "Welkom123";

SoftwareSerial arduino(D7, D6); //The object to receive data from arduino

String inputString = "";         // A String to hold incoming data

bool stringComplete = false;      // Whether the string is complete

int counter = 0;
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  arduino.begin(115200); //init software serial 
  pinMode(D7, INPUT);
  pinMode(D6, OUTPUT);
  inputString.reserve(200); // Reserve 200 bytes for the string

}
 
void loop() {
  while (arduino.available() && stringComplete == false) {
    char inChar = arduino.read();
       if (inChar == '\n') {
      stringComplete = true;
    } else if (inChar != '\r') {
      inputString += inChar;
    }
  }
  if (stringComplete) {

    String time = "";
    String pres = "";
    String temp = "";
    String hum = "";
    String mass = "";
    Serial.println(inputString);

    for (int i = 0; i < inputString.length(); i++){
      if (String(inputString.charAt(i)) == ",") {
        counter += 1;
      }
       else {
         switch(counter){
           case 0:{
             time += char(inputString[i]);
           }
           break;
           case 1:{
             pres += char(inputString[i]);
           }
           break;
           case 2:{
             temp += char(inputString[i]);
           }
           break;
           case 3:{
             hum += char(inputString[i]);
           }
          break;
          case 4:{
            mass += char(inputString[i]);
          }
          break;

          default: {
          }    
        }
      }     
    }
    // Clear the string for the next message
    stringComplete = false;
    mass = "100";
    sendData(temp, hum, time, pres, mass);
    counter = 0; 
    inputString="";
  }
  delay(100);
}



void sendData(String temp, String hum, String time, String pres, String mass) {
  
  if (WiFi.status() == WL_CONNECTED) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient http;

    http.begin(*client, scriptURL);
    http.addHeader("Content-Type", "application/json");
     
    String jsonData = "{\"temp\": " + temp + ", \"hum\": " + hum + ", \"time\": " + "\""  + time + "\"" + ", \"pres\": " + pres + ", \"mass\": " + mass + "}";
    Serial.println(jsonData);
    int httpResponseCode = http.POST(jsonData);
 
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(200);
}
