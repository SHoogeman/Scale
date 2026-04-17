
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <SoftwareSerial.h>

 
// Replace with your WiFi and Web App details

const char* scriptURL = "https://script.google.com/macros/s/AKfycbwlGBjL5e06fL0nQXsFdjupKoZui81lZfJ6SmlX5Tz3o24qniGRE1Ui-OdfEi11PACh/exec";

const char* ssid = "TUD-facility";
const char* password = "646cc23c9edd7";

SoftwareSerial arduino(D7, D6); //The object to receive data from arduino

String inputString = "";         // A String to hold incoming data

bool stringComplete = false;      // Whether the string is complete

int counter = 0;

int LED = 2; 
 
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

  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
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
    
    String hum1 = "";
    String pres1 = "";
    String temp1 = "";

    String hum2 = "";
    String pres2 = "";
    String temp2 = "";

    String amp = "";
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
             pres1 += char(inputString[i]);
           }
           break;
           case 2:{
             temp1 += char(inputString[i]);
           }
           break;
           case 3:{
             hum1 += char(inputString[i]);
           }
          break;
          case 4:{
            pres2 += char(inputString[i]);
          }
          break;
          case 5:{
            temp2 += char(inputString[i]);
          }
          break;
          case 6:{
            hum2 += char(inputString[i]);
          }
          break;
          case 7:{
            mass += char(inputString[i]);
          }
          break;
          case 8:{
            amp += char(inputString[i]);
          }
          break;
          default: {
          }    
        }
      }     
    }
    // Clear the string for the next message
    stringComplete = false;
    sendData(temp1, hum1, time, pres1, mass, amp, hum2, temp2, pres2);
    counter = 0; 
    inputString="";
    
  }
  delay(100);
}



void sendData(String temp1, String hum1, String time, String pres1, String mass, String amp, String hum2, String temp2, String pres2) {
  
  if (WiFi.status() == WL_CONNECTED) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient http;

    http.begin(*client, scriptURL);
    http.addHeader("Content-Type", "application/json");
     
    String jsonData = "{\"temp1\": " + temp1 + ", \"hum1\": " + hum1 + ", \"time\": " + "\""  + time + "\"" + ", \"pres1\": " + pres1 + ", \"mass\": " + mass + ", \"hum2\": " + hum2 + ", \"temp2\": " + temp2 +", \"pres2\": "+ pres2 + ", \"amp\": " + amp + "}";
    Serial.println(jsonData);
    int httpResponseCode = http.POST(jsonData);
 
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(200);
}
