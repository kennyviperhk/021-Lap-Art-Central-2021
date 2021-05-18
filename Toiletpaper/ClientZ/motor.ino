
void motorSetup()
{
  stepper.setMaxSpeed(0);
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);
}

void motorLoop()
{
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(currZ);
    stepper.setMaxSpeed(s);
    stepper.setAcceleration(650);
  }
  stepper.run();
}
