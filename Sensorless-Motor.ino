#include "SensorlessMotor.h"
#include "SimpleTimer.h"

SensorlessMotor motorRight(8, 10, 7, 9, A1, 1.4285714285714286);

SimpleTimer printTimer(10);

byte faultPin = 2;
byte sleepPin = 12;
byte resetPin = 11;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(faultPin, INPUT);
  pinMode(sleepPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(sleepPin, HIGH);
  digitalWrite(resetPin, HIGH);

  printTimer.enable();
  motorRight.enable();

  Serial.println("Starting");
}

void loop() {
  // if (!digitalRead(faultPin)) {
  //   Serial.println("FAULT");  
  // } else {
  //   motorRight.compute();
  // }

  motorRight.compute();

  if (printTimer.check()) {
    // Serial.print('Analog: ');
    Serial.println(motorRight.getSpeed());
  }

  // put your main code here, to run repeatedly:

  // if (sampleMotor.check()) {
  //   // Disconnect one power supply from motor and ground other input.
  //   digitalWrite(e2, LOW);
  //   digitalWrite(i2, LOW);
  //   delay(2);
  //   analogReading = 0.4 * analogRead(analogPin) + 0.6 * analogReading;
  //   Serial.println(analogReading);    
  //   digitalWrite(e2, HIGH);
  //   analogWrite(i2, 1024);
  // }
}