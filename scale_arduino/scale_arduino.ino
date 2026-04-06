//including the libraries for hx711, RTC, BME280 and SD card

#include <HX711.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include <SD.h>
#include <SPI.h>
#include <BME280I2C.h>
#include <SoftwareSerial.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(3, 5);

//making a scale object 
HX711 scale;

//making a BME object 

BME280I2C bme; 
//making a rtc object

static DS1307 RTC; 

//  pins for hx711
const int dataPinHx711 = 7;
const int clockPinHx711 = 6;

//pins for SSD
const int chipSelect = 10;

// create file object
File Data;

String dataString; 

float mass; 
float temp(NAN), hum(NAN), pres(NAN);

int measure_interval = 1; //setting the measurement interval params. 
const int T_init = 0; 
int T_measure = 0; 
int minutes; 


void setup() {
  
  //com to computer
  Serial.begin(9600);
  
  // begin wire
  Wire.begin();
  //set pins to scale 
  scale.begin(dataPinHx711, clockPinHx711);
  //start BME
  bme.begin();
  //begin SD reader
  SD.begin(chipSelect);
  //scale set offset and constants then set current weight to zero 
  scale.set_offset(172001);
  scale.set_scale(-27.967407);
  scale.tare(20);

  // intitialize RTC
  RTC.setHours(13);
  RTC.setMinutes(40);
  RTC.setDay(23);
  RTC.setMonth(3);
  RTC.setYear(2026);

  RTC.begin();

  //com to nodemcu 
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  nodemcu.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  minutes = RTC.getMinutes(); 

  if (minutes >= (T_measure + T_init)) { 

    BME_read();

    //mass = scale.get_units(1000);

    dataString = RTC.getDateTimeString() + "," + String(float(pres)) + "," + String(float(temp)) + "," + String(float(hum));// + "," + String(mass);
    Serial.println(dataString);
    //File Data = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    //if (Data) {
    //  Data.println(dataString);
    //  Data.close();
    //  // print to the serial port too:
    //  //Serial.println(dataString);
    //}
    // if the file isn't open, pop up an error:
    //else {  
    //  Serial.println("error opening datalog.txt");
    //}

    nodemcu.println(dataString);
    
    T_measure += measure_interval;

    if (T_measure >= 60){
      T_measure = 0 + T_init;
    }    
    
  }
  
  delay(500);
}





void BME_read() { //updates temp pres and humidity values.
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  bme.read(pres, temp, hum, tempUnit, presUnit);
}
