#ifndef BirdhouseSensors_H
#define BirdhouseSensors_H

#include "application.h"
#include "math.h"

// #include "Adafruit_Sensor/Adafruit_Sensor.h"
#include "Adafruit_DHT/Adafruit_DHT.h"
#include "Adafruit_TSL2561_U/Adafruit_TSL2561_U.h"
#include "VCNL4010/VCNL4010.h"
#include <stdlib.h>
#undef min
#undef max
#include <vector>

class Sensor {
public:
    virtual String getSensorName();
    virtual std::vector<String> getElementHeaders();
    virtual std::vector<String> getElementValues();
    virtual void start();
    
    String printElementValuesCSV(String inputString);
    String printElementValuesJSON();
    String printElementHeadersCSV(String inputString);
};


class SensorArray {
protected:
    std::vector<Sensor *> _sensors;
public:
    SensorArray(std::vector<Sensor *> sensors);
    SensorArray();
    void init(std::vector<Sensor *> sensors);
    
    void start();
    String getCSVHeader();
    String getCSVRow();
    String getJSONData();
};


class TemperatureHumiditySensor : public Sensor {
protected:
    String _location;
    uint8_t _pin;
    DHT _sensor;

public:
    TemperatureHumiditySensor(uint8_t pin, String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();
    void start();
};

class InteriorTemperatureHumiditySensor : public TemperatureHumiditySensor {
public:
    InteriorTemperatureHumiditySensor();
};

class ExteriorTemperatureHumiditySensor : public TemperatureHumiditySensor {
public:
    ExteriorTemperatureHumiditySensor();
};


class LuminositySensor : public Sensor {
protected:
    String _location;
    int32_t _id;
    Adafruit_TSL2561_Unified _sensor;

public:
    LuminositySensor(int32_t id, String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();
    void start();
};

class InteriorLuminositySensor : public LuminositySensor {
public:
    InteriorLuminositySensor();
};

class ExteriorLuminositySensor : public LuminositySensor {
public:
    ExteriorLuminositySensor();
};


class MotionSensor : public Sensor {
protected:
    String _location;
    uint8_t _pin;
    bool readSensor();

public:
    MotionSensor(uint8_t pin, String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();

    void start();
};

class InteriorMotionSensor : public MotionSensor {
public:
    InteriorMotionSensor();
};

class ExteriorMotionSensor : public MotionSensor {
public:
    ExteriorMotionSensor();
};


class ProximitySensor : public Sensor {
protected:
    String _location;
    uint8_t _pin;
    float readSensor();
    float calcDistance(float proxRaw);

public:
    ProximitySensor(uint8_t pin, String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();

    void start();
};

class InteriorProximitySensor : public ProximitySensor {
public:
    InteriorProximitySensor();
};

class ExteriorProximitySensor : public ProximitySensor {
public:
    ExteriorProximitySensor();
};

////

class SoundSensor : public Sensor {
protected:
    String _location;
    uint8_t _pin;
    float readSensor();

public:
    SoundSensor(uint8_t pin, String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();

    void start();
};

class InteriorSoundSensor : public SoundSensor {
public:
    InteriorSoundSensor();
};

////

class ProxLumSensor : public Sensor {
protected:
    String _location;
    VCNL4010 _sensor;
    float readProx();
    float readLum();

public:
    ProxLumSensor(String location);

    String getSensorName();
    std::vector<String> getElementHeaders();
    std::vector<String> getElementValues();

    void start();
};

class InteriorProxLumSensor : public ProxLumSensor {
public:
    InteriorProxLumSensor();
};

class ExteriorProxLumSensor : public ProxLumSensor {
public:
    ExteriorProxLumSensor();
};

////

#endif
