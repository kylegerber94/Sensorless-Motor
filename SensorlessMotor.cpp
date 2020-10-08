#include "Arduino.h"
#include "SensorlessMotor.h"

SensorlessMotor::SensorlessMotor(byte enablePinA, byte inputPinA, byte enablePinB, byte inputPinB, byte analogPin, float rpmPerStep) {
  this->enablePinA = enablePinA;
  this->inputPinA = inputPinA;
  this->enablePinB = enablePinB;
  this->inputPinB = inputPinB;
  this->analogPin = analogPin;
  this->rpmPerStep = rpmPerStep;

  pinMode(this->enablePinA, OUTPUT);
  pinMode(this->inputPinA, OUTPUT);
  pinMode(this->enablePinB, OUTPUT);
  pinMode(this->inputPinB, OUTPUT);
  pinMode(this->analogPin, INPUT);

  // Initialize Variables.
  rpm = 0;
  rpmSetPoint = 0;
  motorPower = 0;
  analogReading = 0;
  inverted = false;

  // Setup Timers.
  measureTimer = new SimpleTimer(5);
  measureTimer->enable();
  measurementDelayTimer = new SimpleTimer(1);

  // Setup speed PID controller with default parameters (run at 1000 Hz).
  speedPID = new PID(&this->rpm, &this->motorPower, &this->rpmSetPoint, 0.2, 10, 0.01, 10);
  speedPID->setLimit(-100, 100);
  speedPID->setPOnM(true);

  disable();
}

void SensorlessMotor::compute(void) {

  // Update PID controller.
  bool computed = speedPID->compute();
  // If controller output has changed, update motor driver.
  if (computed) {
    motorPower = 100;
    updateMotor();
  }

  // Pause motor to sample it's speed.
  if (measureTimer->check()) {
    // Switch PWM output to high-z by disabling that h-bridge.
    if (motorPower > 0) {
        digitalWrite(enablePinA, LOW);
    } else {
      digitalWrite(enablePinB, LOW);
    }
    // Enable measurement delay timer and reset it.
    measurementDelayTimer->reset();
    measurementDelayTimer->enable();
  }
  if (measurementDelayTimer->check()) {
    // Perform measurement.
      analogReading = 0.1 * analogRead(analogPin) + 0.9 * analogReading;
      rpm = analogReading * rpmPerStep;
      if (motorPower < 0) rpm = -rpm;
    // Disable measurement delay timer.
    measurementDelayTimer->disable();
    // Update motor to re-activate it.
    updateMotor();
  }

}

void SensorlessMotor::updateMotor(void) {
  // Do not update the motor while we are measuring.
  if (measurementDelayTimer->isEnabled()) return;
  
  if (!enabled) {
    // Disable motor driver.
    digitalWrite(enablePinA, LOW);
    digitalWrite(enablePinB, LOW);
  }
  else {
    // Map motor power to PWM duty cycle.
    unsigned int dutyCycle = abs(motorPower) * 1023 / 100;
    // Enable motor driver.
    digitalWrite(enablePinA, HIGH);
    digitalWrite(enablePinB, HIGH);
    if (motorPower == 0) {
      digitalWrite(inputPinA, LOW);
      digitalWrite(inputPinB, LOW);
    } else if (motorPower > 0) {
      analogWrite(inputPinA, dutyCycle);
      digitalWrite(inputPinB, LOW);
    } else {
      digitalWrite(inputPinA, LOW);
      analogWrite(inputPinB, dutyCycle);
    }
  }
}

void SensorlessMotor::disable(void) {
  enable(false);
}

void SensorlessMotor::enable(void) {
  enable(true);
}

void SensorlessMotor::enable(bool enable) {
  enabled = enable;
  updateMotor();
}

float SensorlessMotor::getAnalogReading(void) {
  return analogReading;
}

float SensorlessMotor::getSpeed(void) {
  return rpm;
}