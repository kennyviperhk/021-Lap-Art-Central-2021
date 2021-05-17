/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo upperServo, lowerServo; // create servo object to control a servo

const long interval = 20;           // interval at which to blink (milliseconds)
long moveInterval = 2000;           // interval at which to blink (milliseconds)
int upperPin = 0;
int lowerPin = 1;
float val[2];

float servoSpeed[2];
bool isPositive[2];

bool isMove[2];

unsigned long prevMillis = 0;
unsigned long prevMoveMillis = 0;
void setup() {
  Serial.begin(115200);
  upperServo.attach(upperPin);  // attaches the servo on pin 9 to the servo object
  lowerServo.attach(lowerPin);  // attaches the servo on pin 9 to the servo object
  for (int i = 0; i < 2; i++) {
    isPositive[i] = false;
  }

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interval) {

    prevMillis = currentMillis;

    for (int i = 0; i < 2; i++) {
      if (isMove[i]) {
        if (isPositive[i]) {

          val[i] += servoSpeed[i];
        } else {
          val[i] -= servoSpeed[i];

        }
      }
      if (val[i] > 25) {
        isPositive[i] = false;
      } else if (val[i] <= 0) {
        isPositive[i] = true;
      }
    }
  }

  if (currentMillis - prevMoveMillis >= moveInterval) {
    moveInterval = random(2000,8000);
    servoSpeed[0] =random(0,3);
    servoSpeed[1] =random(0,3);
    prevMoveMillis = currentMillis;
    for (int i = 0; i < 2; i++) {
      isMove[i] = !isMove[i];
    }
  }
  upperServo.write(val[0]);
    lowerServo.write(val[1]);
  Serial.print("upper : \t" );
  Serial.print(val[0] );
  Serial.print("\t lower : \t" );
  Serial.println(val[1] );


}
