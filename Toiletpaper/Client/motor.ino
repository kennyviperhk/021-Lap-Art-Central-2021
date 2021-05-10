AccelStepper stepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);

void motorSetup()
{
  stepper.setMaxSpeed(0);
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);
}

void motorLoop()
{
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(currY);
    stepper.setMaxSpeed(s);
    stepper.setAcceleration(s);
  }
  stepper.run();
}
