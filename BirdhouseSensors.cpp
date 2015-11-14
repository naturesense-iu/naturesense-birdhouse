// This #include statement was automatically added by the Particle IDE.
#include "BirdhouseSensors.h" //Broken, needs fixing

//// Sensor Array: The set of all sensors attached to a birdhouse

SensorArray::SensorArray(Sensor sensors[]){
    _sensors = sensors;
}

void SensorArray::start(){
  for (int i = 0; i < (sizeof(_sensors)/sizeof(*_sensors)); i++)
  {
    _sensors[i].start();
    delay(2000);
  }
  delay(5000);
}

String SensorArray::getCSVHeader(){
  String elementHeaders = "";

  for (int i = 0; i < (sizeof(_sensors)/sizeof(*_sensors)); i++)
  {
    elementHeaders = _sensors[i].printElementHeadersCSV(elementHeaders);
  }
  
  return elementHeaders;
}

String SensorArray::getCSVRow(){
  String elementValues = "";

  for (int i = 0; i < (sizeof(_sensors)/sizeof(*_sensors)); i++)
  {
    elementValues = _sensors[i].printElementValuesCSV(elementValues);
  }
  
  return elementValues;
}

//// Base class for sensors attached to birdhouses

String Sensor::printElementHeadersCSV(String inputString) {
  String * elementHeaders = getElementHeaders();

  for (int i = 0; i < (sizeof(elementHeaders)/sizeof(*elementHeaders)); i++)
  {
    if (inputString != "") {
      inputString += ", ";
    } 
    inputString += elementHeaders[i];
  }

  return inputString;
}

String Sensor::printElementValuesCSV(String inputString) {
  String * elementValues = getElementValues();

  for (int i = 0; i < (sizeof(elementValues)/sizeof(*elementValues)); i++)
  {
    if (inputString != "") {
      inputString += ", ";
    }
    inputString += elementValues[i];
  }
  
  return inputString;
}


//// Combined DHT sensor
//// Default pins: Interior: 5; Exterior: 6

String TemperatureHumiditySensor::getSensorName() {
  return _location + " Temperature and Humidity";
}
String * TemperatureHumiditySensor::getElementHeaders() {
    String elements[] = {_location + "Temperature", _location + "Humidity"};
    return elements;
}
String * TemperatureHumiditySensor::getElementValues(){
    float temp = _sensor.getTempCelcius();
    float hum = _sensor.getHumidity();
    
    String tempString = String(temp, 3);
    String humString = String(hum, 3);
    String elements[] = {tempString, humString};
    return elements;
}
void TemperatureHumiditySensor::start(){
    _sensor.begin();
}
  
TemperatureHumiditySensor::TemperatureHumiditySensor(uint8_t pin, String location) : _sensor(pin, DHT22){
    _location = location;
    _pin = pin;
};


InteriorTemperatureHumiditySensor::InteriorTemperatureHumiditySensor() : TemperatureHumiditySensor(5, "Interior") { };

ExteriorTemperatureHumiditySensor::ExteriorTemperatureHumiditySensor() : TemperatureHumiditySensor(6, "Exterior") { };


//// TSL 2561 Luminosity Sensor
//// Default location: interior (SPI, no specific pin)

LuminositySensor::LuminositySensor(int32_t id, String location) : _sensor(TSL2561_ADDR_FLOAT, id) {
    _location = location;
    _id = id;
}

String LuminositySensor::getSensorName(){
    return _location + " Luminosity";
}
String * LuminositySensor::getElementHeaders(){
    String elements[] = {_location + "Luminosity"};
    return elements;
}
String * LuminositySensor::getElementValues(){
    sensors_event_t event;
    _sensor.getEvent(&event);
    
    float lum = event.light;
    
    String lumString = String(lum, 3);
    String elements[] = {lumString};
    return elements;
}
void LuminositySensor::start(){
    _sensor.enableAutoRange(true);
    _sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
    _sensor.begin();
}

InteriorLuminositySensor::InteriorLuminositySensor() : LuminositySensor(725, "Interior") { };

// ExteriorLuminositySensor::ExteriorLuminositySensor() : LuminositySensor(899, "Exterior") { };

//// PIR Motion Sensor
//// Default location: interior, pin 7

MotionSensor::MotionSensor(uint8_t pin, String location) {
    _location = location;
    _pin = pin;
}

bool MotionSensor::readSensor() {
    bool sensorOn = digitalRead(_pin);
    return sensorOn;
}
String MotionSensor::getSensorName(){
    return _location + " Motion";
}
String * MotionSensor::getElementHeaders(){
    String elements[] = {_location + "Motion"};
    return elements;
}
String * MotionSensor::getElementValues(){
    bool motionDetected = readSensor();
    String motionDetString = "false";
    if (motionDetected) {
        motionDetString = "true";
    }
    String elements[] = {motionDetString};
    return elements;
}
void MotionSensor::start(){
    pinMode(_pin, INPUT);
}

InteriorMotionSensor::InteriorMotionSensor() : MotionSensor(7, "Interior") { };

// ExteriorMotionSensor::ExteriorMotionSensor() : MotionSensor(899, "Exterior") { };


//// IR Proximity Sensor
//// Default location: exterior, pin A2

ProximitySensor::ProximitySensor(uint8_t pin, String location) {
    _location = location;
    _pin = pin;
}

float ProximitySensor::readSensor() {
    float sensorOn = analogRead(_pin);
    return sensorOn;
}
float ProximitySensor::calcDistance(float proxRaw) {
    float distance = 0;
    if (80 < proxRaw < 500) {
        distance = 4800/(proxRaw - 20);
    }
    return distance;
}
String ProximitySensor::getSensorName(){
    return _location + " Proximity";
}
String * ProximitySensor::getElementHeaders(){
    String elements[] = {_location + "ProximityRaw", _location + "ProximityDistanceCm"};
    return elements;
}
String * ProximitySensor::getElementValues(){
    float proxRaw = readSensor();
    String proxRawString = String(proxRaw, 3);
    
    float proxDistance = calcDistance(proxRaw);
    String proxDistanceString = String(proxDistance, 3);
    
    String elements[] = {proxRawString, proxDistanceString};
    return elements;
}
void ProximitySensor::start(){
    pinMode(_pin, INPUT);
}

// InteriorProximitySensor::InteriorProximitySensor() : ProximitySensor(A1, "Interior") { };

ExteriorProximitySensor::ExteriorProximitySensor() : ProximitySensor(A2, "Exterior") { };


// Setup Example:
// InteriorTemperatureHumiditySensor  interiorTemperatureHumiditySensor;
// ExteriorTemperatureHumiditySensor exteriorTemperatureHumiditySensor;
// InteriorLuminositySensor    interiorLuminositySensor;

// Sensor* sensors[] = {&interiorTemperatureHumiditySensor, &exteriorTemperatureHumiditySensor, &interiorLuminositySensor};

// SensorArray birdhouseArray(*sensors);
// int            currentSensor = 0;

