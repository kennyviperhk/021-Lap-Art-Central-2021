
void motorSetup()
{
  stepper.setMaxSpeed(0);
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);
}

void motorLoop()
{
  if (m == 1) {
    if (currentMillis - prevMillis > interval) {
      interval = random(5000, 15000);
      prevMillis = currentMillis;
      currX = random(currX -1000, currX + 1000);
      if (currX > 10000) {
        currX = 10000;
      }
      if (currX < -10000) {
        currX = -10000;
      }
      currY = random(currX - 1000 , currX + 1000);
      if (currY > 10000) {
        currY = 10000;
      }
      if (currY < -10000) {
        currY = -10000;
      }
      currZ = random(currY - 1000 , currY + 1000);
      if (currZ > 10000) {
        currZ = 10000;
      }
      if (currZ < -10000) {
        currZ = -10000;
      }
      String command = "#a" + String(currX) + "b" + String(currY) + "c" + String(currZ);
      decode_text(command);
    }
  }
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(currX * -1);
    stepper.setMaxSpeed(s);
    stepper.setAcceleration(s);
  }
  stepper.run();
}
