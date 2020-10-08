#ifndef SensorlessMotor_h
#define SensorlessMotor_h

#include "PID.h"
#include "SimpleTimer.h"

class SensorlessMotor {
  public:
    SensorlessMotor(byte, byte, byte, byte, byte, float);
    float getSetSpeed(void);
    float getSpeed(void);
    void setSpeed(float);
    void compute(void);
    void disable(void);
    void enable(bool);
    void enable(void);
    void invert(bool);
    void setSpeedPIDTerms(float, float, float);
    float getAnalogReading(void);
  private:
    void updateMotor(void);
    bool inverted;
    bool enabled;
    byte enablePinA;
    byte inputPinA;
    byte enablePinB;
    byte inputPinB;
    byte analogPin;
    float rpm;
    float rpmSetPoint;
    float motorPower;
    float analogReading;
    float rpmPerStep;
    PID* speedPID;
    SimpleTimer* measureTimer;
    SimpleTimer* measurementDelayTimer;
};

#endif
