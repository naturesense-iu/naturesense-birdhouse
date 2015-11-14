#ifndef BirdhouseSensors_H
#define BirdhouseSensors_H

#include "application.h"
#include "math.h"

// #include "Adafruit_Sensor/Adafruit_Sensor.h"
#include "Adafruit_TSL2561_U/Adafruit_TSL2561_U.h"
#include "Adafruit_DHT/Adafruit_DHT.h"
#include<stdlib.h>

class Sensor {
public:
  virtual String getSensorName();
  virtual String * getElementHeaders();
  virtual String * getElementValues();
  virtual void start();
  
  String printElementValuesCSV(String inputString);
  
  String printElementHeadersCSV(String inputString);
};


class SensorArray {
protected:
    Sensor * _sensors;
public:
    SensorArray(Sensor sensors[]);

    void start();
    String getCSVHeader();
    String getCSVRow();
};


class TemperatureHumiditySensor : public Sensor {
protected:
  String _location;
  uint8_t _pin;
  DHT _sensor;

public:
  TemperatureHumiditySensor(uint8_t pin, String location);

  String getSensorName();
  String * getElementHeaders();
  String * getElementValues();
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
  String * getElementHeaders();
  String * getElementValues();
  void start();
};

class InteriorLuminositySensor : public LuminositySensor {
public:
    InteriorLuminositySensor();
};

// class ExteriorLuminositySensor : public LuminositySensor {
// public:
//     ExteriorLuminositySensor();
// };


class MotionSensor : public Sensor {
protected:
    String _location;
  uint8_t _pin;
    bool readSensor();

public:
  MotionSensor(uint8_t pin, String location);

    String getSensorName();
    String * getElementHeaders();
    String * getElementValues();

    void start();
};

class InteriorMotionSensor : public MotionSensor {
public:
    InteriorMotionSensor();
};

// class ExteriorMotionSensor : public MotionSensor {
// public:
//     ExteriorMotionSensor();
// };


class ProximitySensor : public Sensor {
protected:
    String _location;
    uint8_t _pin;
    float readSensor();
    float calcDistance(float proxRaw);

public:
    ProximitySensor(uint8_t pin, String location);

    String getSensorName();
    String * getElementHeaders();
    String * getElementValues();

    void start();
};

// class InteriorProximitySensor : public ProximitySensor {
// public:
//     InteriorProximitySensor();
// };

class ExteriorProximitySensor : public ProximitySensor {
public:
    ExteriorProximitySensor();
};


#endif