#include "BirdhouseSensors.h"

InteriorTemperatureHumiditySensor  interiorTemperatureHumiditySensor;   //default pin: 5
// ExteriorTemperatureHumiditySensor exteriorTemperatureHumiditySensor;    //default pin: 6
// InteriorLuminositySensor    interiorLuminositySensor;                   //SPI pins
InteriorMotionSensor    interiorMotionSensor;                           //default pin: 7
ExteriorProximitySensor exteriorProximitySensor;                        //default pin: A2

Sensor* sensors[3] = {&interiorTemperatureHumiditySensor, &interiorMotionSensor, &exteriorProximitySensor}; //, &exteriorTemperatureHumiditySensor, &interiorLuminositySensor, &interiorMotionSensor, &exteriorProximitySensor};

SensorArray birdhouseArray(*sensors);
int            currentSensor = 0;
uint8_t currIteration = 1;

void setup() {
    
//    birdhouseArray = new SensorArray(sensors);

    Serial.begin(9600);
    Serial.println("Starting!");
    birdhouseArray.start();
    Serial.println("Birdhouse Initialized!");
    String header = birdhouseArray.getCSVHeader();
    Serial.println("Header Built!");
    Serial.println(header);
    //publish CSV header
    Spark.publish("StartingBirdhouse", header);
}

void loop() {
    delay(2000);
    Serial.println("Monitoring Birdhouse! Iteration: " + currIteration);
    currIteration++;
    String currentStatus = birdhouseArray.getCSVRow();
    Serial.println(currentStatus);
  //publish CSV row
    Spark.publish("BirdhouseData", currentStatus);
}
