#include <HX711.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>
#include <SparkFun_FS3000_Arduino_Library.h> 

// Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(3, 5);

// making a scale object 
HX711 scale;
static DS1307 RTC; 

// MEMORY SAVED: Only 1 instance needed because of the I2C Multiplexer
FS3000 fs;

// pins for hx711 and SD 
const uint8_t dataPinHx711 = 7;
const uint8_t clockPinHx711 = 6;
const uint8_t chipSelect = 10;
const uint8_t ACPin = A0;       

#define MUX_ADDR 0x70

#define ACTectionRange 20    // set Non-invasive AC Current Sensor tection range (5A,10A,20A)
#define VREF 5.0
#define sensor_1_address 0x76
#define sensor_2_address 0x77

// MEMORY SAVED: Consolidated 3 instances into 1 single object
Adafruit_BME280 bme; 

// MEMORY SAVED: Changed from int to uint8_t (saves RAM)
const uint8_t measure_interval = 1; 
uint8_t T_measure = 5; 

void setup() {
  // com to computer
  Serial.begin(19200);

  delay(500);
  
  // begin wire
  Wire.begin();
  
  // set pins to scale 
  scale.begin(dataPinHx711, clockPinHx711);
  
  // Initialize the single BME object across different hardware channels
  pca9548a_select(0);
  bme.begin(0x76);

  pca9548a_select(1);
  bme.begin(0x76);

  pca9548a_select(2);
  bme.begin(0x76);

  // begin SD reader
  SD.begin(chipSelect);
  
  // scale set offset and constants then set current weight to zero 
  //scale.set_scale(-19.448113373468736); //rc
  //scale.tare(20);

  // initialize RTC
  pca9548a_select(7);
  RTC.begin();
  RTC.setHours(14); // begin time  
  RTC.setMinutes(5);
  RTC.setSeconds(0);

  // com to nodemcu 
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  nodemcu.begin(115200);

  // Initialize the single FS3000 object across channels 3 and 4
  pca9548a_select(3);
  fs.begin();
  fs.setRange(AIRFLOW_RANGE_15_MPS); 

  pca9548a_select(4);
  fs.begin();
  fs.setRange(AIRFLOW_RANGE_15_MPS);
}

// Helper functie om data lineair te printen in plaats van Strings samen te voegen
void printDataStream(Print &output, float p1, float t1, float h1, float p2, float t2, float h2, float p3, float t3, float h3, float v1, float v2, float m, float amp) {
  pca9548a_select(7);
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
  output.print(p3); output.print(F(","));
  output.print(t3); output.print(F(","));
  output.print(h3); output.print(F(","));
  output.print(v1); output.print(F(","));
  output.print(v2); output.print(F(","));
  output.print(m);  output.print(F(","));
  output.println(amp); 
}

void loop() {
  uint8_t minutes = RTC.getMinutes(); // MEMORY SAVED: int changed to uint8_t
  Serial.println(T_measure);
  if (minutes >= T_measure) { 
    float Amp = readACCurrentValue();
    
    // Read from BME channel 0
    pca9548a_select(0);
    float Temp1 = bme.readTemperature();
    float Humi1 = bme.readHumidity();
    float Pres1 = bme.readPressure();

    // Read from BME channel 1 using the exact same object instance
    pca9548a_select(1);
    float Temp2 = bme.readTemperature();
    float Humi2 = bme.readHumidity();
    float Pres2 = bme.readPressure();

    // Read from BME channel 2 using the exact same object instance
    pca9548a_select(2);
    float Temp3 = bme.readTemperature();
    float Humi3 = bme.readHumidity();
    float Pres3 = bme.readPressure();

    float mass = scale.read_average(10); 

    // Read from FS3000 channel 3
    pca9548a_select(3);
    float v1 = fs.readMetersPerSecond();

    // Read from FS3000 channel 4 using the exact same object instance
    pca9548a_select(4);
    float v2 = fs.readMetersPerSecond();

    // 1. Print naar Serial
    printDataStream(Serial, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, Pres3, Temp3, Humi3, v1, v2, mass, Amp);
    
    // 2. Print naar NodeMCU 
    printDataStream(nodemcu, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, Pres3, Temp3, Humi3, v1, v2, mass, Amp);
    // 3. Print naar SD Kaart
    File Data = SD.open("datalog.txt", FILE_WRITE);
    if (Data) {
      // FIX: Changed 'Serial' to 'Data' so it actually writes to your SD card log file
      printDataStream(Data, Pres1, Temp1, Humi1, Pres2, Temp2, Humi2, Pres3, Temp3, Humi3, v1, v2, mass, Amp);
      Data.close();
    } else {  
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
  delay(2000);
}


float readACCurrentValue() {
  float peakVoltage = 0;
  for (uint8_t i = 0; i < 5; i++) { // MEMORY SAVED: Loop index changed to uint8_t
    peakVoltage += analogRead(ACPin);   
    delay(1);
  }
  peakVoltage = peakVoltage / 5.0;
  float voltageVirtualValue = peakVoltage * 0.707;    

  /* The circuit is amplified by 2 times, so it is divided by 2. */
  voltageVirtualValue = (voltageVirtualValue / 1024.0 * VREF ) / 2.0;

  return voltageVirtualValue * ACTectionRange;
}

void pca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel); 
  Wire.endTransmission();
}