
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
      int seed = random(0, 3);
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
    currX = random(minLimitX , maxLimitX);
    currY = random(maxLimitY , maxLimitY);
    currZ = random(maxLimitZ , maxLimitZ);
  }
  if (s == 1) {
    currX = random(minLimitX , maxLimitX);
    currY = currX;
    currZ = currX;
  }
  if (s == 2) {
    currX = minLimitX;
    currY = minLimitY;
    currZ = minLimitZ;
  }
  if (currX > maxLimitX) {
    currX = maxLimitX;
  }
  if (currX < minLimitX) {
    currX = minLimitX;
  }
  if (currY > maxLimitY) {
    currY = maxLimitY;
  }
  if (currY < minLimitY) {
    currY = minLimitY;
  }
  if (currZ > maxLimitZ) {
    currZ = maxLimitZ;
  }
  if (currZ < minLimitZ) {
    currZ = minLimitZ;
  }
  String command = "#a" + String(currX) + "b" + String(currY) + "c" + String(currZ);
  decode_text(command);
}
