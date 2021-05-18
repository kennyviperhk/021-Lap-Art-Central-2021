
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
      prevMillis = currentMillis;
      int seed = random(1);
      pickSituation(seed);
    }
  }
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(currX * -1);
    stepper.setMaxSpeed(s);
    stepper.setAcceleration(650);
  }
  stepper.run();
}

void pickSituation(int s) {
  if (s == 0) {
    currX = random(0 , limit);
    currY = currX;
    currZ = currX;
  }
  if (s == 1) {
    currX = random(0 , limit);
    currY = random(currX, limit);
    currZ = random(currX, currY);
  }
  if (currX > limit) {
    currX = limit;
  }
  if (currX < 0) {
    currX = 0;
  }
  if (currY > limit) {
    currY = limit;
  }
  if (currY < 0) {
    currY = 0;
  }
  if (currZ > limit) {
    currZ = limit;
  }
  if (currZ < 0) {
    currZ = 0;
  }
  String command = "#a" + String(currX) + "b" + String(currY) + "c" + String(currZ);
  decode_text(command);
}
