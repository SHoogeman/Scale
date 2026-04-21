
# Scale documentation
## Table of Contents
1. [calibration](#calibration)
2. [monitoring](#Remote)
3. [Starting the measurement](#start)
4. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)


## calibration
For calibration of the scale [this sketch](calibration_sketch) can be used. 
This sketch will show the raw output of the [hx711](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf) board (scale) to the arduino, which can be used to calibrate the scale. 

The values found in the calibration have to be put into the following functions: 
1. [scale.set_offset()](scale_arduino/scale_arduino.inoscale_arduino.ino#L52) is used to set the zero weight off set. 
2. [scale.set_scale()](scale_arduino/scale_arduino.ino#L53) is used to set the scales gradient. 

## Remote monitoring
The google sheets are connected using a app script, which listens for https posts. The setup is done by first creating a new sheet. Next open the appscripts by clicking on extensions followed by the appscript button. This leads you to the following screen ![image](Images/appscript_blank.png). Copy paste the code given in [code.gs](AppScript/code.gs), save it and click on implement in the right corner followed by new implementation. ![image](Images/select-web-app.png) in this window select web app. 
![image](Images/des_and_everyone.png) input a description (does not need a specific description) and change the access to everyone, which is very important. Next google will give some pop up you will have to allow everything. by clicking on advanced on the left side of the pop up. 
![image](Images/Webb_app_link.png) Copy the web-app url and replace the [link](ESPV2/ESPV2.ino#L10)

To connect the esp8266 to wifi change the [ssid and password](ESPV2/ESPV2.ino#L12-13) 

## start measurement 
To begin a measurement the sd should be inserted into the SD module. If the SD card is not inserted an error will be shown via the Serial monitor. To set the time interval between measurements adjust [measure_interval](scale_arduino/scale_arduino.ino#L31), the default is set to 1 minute. To set the starting time adjust [RTC.setHours and RTC.setMinutes](scale_arduino/scale_arduino.ino#L58-59). Now plug the arduino into an outlet via the USB B port and everthing should run smoothly. 

## Data formartation 
The data is formated in the following order: 
|pressure 1|humidity 1|temperature 1|pressure 2|humidity 2|temperature 2| Mass| current|  
|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
|Pascal|%|Celsius|Pascal|%|Celsius|grams|Ampère| 


