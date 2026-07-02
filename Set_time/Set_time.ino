#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

#define MUX_ADDR 0x70


void setup() {
  // put your setup code here, to run once:
  RTC.begin();

  Serial.begin(115200);
    pca9548a_select(7);
    RTC.begin();

    //RTC.setHourMode(CLOCK_H12);
    RTC.setHourMode(CLOCK_H24);

    if (RTC.getHourMode() == CLOCK_H12)
    {
        //RTC.setMeridiem(HOUR_PM);
    }

    RTC.setDay(1);
    RTC.setMonth(7);
    RTC.setYear(2026);

    RTC.setHours(9);
    RTC.setMinutes(24);
    RTC.setSeconds(25);

    //RTC.setWeek(1); // To update the week manually.

	RTC.updateWeek(); // updates the week automatically based on date in the RTC.

    //RTC.setDate(22,07,29);
    //RTC.setTime(22,10,20);

}

void loop()
{
	
    Serial.print(RTC.getWeekString().substring(0, 3));
    Serial.print(" ");
    Serial.print(RTC.getDateString());
    Serial.print(" ");
    Serial.print(RTC.getTimeString());
    Serial.println();
    delay(1000);
}




void pca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel); 
  Wire.endTransmission();

  
}