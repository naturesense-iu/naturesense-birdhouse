// This #include statement was automatically added by the Particle IDE.
#include "BirdhouseSensors.h"
#include "Adafruit_TSL2561_U/Adafruit_TSL2561_U.h"
#include "Adafruit_DHT/Adafruit_DHT.h"

InteriorTemperatureHumiditySensor  interiorTemperatureHumiditySensor;   //default pin: 5
ExteriorTemperatureHumiditySensor exteriorTemperatureHumiditySensor;    //default pin: 6
InteriorLuminositySensor    interiorLuminositySensor;                   //SPI pins
InteriorMotionSensor    interiorMotionSensor;                           //default pin: 7
ExteriorProximitySensor exteriorProximitySensor;                        //default pin: A2

Sensor* sensors[] = {&interiorTemperatureHumiditySensor, &exteriorTemperatureHumiditySensor, &interiorLuminositySensor, &interiorMotionSensor, &exteriorProximitySensor};

SensorArray birdhouseArray(*sensors);
int            currentSensor = 0;

void setup() {
    
//    birdhouseArray = new SensorArray(sensors);

    Serial.begin(9600);
    birdhouseArray.start();
    String header = birdhouseArray.getCSVHeader();
    //publish CSV header
    Serial.print(header);
}

void loop() {
    delay(2000);
  
    String currentStatus = birdhouseArray.getCSVRow();
  //publish CSV row
    Serial.print(currentStatus);
}
