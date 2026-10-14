#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

#define I2C_DEV_ADDR 0x55

const char* ssid = "S21 van Sjoerd";
const char* password = "ugki4500";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbxWAr_kMeby1LsUuoVyyNGm05QfF062mMueQDR7VMfMgVgZlLkew0piug8uW86KHOeHLQ/exec";

// Global buffer to accumulate incoming I2C chunks
String inputString = ""; 
volatile bool messageReady = false; 

void parseAndSendData(String data);
void onReceive(int len);

void setup() {
  Serial.begin(115200);
  inputString.reserve(200); // Pre-allocate memory space
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  Wire.onReceive(onReceive);
  Wire.setPins(23, 22);

  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Serial.printf("I2C Slave ready at address: 0x%02X\n", I2C_DEV_ADDR);
}

void loop() {
  // Check if the I2C interrupt flagged a completed message
  if (messageReady) {
    String messageToProcess = inputString;
    inputString = ""; // Reset buffer immediately
    messageReady = false; // Reset flag
    
    parseAndSendData(messageToProcess);
  }
}

// I2C Callback context runs on an interrupt thread
void onReceive(int len) {
  while (Wire.available()) {
    char inChar = Wire.read();
    
    if (inChar == '\n') {
      // End signal found! Signal the main loop to process it
      messageReady = true; 
    } else if (inChar != '\r') {
      inputString += inChar;
    }
  }
}

void parseAndSendData(String data) {
  int counter = 0;
  String time = "", hum1 = "", pres1 = "", temp1 = "", hum2 = "", pres2 = "", temp2 = "";
  String pres3 = "", temp3 = "", hum3 = "", amp = "", mass = "", v1 = "", v2 = "";
  
  //Serial.print("Processing accumulated line: ");
  //Serial.println(data);

  for (int i = 0; i < data.length(); i++){
    if (data.charAt(i) == ',') {
      counter += 1;
    } else {
      switch(counter){
        case 0:  time  += data[i]; break;
        case 1:  pres1 += data[i]; break;
        case 2:  temp1 += data[i]; break;
        case 3:  hum1  += data[i]; break;
        case 4:  pres2 += data[i]; break;
        case 5:  temp2 += data[i]; break;
        case 6:  hum2  += data[i]; break;
        case 7:  pres3 += data[i]; break;
        case 8:  temp3 += data[i]; break;
        case 9:  hum3  += data[i]; break;
        case 10: v1    += data[i]; break;
        case 11: v2    += data[i]; break;
        case 12: mass  += data[i]; break;
        case 13: amp   += data[i]; break;
        default: break;
      }
    }    
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
 
    String jsonData = "{\"temp1\": " + temp1 + ", \"hum1\": " + hum1 + ", \"time\": \""  + time + "\"" + ", \"pres1\": " + pres1 + ", \"pres2\": " + pres2 + ", \"temp2\": " + temp2 + ", \"hum2\": " + hum2 + ", \"pres3\": " + pres3 + ", \"temp3\": " + temp3 + ", \"hum3\": " + hum3 + ", \"v1\": " + v1 + ", \"v2\": \"" + v2 + "\"" + ", \"mass\": " + mass + ", \"amp\": " + amp + "}";
    Serial.println(jsonData);
    int httpResponseCode = http.POST(jsonData);
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    WiFi.begin(ssid, password);

  }
}