// This #include statement was automatically added by the Particle IDE.
#include "BirdhouseSensors.h" //Broken, needs fixing

//// Sensor Array: The set of all sensors attached to a birdhouse
SensorArray::SensorArray(std::vector<Sensor *> sensors){
    _sensors = sensors;
}
SensorArray::SensorArray(){
//    _sensors = new std::vector<Sensor>();
}

void SensorArray::init(std::vector<Sensor *> sensors){
    _sensors = sensors;
}

void SensorArray::start(){
    int sensorCount = _sensors.size();
    Particle.publish("SensorArraySize", String(sensorCount));
    for (int i = 0; i < sensorCount; i++)
    {
        _sensors.at(i)->start();
        delay(2000);
    }
    delay(5000);
}

String SensorArray::getCSVHeader(){
    String elementHeaders = "";
    int sensorCount = _sensors.size();

    for (int i = 0; i < sensorCount; i++)
    {
        elementHeaders = _sensors.at(i)->printElementHeadersCSV(elementHeaders);
    }
    
    return elementHeaders;
}

String SensorArray::getCSVRow(){
    String elementValues = "";

    int sensorCount = _sensors.size();

    for (int i = 0; i < sensorCount; i++)
    {
        elementValues = _sensors.at(i)->printElementValuesCSV(elementValues);
    }
    
    return elementValues;
}

//// Base class for sensors attached to birdhouses

String Sensor::printElementHeadersCSV(String inputString) {
    std::vector<String> elementHeaders = getElementHeaders();

    for (int i = 0; i < (elementHeaders.size()); i++)
    {
        if (inputString != "") {
            inputString += ", ";
        } 
        inputString += elementHeaders.at(i);
        // Particle.publish("pEHC.inputString: ", elementHeaders.at(i));
    }
    

    return inputString;
}

String Sensor::printElementValuesCSV(String inputString) {
    std::vector<String> elementValues = getElementValues();
//  Particle.publish("pEVC.elementValues: ", elementValues[0]);

    for (int i = 0; i < (elementValues.size()); i++)
    {
        if (inputString != "") {
            inputString += ", ";
        }
        inputString += elementValues.at(i);
        // Particle.publish("pEVC.inputString: ", elementValues.at(i);
    }

    
    return inputString;
}


//// Combined DHT sensor
//// Default pins: Interior: 5; Exterior: 6

String TemperatureHumiditySensor::getSensorName() {
    return _location + " Temperature and Humidity";
}
std::vector<String> TemperatureHumiditySensor::getElementHeaders() {
    // Particle.publish("GettingSensorHeaders", getSensorName());

    std::vector<String> elements; 
    elements.push_back(_location + "Temperature");
    elements.push_back(_location + "Humidity");
    
    return elements;
}
std::vector<String> TemperatureHumiditySensor::getElementValues(){
    // Particle.publish("GettingSensorValues", getSensorName());
    float temp = _sensor.getTempCelcius();
    float hum = _sensor.getHumidity();

    String tempString = String(temp, 3);
    String humString = String(hum, 3);
    
    // Particle.publish("temp", tempString);
    // Particle.publish("hum", humString);
    
    std::vector<String> elements; 
    elements.push_back(tempString);
    elements.push_back(humString);
    
    return elements;
}
void TemperatureHumiditySensor::start(){
    Particle.publish("StartingSensor", getSensorName());
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
std::vector<String> LuminositySensor::getElementHeaders(){
    std::vector<String> elements; 
    elements.push_back(_location + "Luminosity");

    return elements;
}
std::vector<String> LuminositySensor::getElementValues(){
    sensors_event_t event;
    _sensor.getEvent(&event);
    
    float lum = event.light;
    
    String lumString = String(lum, 3);
    std::vector<String> elements; 
    elements.push_back(lumString);

    return elements;
}
void LuminositySensor::start(){
    Particle.publish("StartingSensor", getSensorName());
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
std::vector<String> MotionSensor::getElementHeaders(){
    std::vector<String> elements;
    elements.push_back(_location + "Motion");

    return elements;
}
std::vector<String> MotionSensor::getElementValues(){
    bool motionDetected = readSensor();
    String motionDetString = "false";
    if (motionDetected) {
        motionDetString = "true";
    }

    std::vector<String> elements; 
    elements.push_back(motionDetString);

    return elements;
}
void MotionSensor::start(){
    Particle.publish("StartingSensor", getSensorName());
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
    if ((80 < proxRaw) && (proxRaw < 500)) {
        distance = 4800/(proxRaw - 20);
    }
    return distance;
}
String ProximitySensor::getSensorName(){
    return _location + " Proximity";
}
std::vector<String> ProximitySensor::getElementHeaders(){
    std::vector<String> elements;
    elements.push_back(_location + "ProximityRaw");
    elements.push_back(_location + "ProximityDistanceCm");
    return elements;

}
std::vector<String> ProximitySensor::getElementValues(){
    float proxRaw = readSensor();
    String proxRawString = String(proxRaw, 3);
    
    float proxDistance = calcDistance(proxRaw);
    String proxDistanceString = String(proxDistance, 3);
    
    std::vector<String> elements; 
    elements.push_back(proxRawString);
    elements.push_back(proxDistanceString);

    return elements;
}
void ProximitySensor::start(){
    Particle.publish("StartingSensor", getSensorName());
    pinMode(_pin, INPUT);
}

// InteriorProximitySensor::InteriorProximitySensor() : ProximitySensor(A0, "Interior") { };

ExteriorProximitySensor::ExteriorProximitySensor() : ProximitySensor(A1, "Exterior") { };


//// Mic-based Sound Sensor
//// Default location: interior, pin A3

SoundSensor::SoundSensor(uint8_t pin, String location) {
    _location = location;
    _pin = pin;
}

float SoundSensor::readSensor() {
    float sensorOn = analogRead(_pin);
    return sensorOn;
}
String SoundSensor::getSensorName(){
    return _location + " Sound";
}
std::vector<String> SoundSensor::getElementHeaders(){
    std::vector<String> elements;
    elements.push_back(_location + "SoundLevel");
    return elements;
}
std::vector<String> SoundSensor::getElementValues(){
    float soundLevelRaw = readSensor();
    String soundLevelRawString = String(soundLevelRaw, 3);
    
    std::vector<String> elements; 
    elements.push_back(soundLevelRawString);

    return elements;
}
void SoundSensor::start(){
    Particle.publish("StartingSensor", getSensorName());
    pinMode(_pin, INPUT);
}

InteriorSoundSensor::InteriorSoundSensor() : SoundSensor(A3, "Interior") { };

//ExteriorSoundSensor::ExteriorSoundSensor() : SoundSensor(A4, "Exterior") { };


// Setup Example:
// InteriorTemperatureHumiditySensor  interiorTemperatureHumiditySensor;
// ExteriorTemperatureHumiditySensor exteriorTemperatureHumiditySensor;
// InteriorLuminositySensor    interiorLuminositySensor;

// Sensor* sensors[] = {&interiorTemperatureHumiditySensor, &exteriorTemperatureHumiditySensor, &interiorLuminositySensor};

// SensorArray birdhouseArray(*sensors);
// int            currentSensor = 0;
