#include <HX711.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include <Adafruit_BME280.h>
#include <SparkFun_FS3000_Arduino_Library.h> 
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "S21 van Sjoerd";
const char* password = "ugki4500";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbxWAr_kMeby1LsUuoVyyNGm05QfF062mMueQDR7VMfMgVgZlLkew0piug8uW86KHOeHLQ/exec";

// Makes objects for sensors
static DS3231 RTC;
static Adafruit_BME280 bme; 
static FS3000 fs;
static HX711 scale;

// Pins
int SDA_PIN = 21;
int SCL_PIN = 22; 
const uint8_t dataPinHx711 = 12;
const uint8_t clockPinHx711 = 13;
const uint8_t ACPin = 34;  
    

// Variable to track the last logged minute from the RTC
int lastMinute = -1;

// Function to change the multiplexer selector
#define MUX_ADDR 0x70

void pca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel); 
  Wire.endTransmission(); 
}

// Function to measure the current
#define ACTectionRange 20     

float readACCurrentValue() {
  float totalMilliVolts = 0;
  
  // Take 5 samples
  for (uint8_t i = 0; i < 5; i++) { 
    totalMilliVolts += analogReadMilliVolts(ACPin); // Returns calibrated voltage in mV directly
    delay(1);
  }
  // Average the readings and convert millivolts to volts
  float avgVoltage = (totalMilliVolts / 5.0) / 1000.0; 
  float voltageVirtualValue = avgVoltage * 0.707;    
  return voltageVirtualValue * ACTectionRange;
}

// Function to handle Wi-Fi connection with a 30-second timeout
bool connectWiFiWithTimeout(unsigned long timeoutMs) {
  WiFi.begin(ssid, password);
  //Serial.print("Connecting to WiFi");
  
  unsigned long startAttemptTime = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    
    // Check if 30 seconds have passed
    if (millis() - startAttemptTime >= timeoutMs) {
      //Serial.println("\nWiFi connection timed out! Entering offline mode.");
      return false;
    }
  }
  
  Serial.println("\nWiFi connected successfully.");
  return true;
}

void setup() {
  Serial.begin(9600, SERIAL_8N1); delay(100);
  
  Wire.begin(SDA_PIN, SCL_PIN); delay(100);

  pca9548a_select(7); RTC.begin(); delay(100);

  pca9548a_select(0); bme.begin(0x76); delay(100);

  pca9548a_select(1); bme.begin(0x76); delay(100);

  pca9548a_select(2); bme.begin(0x76); delay(100);

  pca9548a_select(3); fs.begin(0x28); fs.setRange(AIRFLOW_RANGE_15_MPS); delay(100);
  
  pca9548a_select(4); fs.begin(0x28); fs.setRange(AIRFLOW_RANGE_15_MPS); delay(100);

  scale.begin(dataPinHx711, clockPinHx711); delay(100);

  analogReadResolution(12);
  // Connect to WiFi with 30-second limit during setup
  connectWiFiWithTimeout(30000);
  Serial.println("start measurement.");
}

void loop() {
  // 1. Select the RTC channel (7) to check the current time
  pca9548a_select(7);
  int currentMinute = RTC.getMinutes();

  // 2. Check if a new minute has rolled over
  if (currentMinute != lastMinute) {
    lastMinute = currentMinute; // Update tracker so it only logs once this minute

  // 3. Read all sensors across multiplexer channels
    pca9548a_select(0); float Temp1 = bme.readTemperature(); float Humi1 = bme.readHumidity(); float Pres1 = bme.readPressure();
    pca9548a_select(1); float Temp2 = bme.readTemperature(); float Humi2 = bme.readHumidity(); float Pres2 = bme.readPressure();
    pca9548a_select(2); float Temp3 = bme.readTemperature(); float Humi3 = bme.readHumidity(); float Pres3 = bme.readPressure();

    pca9548a_select(3); float v1 = fs.readMetersPerSecond();
    pca9548a_select(4); float v2 = fs.readMetersPerSecond();

    float mass = scale.read_average(10); 

    float Amp = readACCurrentValue();
    
  // 4. Output the recorded data stream & handle web upload
  printDataStream(Serial, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, Pres3, Temp3, Humi3, v1, v2, Amp, mass);
  }

  delay(500); 
}

void printDataStream(Print &output, float p1, float t1, float h1, float p2, float t2, float h2, float p3, float t3, float h3, float v1, float v2, float A, float m){
  
  pca9548a_select(7);  
  String date = RTC.getDateString();
  String time = RTC.getTimeString();
  String timestampStr = date + " " + time; 

  output.print(timestampStr);
  output.print(F(","));
  output.print(p1);   output.print(F(","));
  output.print(t1);   output.print(F(","));
  output.print(h1);   output.print(F(","));
  output.print(p2);   output.print(F(","));
  output.print(t2);   output.print(F(","));
  output.print(h2);   output.print(F(","));
  output.print(p3);   output.print(F(","));
  output.print(t3);   output.print(F(","));
  output.print(h3);   output.print(F(","));
  output.print(v1);   output.print(F(","));
  output.print(v2);   output.print(F(","));
  output.print(A);    output.print(F(","));
  output.println(m);

  // Handle Wi-Fi HTTP POST upload if connected
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
 
    String jsonData = "{\"temp1\": " + String(t1) + 
                      ", \"hum1\": " + String(h1) + 
                      ", \"time\": \"" + timestampStr + "\"" + 
                      ", \"pres1\": " + String(p1) + 
                      ", \"pres2\": " + String(p2) + 
                      ", \"temp2\": " + String(t2) + 
                      ", \"hum2\": " + String(h2) + 
                      ", \"pres3\": " + String(p3) + 
                      ", \"temp3\": " + String(t3) + 
                      ", \"hum3\": " + String(h3) + 
                      ", \"v1\": " + String(v1) +
                      ", \"v2\": " + String(v2) + 
                      ", \"mass\": " + String(m) + 
                      ", \"amp\": " + String(A) + "}"; 
                      
    int httpResponseCode = http.POST(jsonData);
    http.end();
  } else {
    connectWiFiWithTimeout(30000);
  }
}