#include <HX711.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include <Adafruit_BME280.h>
#include <SparkFun_FS3000_Arduino_Library.h> 
#include <WiFi.h>
#include <HTTPClient.h>

// --- Pin & Hardware Definitions ---
#define MUX_ADDR        0x70  // Default I2C address for TCA9548A / PCA9548A
#define RX_PIN 3
#define TX_PIN 1

int SDA_PIN = 21;
int SCL_PIN = 22; 

const uint8_t dataPinHx711  = 10;
const uint8_t clockPinHx711 = 9;
const uint8_t ACPin         = 15;       

#define ACTectionRange 20     
#define VREF           3.3    // ESP32 ADC operates at 3.3V logic

// --- Wi-Fi & Google Sheets Credentials ---
const char* ssid = "S21 van Sjoerd";
const char* password = "ugki4500";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbxWAr_kMeby1LsUuoVyyNGm05QfF062mMueQDR7VMfMgVgZlLkew0piug8uW86KHOeHLQ/exec";

// --- Global Objects ---
HX711 scale;
static DS3231 RTC; 
FS3000 fs;
Adafruit_BME280 bme; 

// --- Timing & Measurement Variables ---
const uint8_t measure_interval = 1; 
uint8_t T_measure = 5; 

// Global telemetry variables
float Temp1 = 0, Humi1 = 0, Pres1 = 0;
float Temp2 = 0, Humi2 = 0, Pres2 = 0;
float Temp3 = 0, Humi3 = 0, Pres3 = 0;
float v1 = 0, v2 = 0, mass = 0, Amp = 0;
String timestampStr = "";

// Multiplexer channel selector
void pca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel); 
  Wire.endTransmission();
}

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); 
  analogReadResolution(12);
  delay(100);
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);

  delay(100);

  //scale.begin(dataPinHx711, clockPinHx711);

  // Initialize BME280 sensors through multiplexer channels 0, 1, 2
  pca9548a_select(0); bme.begin(0x76);
  pca9548a_select(1); bme.begin(0x76);
  pca9548a_select(2); bme.begin(0x76);
  delay(200);
  // Initialize RTC on channel 7
  pca9548a_select(7); 
  RTC.begin();

  // Initialize FS3000 airflow sensors on channels 3 and 4
  pca9548a_select(3); fs.begin(); fs.setRange(AIRFLOW_RANGE_15_MPS); 
  pca9548a_select(4); fs.begin(); fs.setRange(AIRFLOW_RANGE_15_MPS);

  
  delay(200);
  pca9548a_select(7);
  T_measure = RTC.getMinutes();
  Serial2.println(T_measure);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial2.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     Serial2.print(".");
  }
  Serial2.println("WiFi connected.");
}


// Fused Data Stream Handler (Logs to Serial/OpenLog AND uploads via HTTP POST)
void printDataStream(Print &output, float p1, float t1, float h1, float p2, float t2, float h2, float p3, float t3, float h3, float v1, float v2, float m, float amp) {
  //pca9548a_select(7);
  String date = RTC.getDateString();
  String time = RTC.getTimeString();
  timestampStr = date + " " + time;

  // 1. Print formatted CSV text (captured by OpenLog)
  output.print(timestampStr);
  output.print(F(","));
  output.print(p1);  output.print(F(","));
  output.print(t1);  output.print(F(","));
  output.print(h1);  output.print(F(","));
  output.print(p2);  output.print(F(","));
  output.print(t2);  output.print(F(","));
  output.print(h2);  output.print(F(","));
  output.print(p3);  output.print(F(","));
  output.print(t3);  output.print(F(","));
  output.print(h3);  output.print(F(","));
  output.print(v1);  output.print(F(","));
  output.print(v2);  output.print(F(","));
  //output.print(m);   output.print(F(","));
  output.println(amp); 

  //2. Simultaneously handle Wi-Fi HTTP POST upload inside the stream handler
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
                      ", \"amp\": " + String(amp) + "}";
                      
    int httpResponseCode = http.POST(jsonData);
    Serial.print(F("Cloud HTTP Response: "));
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println(F("WiFi Disconnected. Reconnecting..."));
    WiFi.begin(ssid, password);
  }
}

float readACCurrentValue() {
  float peakVoltage = 0;
  for (uint8_t i = 0; i < 5; i++) { 
    peakVoltage += analogRead(ACPin);   
    delay(1);
  }
  peakVoltage = peakVoltage / 5.0;
  float voltageVirtualValue = peakVoltage * 0.707;     
  
  voltageVirtualValue = (voltageVirtualValue / 4095.0 * VREF) / 2.0;
  return voltageVirtualValue * ACTectionRange;
}

void loop() {
  pca9548a_select(7);
  uint8_t minutes = RTC.getMinutes(); 
  Serial2.println(minutes);
  
  if (minutes >= T_measure) { 
    Serial.println("loop");
    Amp = readACCurrentValue();
    
    // Read BME280 #1 (Channel 0)
    pca9548a_select(0);
    Temp1 = bme.readTemperature();
    Humi1 = bme.readHumidity();
    Pres1 = bme.readPressure();
    
    // Read BME280 #2 (Channel 1)
    pca9548a_select(1);
    Temp2 = bme.readTemperature();
    Humi2 = bme.readHumidity();
    Pres2 = bme.readPressure();

    // // Read BME280 #3 (Channel 2)
    pca9548a_select(2);
    Temp3 = bme.readTemperature();
    Humi3 = bme.readHumidity();
    Pres3 = bme.readPressure();
    
    //mass = scale.read_average(10); 
    
    // Read FS3000 Airflow #1 (Channel 3)
    pca9548a_select(3);
    v1 = fs.readMetersPerSecond();
    
    // Read FS3000 Airflow #2 (Channel 4)
    pca9548a_select(4);
    v2 = fs.readMetersPerSecond();

    Serial.print(F("Sending packet: "));
    
    // Trigger the fused print & upload stream
    printDataStream(Serial2, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, Pres3, Temp3, Humi3, v1, v2, mass, Amp);
    
    T_measure += measure_interval;

    if (T_measure >= 60) {
      T_measure = 0;
      while (true) {
        pca9548a_select(7);
        if (RTC.getMinutes() != 59) break;
        delay(100);
      }
    }     
  }

  // Short idle delay before checking interval loop again
  delay(2000);
}