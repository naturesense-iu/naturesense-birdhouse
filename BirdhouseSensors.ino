// This #include statement was automatically added by the Particle IDE.
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
//  sensors.push_back(new InteriorSoundSensor);                //default pin: A4
//  sensors.push_back(new ExteriorProxLumSensor);              //I2C pins: D0 => SDA, D1 => SCL

    birdhouseArray.init(sensors);
}

void setup() {
    
    initBirdhouseArray();

    // Serial.begin(9600);
    // Serial.println("Starting!");
    birdhouseArray.start();
    // Serial.println("Birdhouse Initialized!");
    // String header = birdhouseArray.getCSVHeader();
    // Serial.println("Header Built!");
    // Serial.println(header);

    //publish CSV header
    // Particle.publish("StartingBirdhouse", header);
}

void loop() {
    // try {
    delay(30000);
        // Serial.println("Monitoring Birdhouse! Iteration: " + currIteration);
        // currIteration++;
        // String currentStatus = birdhouseArray.getCSVRow();
        String currentStatus = birdhouseArray.getJSONData();
        // Serial.println(currentStatus);
    
      //publish CSV row
        Particle.publish("BirdhouseData", currentStatus);
       // Particle.publish("myjsonlog", "{ \"birdhouse_data\" : " + currentStatus + " }", 60, PRIVATE);
    // }
    // catch (const std::exception& e) {
    //     // Serial.println("Error: " + e);
        delay(30000);
    // }
        // delay(30000);
        System.sleep(D7,RISING,120);
}