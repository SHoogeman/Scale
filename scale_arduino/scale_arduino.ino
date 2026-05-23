// including the libraries for hx711, RTC, BME280 and SD card
#include <HX711.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>

// Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(3, 5);

// making a scale object 
HX711 scale;
static DS1307 RTC; 

// pins for hx711 and SD (const uint8_t bespaart geheugen)
const uint8_t dataPinHx711 = 7;
const uint8_t clockPinHx711 = 6;
const uint8_t chipSelect = 10;
const uint8_t ACPin = A0;       

#define ACTectionRange 20    // set Non-invasive AC Current Sensor tection range (5A,10A,20A)
#define VREF 5.0
#define sensor_1_address 0x76
#define sensor_2_address 0x77

Adafruit_BME280 bme1; // I2C device 1
Adafruit_BME280 bme2; // I2C device 2

const int measure_interval = 1; 
int T_measure = 17; 

void setup() {
  // com to computer
  Serial.begin(19200);

  delay(500);

  
  // begin wire
  Wire.begin();
  
  // set pins to scale 
  scale.begin(dataPinHx711, clockPinHx711);
  
  // start BME
  bme1.begin(sensor_1_address);
  bme2.begin(sensor_2_address);
  
  // begin SD reader
  SD.begin(chipSelect);
  
  // scale set offset and constants then set current weight to zero 
  //scale.set_scale(-19.448113373468736); //rc
  //scale.tare(20);

  // intitialize RTC
  RTC.begin();
  RTC.setHours(9); // begin time  
  RTC.setMinutes(18);
  RTC.setSeconds(0);

  // com to nodemcu 
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  nodemcu.begin(115200);
}

// Helper functie om data lineair te printen in plaats van Strings samen te voegen (Bespaart >500 bytes RAM)
void printDataStream(Print &output, float p1, float t1, float h1, float p2, float t2, float h2, float m, float amp) {
  output.print(RTC.getHours());
  output.print(F(":"));
  output.print(RTC.getMinutes());
  output.print(F(","));
  output.print(p1); output.print(F(","));
  output.print(t1); output.print(F(","));
  output.print(h1); output.print(F(","));
  output.print(p2); output.print(F(","));
  output.print(t2); output.print(F(","));
  output.print(h2); output.print(F(","));
  output.print(m);  output.print(F(","));
  output.println(amp); // println sluit de regel netjes af
}

void loop() {
  int minutes = RTC.getMinutes(); 
  Serial.println(T_measure);
  if (minutes >= T_measure) { 
    // Lokale variabelen gebruiken minder permanent RAM
    float Amp = readACCurrentValue();
    float Temp1 = bme1.readTemperature();
    float Humi1 = bme1.readHumidity();
    float Pres1 = bme1.readPressure();
    
    float Temp2 = bme2.readTemperature();
    float Humi2 = bme2.readHumidity();
    float Pres2 = bme2.readPressure();

    // 10 points. 
    float mass = scale.read_average(10); 

    // 1. Print naar Serial
    printDataStream(Serial, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, mass, Amp);
    
    // 2. Print naar NodeMCU
    printDataStream(nodemcu, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, mass, Amp);

    // 3. Print naar SD Kaart
    File Data = SD.open("datalog.txt", FILE_WRITE);
    if (Data) {
      printDataStream(Data, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, mass, Amp);
      Data.close();
    } else {  
      // Gebruik F() macro om vaste tekst uit het RAM te houden
      Serial.println(F("error opening datalog.txt")); 
    }
    
    T_measure += measure_interval;

    if (T_measure >= 60){
      T_measure = 0;
      while(RTC.getMinutes() == 59){
        delay(100);
      }
    }    
  }
  delay(1500);
}

float readACCurrentValue() {
  float peakVoltage = 0;
  for (int i = 0; i < 5; i++) {
    peakVoltage += analogRead(ACPin);   // read peak voltage
    delay(1);
  }
  peakVoltage = peakVoltage / 5.0;
  float voltageVirtualValue = peakVoltage * 0.707;    // change the peak voltage to the Virtual Value of voltage

  /* The circuit is amplified by 2 times, so it is divided by 2. */
  voltageVirtualValue = (voltageVirtualValue / 1024.0 * VREF ) / 2.0;

  return voltageVirtualValue * ACTectionRange;
}