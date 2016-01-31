#include <vector>
#include "BirdhouseSensors.h"

SensorArray birdhouseArray; //(sensors);
int            currentSensor = 0;
uint8_t currIteration = 1;

void initBirdhouseArray() {
    std::vector<Sensor *> sensors;
    
    sensors.push_back(new InteriorTemperatureHumiditySensor);  //default pin: 5
    sensors.push_back(new ExteriorTemperatureHumiditySensor);  //default pin: 6
    sensors.push_back(new InteriorLuminositySensor);           //SPI pins
    sensors.push_back(new InteriorMotionSensor);               //default pin: 7
    sensors.push_back(new ExteriorProximitySensor);            //default pin: A1
    sensors.push_back(new InteriorSoundSensor);                //default pin: A4

    birdhouseArray.init(sensors);
}

void setup() {
    
    initBirdhouseArray();

    Serial.begin(9600);
    Serial.println("Starting!");
    birdhouseArray.start();
    Serial.println("Birdhouse Initialized!");
    String header = birdhouseArray.getCSVHeader();
    Serial.println("Header Built!");
    Serial.println(header);

    //publish CSV header
    Particle.publish("StartingBirdhouse", header);
}

void loop() {
    delay(2000);

    Serial.println("Monitoring Birdhouse! Iteration: " + currIteration);
    currIteration++;
    String currentStatus = birdhouseArray.getCSVRow();
    Serial.println(currentStatus);

  //publish CSV row
    Particle.publish("BirdhouseData", currentStatus);
}