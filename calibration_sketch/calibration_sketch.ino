//
//    FILE: HX_plotter.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711


#include "HX711.h"

HX711 scale;

//  adjust pins if needed
uint8_t dataPin = 7;
uint8_t clockPin = 6;

float f;


void setup()
{
  Serial.begin(115200);
  

  scale.begin(dataPin, clockPin);

  scale.tare();
}


void loop()
{
  //  continuous scale 4x per second
  f = scale.read_average(100);
  Serial.println(f);
  delay(250);
}


//  -- END OF FILE --

