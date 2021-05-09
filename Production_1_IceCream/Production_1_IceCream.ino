/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo upperServo, lowerServo; // create servo object to control a servo


int upperPin = 0;
int lowerPin = 1;
int upperVal;
int lowerVal;

unsigned long preLowerMillis = 0;
unsigned long preUpperMillis = 0;
void setup() {
  upperServo.attach(upperPin);  // attaches the servo on pin 9 to the servo object
  lowerServo.attach(lowerPin);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  val += 1;
  val2 += 1;
  if (upperVal > 25) {
    upperVal = 0;
  }
  if (lowerVal > 25) {
    lowerVal = 0;
  }
  upperServo.write(upperVal);                  // sets the servo position according to the scaled value
  lowerServo.write(lowerVal);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}
